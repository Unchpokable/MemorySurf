#pragma once

#include <cstdint>
#include <Windows.h>

enum InjectorMode : uint16_t {
    Inject = 0x692D, // -i
    Unload = 0x752D  // -u
};

struct InjectorArguments {
    InjectorArguments() = default;

    InjectorMode mode;
    DWORD procId;
    wchar_t dllPath[MAX_PATH];
};

InjectorArguments ParseArguments(int argc, char** argv);
