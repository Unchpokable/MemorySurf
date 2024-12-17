#pragma once
#include <A3d_Channels.h>

namespace Necromancy::Typedefs {

//    void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

using ChannelGroup_GetPoolName = const char*(__thiscall*)(A3d_ChannelGroup*);

using TrueCallChannelFn = void(__thiscall*)(A3d_Channel* self);
using A3d_Channel_TrueCallChannel = void(__thiscall*)(A3d_Channel* self);

using Aco_FloatChannel_GetFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_GetDefaultFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_SetFloat = void(__thiscall*)(void* self, float value);
using Aco_ArrayTable_GetTable = void*(__thiscall*)(void* self);

using ArrayTable_GetElementCount = int(__thiscall*)(void*);
using ArrayTable_GetElementAtIndex = void*(__thiscall*)(void*, int);

}
