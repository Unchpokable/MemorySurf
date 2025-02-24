#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace necromancy {

using DirectXEndScene = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);

HRESULT __stdcall HkEndScene_DumpMemory(LPDIRECT3DDEVICE9 device);
void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

HRESULT InitDirect3D();

HWND GetProcWindow();

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);

void Setup(HMODULE thisDll);

DWORD WINAPI Unload(LPVOID lpThreadParameter);
DWORD WINAPI Main(LPVOID lpThreadParameter);

}
