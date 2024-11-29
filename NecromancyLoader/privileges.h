#pragma once

#include "windefprettify.h"

constexpr WinDword SeDebugPrivileges = 20;

// typedef NTSTATUS(NTAPI* RtlAdjustPrivilegeFunc)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
using RtlAdjustPrivilegeFunction = NTSTATUS(NTAPI*)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
bool RaisePrivilegesToDebug();
