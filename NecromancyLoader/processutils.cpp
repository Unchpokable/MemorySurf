#include "pch.h"
#include "processutils.h"

WinDword ProcessUtils::getProcessId(const std::wstring& procName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if(procName == processInfo.szExeFile) {
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
