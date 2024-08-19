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

std::vector<ChannelCaller<void*>*> ChannelCaller<void*>::DefaultCaller::createForChannels(A3d_ChannelGroup* target, const std::vector<int>& desiredChannels)
{
    std::vector<ChannelCaller<void*>*> result;
    result.reserve(desiredChannels.size());

    for (auto idx : desiredChannels) 
    {
        result.push_back(ChannelCaller<void*>::DefaultCaller::create(target->GetChannel(idx)));
    }

    return result;
}
