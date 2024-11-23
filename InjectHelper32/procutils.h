#pragma once

#include <Windows.h>
#include "arguments.h"

#define LAST_ERROR HRESULT_FROM_WIN32(GetLastError())

HRESULT InjectProcess(const InjectorArguments& args);

HRESULT UnloadProcess(const InjectorArguments& args);

HMODULE FindDll(HANDLE proc, const wchar_t* dllName);

const wchar_t* LibName(const wchar_t* libPath);
