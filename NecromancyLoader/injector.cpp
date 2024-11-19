#include "pch.h"
#include "injector.h"

bool WinDllInjector::setTargetLibrary(WinConstStr libraryName) {
    return false;
}

bool WinDllInjector::setTargetProc(WinConstStr procName) {
    return false;
}

bool WinDllInjector::setTargetProcPid(WinDword procId) {
    return false;
}

WinResult WinDllInjector::inject() {
    return WinResult();
}

WinResult WinDllInjector::free() {
    return WinResult();
}

bool WinDllInjector::hasTargetLoadedLibrary() {
    return false;
}

WinResult WinDllInjector::targetFreeLibrary() {
    return WinResult();
}

bool WinDllInjector::isTargetLibraryX86() {
    return false;
}

bool WinDllInjector::isTargetProcX86() {
    return false;
}
