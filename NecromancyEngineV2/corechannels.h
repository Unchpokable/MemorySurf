#pragma once

#include <type_traits>

#include "proxymemoryobject.h"

namespace necromancy::hooks {

class A3d_Channel;

template<typename T>
concept Quest3DChannel = std::is_base_of_v<A3d_Channel, std::remove_pointer_t<T>> || std::is_same_v<std::remove_pointer_t<T>*, void*>;

class CoreChannels : public ProxyMemoryObject {
public:
    static void init();

    template<Quest3DChannel Ch = void>
    static const Ch* getChannel(void* object, std::int32_t id);

    static const char* getPoolName(void* object);
    static const char* getChannelName(void* object);
    static void trueCallChannel(void* object);

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

template <Quest3DChannel Ch>
const Ch* CoreChannels::getChannel(void* object, std::int32_t id) {
    auto channel = _instance->_functions[_getChannelId](object, id);

    return reinterpret_cast<Ch*>(channel);
}

}

