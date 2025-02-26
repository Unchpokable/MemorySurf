#pragma once

#include <type_traits>

#include "proxymemoryobject.h"
#include "virtualfunction.h"
#include "typedefs.h"

class A3d_Channel;

namespace necromancy::hooks {

template<typename T>
concept Quest3DChannel = std::is_base_of_v<A3d_Channel, std::remove_pointer_t<T>> || std::is_same_v<std::remove_pointer_t<T>*, void*>;

class CoreChannels : public ProxyMemoryObject {
public:
    STATIC_DYNAMIC_CALL;

    static void init();

    static VirtualFunction<typedefs::ChannelGroup_GetChannel> getChannel();
    static VirtualFunction<typedefs::ChannelGroup_GetPoolName> getPoolName();
    static VirtualFunction<typedefs::Channel_GetChannelName> getChannelName();
    static VirtualFunction<typedefs::TrueCallChannelFn> trueCallChannel();

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

