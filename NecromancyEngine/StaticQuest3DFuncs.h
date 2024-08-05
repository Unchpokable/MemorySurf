#pragma once

static void(__thiscall* TrueCallChannel)(A3d_Channel* self) = nullptr;

static float(__thiscall* Aco_FloatChannel_GetFloat)(void* self) = nullptr;
static float(__thiscall* Aco_FloatChannel_GetDefaultFloat)(void* self) = nullptr;
static void(__thiscall* Aco_FloatChannel_SetFloat)(void* self, float value) = nullptr;

static void*(__thiscall* Aco_ArrayTable_GetTable)(void* self) = nullptr;
static int(__thiscall* Aco_ArrayTable_GetTimeStamp)(void* self) = nullptr;

void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

void InitTrueCallChannel();

void DetourQ3dFunctions();
