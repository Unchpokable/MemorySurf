// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "load.h"

#pragma comment(lib, "d3dx9.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(nullptr, 0, necromancy::Main, hModule, 0, nullptr);
        return TRUE;
    }   
    case DLL_PROCESS_DETACH:
    {
        CreateThread(nullptr, 0, necromancy::Unload, hModule, 0, nullptr);
        return TRUE;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

