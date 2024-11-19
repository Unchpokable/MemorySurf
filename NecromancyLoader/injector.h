#pragma once

#include "windefprettify.h"

#pragma comment(lib, "ntdll.lib")

class WinDllInjector final {
public:
    WinDllInjector() = default;
    ~WinDllInjector() = default;

    void setTargetLibrary(const std::wstring& libraryName);
    bool setTargetProc(const std::wstring& procName);
    void setTargetProcPid(WinDword procId);

    WinResult inject() const;
    WinResult free() const;

private:
    static WinDword getProcessId(const std::wstring& procName);

    bool hasTargetLoadedLibrary() const;
    bool isTargetLibraryX86();
    bool isTargetProcX86();
    WinModuleHandle findTargetDll() const;

    WinResult targetFreeLibrary() const;

    std::wstring _targetLibraryName;
    WinDword _targetProcId;
};
