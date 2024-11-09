#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Necromancy {

using DirectXEndScene = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9);

HRESULT __stdcall HkEndScene_DumpMemory(LPDIRECT3DDEVICE9 device);
HRESULT InitDirect3D();

void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

void Setup();

}
