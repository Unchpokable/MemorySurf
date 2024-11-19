#include "pch.h"
#include "privileges.h"

bool RaisePrivilegesToDebug() {
    auto f_RtlAdjustPrivilege = reinterpret_cast<RtlAdjustPrivilegeFunc>(GetProcAddress(GetModuleHandleA("ntdll"), "RtlAdjustPrivilege"));
    boolean OldPrivilege;
    f_RtlAdjustPrivilege(SeDebugPrivileges, TRUE, FALSE, &OldPrivilege);

    return false;
}
