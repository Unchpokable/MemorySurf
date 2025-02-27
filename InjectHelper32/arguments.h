#pragma once

#include <cstdint>
#include <Windows.h>

enum InjectorMode : uint8_t
{
    Inject = 105, // -i
    Unload = 117  // -u
};

struct InjectorArguments
{
    InjectorArguments() = default;

    InjectorMode mode;
    DWORD procId;
    wchar_t dllPath[MAX_PATH];
};

InjectorArguments ParseArguments(int argc, char** argv);
