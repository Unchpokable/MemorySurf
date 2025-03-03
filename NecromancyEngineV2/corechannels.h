#pragma once

#include "proxymemoryobject.h"
#include "signatures.h"
#include "virtualfunction.h"

class A3d_Channel;

namespace necromancy::hooks {
class CoreChannels : public ProxyMemoryObject
{
public:
    STATIC_DYNAMIC_CALL

    static void init();

    static VirtualFunction<signatures::ChannelGroup_GetChannel> getChannel();
    static VirtualFunction<signatures::ChannelGroup_GetPoolName> getPoolName();
    static VirtualFunction<signatures::Channel_GetChannelName> getChannelName();
    static VirtualFunction<signatures::TrueCallChannelFn> trueCallChannel();

    static bool allValid();

protected:
    explicit CoreChannels();

private:
    static CoreChannels* _instance;

    static std::size_t _getChannelId;
    static std::size_t _getPoolNameId;
    static std::size_t _getChannelNameId;
    static std::size_t _trueCallChannelId;
};
}
