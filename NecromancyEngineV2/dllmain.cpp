﻿// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "load.h"
#include "taggedexception.hpp"

#pragma comment(lib, "d3dx9.lib")

LPCWSTR ConvertToLPCWSTR(const char* charString) {
    int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charString, -1, nullptr, 0);

    wchar_t* wString = new wchar_t[bufferSize];

    MultiByteToWideChar(CP_UTF8, 0, charString, -1, wString, bufferSize);

    return wString;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#ifdef _DEBUG
        //MessageBox(NULL, "Im in!", "Hello from Necromancy", MB_OK | MB_ICONINFORMATION);
#endif
        CreateThread(nullptr, 0, Necromancy::Main, hModule, 0, nullptr);
        return TRUE;
    }   
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        //CreateThread(nullptr, 0, Necromancy::Unload, nullptr, 0, nullptr);
        break;
    }
    return TRUE;
}

