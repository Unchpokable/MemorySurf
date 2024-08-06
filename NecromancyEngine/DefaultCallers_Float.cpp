#include "pch.h"
#include "DefaultCallers.hpp"
#include "StaticQuest3DFuncs.h"

ChannelCaller<float>* ChannelCaller<float>::DefaultCaller::create(A3d_Channel* target) {
    auto reader = new ChannelCaller();
    reader->assignChannel(target);
    reader->assignGet(Aco_FloatChannel_GetFloat);
    reader->assignGetDefault(Aco_FloatChannel_GetDefaultFloat);
    reader->assignSet(Aco_FloatChannel_SetFloat);
    return reader;
}

std::vector<ChannelCaller<float>*> ChannelCaller<float>::DefaultCaller::createForChannels(
        A3d_ChannelGroup* target, const std::vector<int>& desiredChannels) {
    std::vector<ChannelCaller<float>*> result;

    result.reserve(desiredChannels.size());

    for (auto idx : desiredChannels)
    {
        result.push_back(new ChannelCaller(target->GetChannel(idx)));
    }

    return result;
}
