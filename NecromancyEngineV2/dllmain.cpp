﻿// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "load.h"

#pragma comment(lib, "d3dx9.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(nullptr, 0, Necromancy::Main, hModule, 0, nullptr);
        return TRUE;
    }   
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

