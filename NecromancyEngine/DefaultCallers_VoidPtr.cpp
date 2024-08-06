#include "pch.h"
#include "DefaultCallers.hpp"
#include "StaticQuest3DFuncs.h"

ChannelCaller<void*>* ChannelCaller<void*>::DefaultCaller::create(A3d_Channel* target)
{
    auto reader = new ChannelCaller<void*>;
    reader->assignChannel(target);
    reader->assignGet(Aco_ArrayTable_GetTable);

    return reader;
}
