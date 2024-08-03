#include "pch.h"
#include "StaticQuest3DFuncs.h"

#include "EngineInterfaceProxy.h"

void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx)
{
    if(!EngineInterfaceProxy::getEngine())
        EngineInterfaceProxy::setEngine(self->engine);
    TrueCallChannel(self);
}

void InitTrueCallChannel()
{
    TrueCallChannel =
        static_cast<void(__thiscall*)(A3d_Channel*)>(DetourFindFunction("highpoly.dll", "?CallChannel@A3d_Channel@@UAEXXZ"));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueCallChannel, HkTrueCallChannel);
    DetourTransactionCommit();
}

void DetourQ3dFunctions() {
    Aco_FloatChannel_GetFloat =
        (float(__thiscall*)(void*))
        DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll", "?GetFloat@Aco_FloatChannel@@UAEMXZ");
    Aco_FloatChannel_GetDefaultFloat =
        (float(__thiscall*)(void*))
        DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll", "?GetDefaultFloat@Aco_FloatChannel@@UAEMXZ");
    Aco_FloatChannel_SetFloat =
        (void(__thiscall*)(void*, float))
        DetourFindFunction("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll", "?SetFloat@Aco_FloatChannel@@UAEXM@Z");
}
