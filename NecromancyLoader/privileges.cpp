#include "pch.h"
#include "privileges.h"

#include <winternl.h>

bool RaisePrivilegesToDebug()
{
    HANDLE hToken;
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return false;
    }

    TOKEN_PRIVILEGES tp;
    LUID luid;
    if(!LookupPrivilegeValueW(nullptr, L"SeDebugPrivilege", &luid)) {
        CloseHandle(hToken);
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    TOKEN_PRIVILEGES previousTp;
    DWORD returnLength;
    if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(previousTp), &previousTp, &returnLength)) {
        CloseHandle(hToken);
        return false;
    }

    const bool wasChanged = (previousTp.Privileges[0].Attributes != SE_PRIVILEGE_ENABLED);
    CloseHandle(hToken);
    return wasChanged;
}
