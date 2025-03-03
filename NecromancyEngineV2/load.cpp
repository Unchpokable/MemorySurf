#include "pch.h"

#include "load.h"
#include "engine.h"
#include "genericutils.h"
#include "hook.h"
#include "logger.h"
#include "taggedexception.hpp"
#include "corechannels.h"

#define UNUSED(x) (void)x;

namespace {
// constants

constexpr ptrdiff_t DirectX_EndSceneOffset = 42;

// runtime global variables
constexpr std::chrono::duration<long long, std::milli> g_deltaTime { 100 };

necromancy::NecromancyEngine* g_necromancyEngine = nullptr;
necromancy::hooks::Hook* g_endSceneHook = nullptr;
necromancy::hooks::Hook* g_trueCallChannelHook = nullptr;
std::chrono::time_point<std::chrono::steady_clock> g_timePoint;

// Dll data
HMODULE g_this;
}

HRESULT __stdcall necromancy::HkEndScene_DumpMemory(LPDIRECT3DDEVICE9 device)
{
    auto now = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(now - g_timePoint) < g_deltaTime) {
        g_timePoint = now;
        return g_endSceneHook->original<DirectXEndScene>()(device);
    }

    g_necromancyEngine->dump();
    g_necromancyEngine->send();

    return g_endSceneHook->original<DirectXEndScene>()(device);
}

void __fastcall necromancy::HkTrueCallChannel(A3d_Channel* self, DWORD edx)
{
    if(g_necromancyEngine->engineInterface() == nullptr) {
        g_necromancyEngine->setQ3DEngineInterface(self->engine);
        Logger::logCondition(notNull(g_necromancyEngine->engineInterface()), "Engine interface is available");

        g_necromancyEngine->setupChannelReaders();
        g_trueCallChannelHook->detach();
        if(g_endSceneHook->attach() != hooks::Status::Ok) {
            Logger::panic("Hooks", "Attaching hook to DirectX EndScene failed - load.cpp:HkTrueCallChannel");
            Unload(nullptr);
        }

        Logger::info("Setup step 2 of 2 - hook EndScene succeeded");
        return g_trueCallChannelHook->original<signatures::TrueCallChannelFn>()(self);
    }

    return g_trueCallChannelHook->original<signatures::TrueCallChannelFn>()(self);
}

HRESULT necromancy::InitDirect3D()
{
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if(!pD3D) {
        Logger::critical("Environment", "Unable to initialize Direct3D 9 context");
        return E_FAIL;
    }

    LPDIRECT3DDEVICE9 pDevice = nullptr;

    auto window = GetProcWindow();

    D3DPRESENT_PARAMETERS params = {};
    params.Windowed = false;
    params.SwapEffect = D3DSWAPEFFECT_DISCARD;
    params.hDeviceWindow = window;

    auto result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice);

    if(FAILED(result) || !pDevice) {
        Logger::critical("Environment", "Unable to create DirectX device - load.cpp:InitDirect3D. Trying alternative parameters...");

        params.Windowed = !params.Windowed;
        result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, &pDevice);
        if(FAILED(result) || !pDevice) {
            pD3D->Release();
            Logger::critical("Environment", "Unable to create DirectX device - load.cpp:InitDirect3D");
            return E_FAIL;
        }
    }

    void** vTable = *reinterpret_cast<void***>(pDevice);
    auto endScene = reinterpret_cast<DirectXEndScene>(vTable[DirectX_EndSceneOffset]);
    if(g_endSceneHook != nullptr) {
        g_endSceneHook->detach();
        delete g_endSceneHook;
    }

    g_endSceneHook = new hooks::Hook(endScene, HkEndScene_DumpMemory);

    pDevice->Release();
    pD3D->Release();

    Logger::info("DirectX environment initialized");

    return 0;
}

HWND necromancy::GetProcWindow()
{
    HWND mainWindow = nullptr;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&mainWindow));
    return mainWindow;
}

BOOL CALLBACK necromancy::EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
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

void necromancy::Setup(HMODULE thisDll)
{
    g_this = thisDll;

    g_necromancyEngine = new NecromancyEngine();
    g_trueCallChannelHook = new hooks::Hook(hooks::CoreChannels::trueCallChannel(), HkTrueCallChannel);

    if(FAILED(InitDirect3D())) {
        Logger::panic("DirectX", "Failed to initialize DirectX environment. Hook setup failed");
        Unload(nullptr);
    }

    auto callChannelStatus = g_trueCallChannelHook->attach();
    if(callChannelStatus == hooks::Status::DetourException) {
        Logger::panic("Hooks", "Critical exception during attaching TrueCallChannel hook");
        Unload(nullptr);
    }

    Logger::info("Setup step 1 of 2 succeeded");
}

DWORD WINAPI necromancy::Unload(LPVOID lpThreadParameter)
{
    UNUSED(lpThreadParameter);

    g_endSceneHook->detach();
    delete g_endSceneHook;

    // g_trueCallChannelHook->detach(); // should be detached after attaching an EndScene
    delete g_trueCallChannelHook;

    delete g_necromancyEngine;

    Logger::info("Shutting down");
    FreeLibraryAndExitThread(g_this, S_OK);
}

DWORD WINAPI necromancy::Main(LPVOID lpThreadParameter)
{
    Setup((HMODULE)lpThreadParameter);

    Logger::enableBuffering();
    Logger::setBufferSize(3);

    return TRUE;
}
