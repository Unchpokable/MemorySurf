#pragma once
#include <A3d_Channels.h>

namespace necromancy::typedefs {
//    void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

using ChannelGroup_GetPoolName = const char*(__thiscall*)(void*);
using ChannelGroup_GetChannel = void*(__thiscall*)(void*, std::int32_t);

using Channel_GetChannelName = const char*(__thiscall*)(void*);

using TrueCallChannelFn = void(__thiscall*)(void* self);
using A3d_Channel_TrueCallChannel = void(__thiscall*)(void* self);

using Aco_FloatChannel_GetFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_GetDefaultFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_SetFloat = void(__thiscall*)(void* self, float value);
using Aco_ArrayTable_GetTable = void*(__thiscall*)(void* self);

using A3d_List_GetItem = void*(__thiscall*)(void*, std::int32_t); // void* A3d_List::GetItem(std::int32_t idx);

using ArrayValue_GetFloat = float(__thiscall*)(void*);

// array table internals
}
