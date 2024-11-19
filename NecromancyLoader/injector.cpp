// ReSharper disable CppCStyleCast
#include "pch.h"
#include "injector.h"

void WinDllInjector::setTargetLibrary(const std::wstring& libraryName) {
    _targetLibraryName = libraryName;
}

bool WinDllInjector::setTargetProc(const std::wstring& procName) {
    auto procId = getProcessId(procName);

    _targetProcId = procId;
    return true;
}

void WinDllInjector::setTargetProcPid(WinDword procId) {
    _targetProcId = procId;
}

WinResult WinDllInjector::inject() const {
    WinHandle procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _targetProcId);
    auto dllPath = _targetLibraryName.c_str();
    size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

    if(procHandle) {
        auto loadLibAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
        LPVOID targetProcAllocMem = VirtualAllocEx(procHandle, NULL, dllPathSize,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if(targetProcAllocMem == nullptr)
            return E_FAIL;

        if(!WriteProcessMemory(procHandle, targetProcAllocMem, dllPath, dllPathSize, NULL)) {
            VirtualFreeEx(procHandle, targetProcAllocMem, 0, MEM_RELEASE);
            CloseHandle(procHandle);
            return E_FAIL;
        }

        HANDLE hRemoteThread = CreateRemoteThread(procHandle, NULL, NULL,
            (LPTHREAD_START_ROUTINE)loadLibAddr, targetProcAllocMem, 0, NULL);

        if(hRemoteThread == nullptr) {
            VirtualFreeEx(procHandle, targetProcAllocMem, 0, MEM_RELEASE);
            CloseHandle(procHandle);
            return E_FAIL;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);
        VirtualFreeEx(procHandle, targetProcAllocMem, dllPathSize, MEM_RELEASE);
        CloseHandle(hRemoteThread);
        CloseHandle(procHandle);
        return 0;
    }

    return E_FAIL;
}

WinResult WinDllInjector::free() const
{
    return targetFreeLibrary();
}

bool WinDllInjector::hasTargetLoadedLibrary() const {
    return findTargetDll() != NULL;
}

WinResult WinDllInjector::targetFreeLibrary() const
{
    WinHandle procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _targetProcId);

    auto freeLibraryAddress = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary");
    if(!freeLibraryAddress) {
        return E_FAIL;
    }

    auto dll = findTargetDll();
    if(!dll) {
        return E_FAIL;
    }

    WinHandle thread = CreateRemoteThread(procHandle, nullptr, 0, (LPTHREAD_START_ROUTINE)freeLibraryAddress, dll, 0, nullptr);
    if(!thread) {
        return E_FAIL;
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    CloseHandle(procHandle);
    return 0;
}

bool WinDllInjector::isTargetLibraryX86() {
    return true; // todo
}

bool WinDllInjector::isTargetProcX86() {
    return true; // todo
}

WinModuleHandle WinDllInjector::findTargetDll() const
{
    WinHandle procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _targetProcId);
    if(!procHandle) {
        // todo replace exceptions to status codes
        throw std::runtime_error("Denied access to process:");
    }

    WinModuleHandle moduleArray[1024];
    WinDword bytesRequired;

    if(!EnumProcessModulesEx(procHandle, moduleArray, sizeof(moduleArray), &bytesRequired, LIST_MODULES_32BIT)) {
        throw std::runtime_error("Unable to enumerate process modules");
    }

    size_t modulesCount = bytesRequired / sizeof(WinModuleHandle);

    for(size_t i { 0 }; i < modulesCount; i++) {
        wchar_t moduleName[MAX_PATH] = { 0 };

        if(GetModuleBaseNameW(procHandle, moduleArray[i], moduleName, MAX_PATH)) {
            if(_wcsicmp(_targetLibraryName.c_str(), moduleName) == 0) {
                CloseHandle(procHandle);
                return moduleArray[i];
            }
        }
    }
    CloseHandle(procHandle);
    return NULL;
}

WinDword WinDllInjector::getProcessId(const std::wstring& procName) {

    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if(procName != processInfo.szExeFile) {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while(Process32Next(processesSnapshot, &processInfo)) {
        if(procName == processInfo.szExeFile) {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}