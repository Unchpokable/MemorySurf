#pragma once

#include <cstdint>
#include <Windows.h>

enum InjectorMode : uint16_t {
    Inject = 0x6900, // -i
    Unload = 0x7500  // -u
};

struct InjectorArguments {
    InjectorArguments();

    InjectorMode mode;
    DWORD procId;
    wchar_t dllPath[MAX_PATH];
};

InjectorArguments ParseArguments(int argc, char** argv);
