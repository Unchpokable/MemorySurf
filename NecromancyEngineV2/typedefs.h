#pragma once
#include <A3d_Channels.h>

namespace Necromancy::Typedefs {

//    void __fastcall HkTrueCallChannel(A3d_Channel* self, DWORD edx);

using ChannelGroup_GetPoolName = const char*(__thiscall*)(A3d_ChannelGroup*);
using ChannelGroup_GetChannel = A3d_Channel*(__thiscall*)(A3d_ChannelGroup*, std::int32_t);

using Channel_GetChannelName = const char*(__thiscall*)(A3d_Channel*);

using TrueCallChannelFn = void(__thiscall*)(A3d_Channel* self);
using A3d_Channel_TrueCallChannel = void(__thiscall*)(A3d_Channel* self);

using Aco_FloatChannel_GetFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_GetDefaultFloat = float(__thiscall*)(void* self);
using Aco_FloatChannel_SetFloat = void(__thiscall*)(void* self, float value);
using Aco_ArrayTable_GetTable = void*(__thiscall*)(void* self);

using ArrayTable_GetElementCount = std::int32_t(__thiscall*)(void*); // std::int32_t Aco_ArrayTable::GetRowsCount() or smth
using ArrayTable_GetElementAtIndex = void*(__thiscall*)(void*, std::int32_t); // void* Aco_ArrayTable::GetRow(std::int32_t idx) or smth

using A3d_List_GetItem = void*(__thiscall*)(void*, std::int32_t); // void* A3d_List::GetItem(std::int32_t idx);

// array table internals

// for now lets suppose that ArrayTable class working with something, called "ArrayConnectItem" so

using ArrayConnectItem_GetRow = void*(__thiscall*)(void*, std::int32_t); // todo: extract functions from quest3d
using ArrayConnectItem_GetColumn = void*(__thiscall*)(void*); // lol GetColumn do not need an index pog wtf WHY
using ArrayConnectItem_GetRowCount = std::int32_t(__thiscall*)(void*);

}
