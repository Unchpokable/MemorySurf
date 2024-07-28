#pragma once

EngineInterface* Q3d_Engine { nullptr };

void(__thiscall* TrueCallChannel)(A3d_Channel* self) { nullptr };

float(__thiscall* Aco_FloatChannel_GetFloat)(void* self) { nullptr };
float(__thiscall* Aco_FloatChannel_GetDefaultFloat)(void* self) { nullptr };
void(__thiscall* Aco_FloatChannel_SetFloat)(void* self, float value) { nullptr };

inline void __fastcall hkTrueCallChannel(A3d_Channel* self, DWORD edx)
{
    if(!Q3d_Engine)
        Q3d_Engine = self->engine;
    TrueCallChannel(self);
}

inline void initTrueCallChannel()
{
    TrueCallChannel =
        static_cast<void(__thiscall*)(A3d_Channel*)>(DetourFindFunction("highpoly.dll", "?CallChannel@A3d_Channel@@UAEXXZ"));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)TrueCallChannel, hkTrueCallChannel);
    DetourTransactionCommit();
}

void DetourQ3dFunctions();