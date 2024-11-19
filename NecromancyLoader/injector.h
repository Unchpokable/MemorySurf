#pragma once

#include "windefprettify.h"

class WinDllInjector final {
public:
    WinDllInjector() = default;
    ~WinDllInjector() = default;

    bool setTargetLibrary(WinConstStr libraryName);
    bool setTargetProc(WinConstStr procName);
    bool setTargetProcPid(WinDword procId);

    WinResult inject();
    WinResult free();

private:
    bool hasTargetLoadedLibrary();
    WinResult targetFreeLibrary();
    bool isTargetLibraryX86();
    bool isTargetProcX86();

    WinConstStr _targetLibraryName;
    WinDword _targetProcId;
};
