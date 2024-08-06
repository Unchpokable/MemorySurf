#pragma once

#include "ChannelCaller.hpp"

template<>
class ChannelCaller<float>::DefaultCaller
{
public:
    static ChannelCaller<float>* create(A3d_Channel* target);
    static std::vector<ChannelCaller<float>*> createForChannels(A3d_ChannelGroup* target, const std::vector<int>& desiredChannels);
};

template<>
class ChannelCaller<void*>::DefaultCaller
{
public:
    static ChannelCaller<void*>* create(A3d_Channel* target);
    static std::vector<ChannelCaller<void*>*> createForChannels(A3d_ChannelGroup* target, const std::vector<int>& desiredChannels);
};