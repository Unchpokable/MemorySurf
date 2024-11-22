#pragma once

#include <Windows.h>
#include "arguments.h"

HRESULT InjectProcess(const InjectorArguments& args);

HRESULT UnloadProcess(const InjectorArguments& args);

HMODULE FindDll(HANDLE Proc, const wchar_t* dllName);

const wchar_t* LibName(const wchar_t* libPath);
