#pragma once

void(__thiscall* TrueCallChannel)(A3d_Channel* self) { nullptr };

float(__thiscall* Aco_FloatChannel_GetFloat)(void* self) { nullptr };
float(__thiscall* Aco_FloatChannel_GetDefaultFloat)(void* self) { nullptr };
void(__thiscall* Aco_FloatChannel_SetFloat)(void* self, float value) { nullptr };

inline void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

inline void InitTrueCallChannel();

void DetourQ3dFunctions();
