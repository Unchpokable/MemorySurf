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
}
