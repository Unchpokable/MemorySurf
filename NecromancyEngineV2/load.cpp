#include "pch.h"
#include "engine.h"
#include "load.h"

#include "hook.h"
#include "taggedexception.hpp"

namespace {

// constants

constexpr ptrdiff_t DirectX_EndSceneOffset = 42;

// runtime global variables
constexpr std::chrono::duration<long long, std::milli> g_deltaTime { 100 };

Necromancy::NecromancyEngine* g_necromancyEngine = nullptr;
Necromancy::Detours::Hook* g_endSceneHook = nullptr;
Necromancy::Detours::Hook* g_trueCallChannelHook = nullptr;
std::chrono::time_point<std::chrono::steady_clock> g_timePoint;

// Dll data
HMODULE g_this;

}

HRESULT __stdcall Necromancy::HkEndScene_DumpMemory(LPDIRECT3DDEVICE9 device) {
    auto now = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(now - g_timePoint) < g_deltaTime) {
        g_timePoint = now;
        return g_endSceneHook->original<DirectXEndScene>()(device);
    }

    g_necromancyEngine->dump();
    g_necromancyEngine->send();

    return g_endSceneHook->original<DirectXEndScene>()(device);
}

void __fastcall Necromancy::HkTrueCallChannel(A3d_Channel* self, DWORD edx) {
    if(g_necromancyEngine->engineInterface() == nullptr) {
        g_necromancyEngine->setQ3DEngineInterface(self->engine);
        g_necromancyEngine->setupChannelReaders();
        g_trueCallChannelHook->detach();
        if(g_endSceneHook->attach() != Detours::Status::Ok) {
            throw std::runtime_error("Unable to attach hook to EndScene");
        }
        return g_trueCallChannelHook->original<Typedefs::TrueCallChannelFn>()(self);
    }

    return g_trueCallChannelHook->original<Typedefs::TrueCallChannelFn>()(self);
}

HRESULT Necromancy::InitDirect3D() {
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if(!pD3D)
        return E_FAIL;

    LPDIRECT3DDEVICE9 pDevice = nullptr;

    auto window = GetProcWindow();

    D3DPRESENT_PARAMETERS params = {};
    params.Windowed = false;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = window;

    auto result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice);

    if(FAILED(result) || !pDevice) {
        params.Windowed = !params.Windowed;
        result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice);
        if(FAILED(result) || !pDevice) {
            pD3D->Release();
            return E_FAIL;
        }
    }

    void** vTable = *reinterpret_cast<void***>(pDevice);
    auto endScene = reinterpret_cast<DirectXEndScene>(vTable[DirectX_EndSceneOffset]);
    if(g_endSceneHook != nullptr) {
        g_endSceneHook->detach();
        delete g_endSceneHook;
    }

    g_endSceneHook = new Detours::Hook(endScene, HkEndScene_DumpMemory);

    pDevice->Release();
    pD3D->Release();

    return 0;
}

HWND Necromancy::GetProcWindow() {
    HWND mainWindow = nullptr;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&mainWindow));
    return mainWindow;
}

BOOL CALLBACK Necromancy::EnumWindowsProc(HWND hwnd, LPARAM lparam) {
    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);

    if(processId == GetCurrentProcessId()) {
        if(IsWindowVisible(hwnd)) {
            HWND* targetHwnd = reinterpret_cast<HWND*>(lparam);
            *targetHwnd = hwnd;
            return FALSE;
        }
    }
    return TRUE;
}

void Necromancy::Setup(HMODULE thisDll) {
    g_this = thisDll;

    g_necromancyEngine = new NecromancyEngine();
    g_trueCallChannelHook = new Detours::Hook(g_necromancyEngine->functions().get<Typedefs::TrueCallChannelFn>("TrueCallChannelFn"), HkTrueCallChannel);
    auto callChannelStatus = g_trueCallChannelHook->attach();

    if(callChannelStatus == Detours::Status::DetourException) {
        throw RuntimeException("Critical exception during attaching TrueCallChannel hook");
    }

    if(callChannelStatus == Detours::Status::InvalidHookMode) {
        throw LogicException("Trying to hook with unstable attach using non-unstable hook mode");
    }

    if(FAILED(InitDirect3D())) {
        g_trueCallChannelHook->detach();
        FreeLibrary(g_this);
        throw RuntimeException("Unable to attach hook to DirectX EndScene");
    }

}

DWORD __stdcall Necromancy::Main(LPVOID lpThreadParameter) {
    Setup((HMODULE)lpThreadParameter);

    return TRUE;
}

