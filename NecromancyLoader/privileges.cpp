#include "pch.h"
#include "privileges.h"

bool RaisePrivilegesToDebug() {
    auto rtlAdjustPrivilege = reinterpret_cast<RtlAdjustPrivilegeFunction>(GetProcAddress(GetModuleHandleA("ntdll"), "RtlAdjustPrivilege"));
    boolean OldPrivilege;
    rtlAdjustPrivilege(SeDebugPrivileges, TRUE, FALSE, &OldPrivilege);

    return false;
}
