// ReSharper disable CppCStyleCast
// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "procutils.h"

#include <Psapi.h>
#include <stdexcept>

HRESULT InjectProcess(const InjectorArguments& args) {
    HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, args.procId);

    auto existingDll = FindDll(procHandle, LibName(args.dllPath));
    if(existingDll != INVALID_HANDLE_VALUE || existingDll != NULL) {
        return E_FAIL;
    }

    auto dllPath = args.dllPath;
    size_t dllPathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);

    if(procHandle) {
        auto loadLibAddr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
        LPVOID targetProcAllocMem = VirtualAllocEx(procHandle, NULL, dllPathSize,
            MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if(targetProcAllocMem == nullptr) {
            return LAST_ERROR;
        }

        if(!WriteProcessMemory(procHandle, targetProcAllocMem, dllPath, dllPathSize, NULL)) {
            VirtualFreeEx(procHandle, targetProcAllocMem, 0, MEM_RELEASE);
            CloseHandle(procHandle);
            return LAST_ERROR;
        }
        HANDLE hRemoteThread = CreateRemoteThread(procHandle, NULL, NULL,
            (LPTHREAD_START_ROUTINE)loadLibAddr, targetProcAllocMem, 0, NULL);

        if(hRemoteThread == nullptr) {
            VirtualFreeEx(procHandle, targetProcAllocMem, 0, MEM_RELEASE);
            CloseHandle(procHandle);
            return LAST_ERROR;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);
        VirtualFreeEx(procHandle, targetProcAllocMem, dllPathSize, MEM_RELEASE);
        CloseHandle(hRemoteThread);
        CloseHandle(procHandle);
        return S_OK;
    }

    return LAST_ERROR;
}

HRESULT UnloadProcess(const InjectorArguments& args) {
    HANDLE procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, args.procId);

    auto dll = FindDll(procHandle, LibName(args.dllPath));
    if(!dll) {
        return E_FAIL;
    }

    auto freeLibraryAddress = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "FreeLibrary");
    if(!freeLibraryAddress) {
        return E_FAIL;
    }

    HANDLE thread = CreateRemoteThread(procHandle, nullptr, 0, (LPTHREAD_START_ROUTINE)freeLibraryAddress, dll, 0, nullptr);
    if(!thread) {
        return E_FAIL;
    }

    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    CloseHandle(procHandle);
    return 0;
}

HMODULE FindDll(HANDLE proc, const wchar_t* dllName) {
    if(!proc) {
        // todo replace exceptions to status codes
        throw std::runtime_error("Denied access to process:");
    }

    HMODULE moduleArray[1024];
    DWORD bytesRequired;

    if(!EnumProcessModulesEx(proc, moduleArray, sizeof(moduleArray), &bytesRequired, LIST_MODULES_32BIT)) {
        throw std::runtime_error("Unable to enumerate process modules");
    }

    size_t modulesCount = bytesRequired / sizeof(HMODULE);

    for(size_t i { 0 }; i < modulesCount; i++) {
        wchar_t moduleName[MAX_PATH] = { 0 };

        if(GetModuleBaseNameW(proc, moduleArray[i], moduleName, MAX_PATH)) {
            if(_wcsicmp(dllName, moduleName) == 0) {
                return moduleArray[i];
            }
        }
    }
    return NULL;
}

const wchar_t* LibName(const wchar_t* libPath) {
    const wchar_t* lastBackslash = wcsrchr(libPath, L'\\');
    return lastBackslash ? lastBackslash + 1 : libPath;
}
