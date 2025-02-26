#include "pch.h"

#include "corechannels.h"

#include "typedefs.h"

using namespace necromancy::hooks;
using namespace necromancy::typedefs;

CoreChannels* CoreChannels::_instance = nullptr;
std::size_t CoreChannels::_getChannelId = -1;
std::size_t CoreChannels::_getPoolNameId = -1;
std::size_t CoreChannels::_getChannelNameId = -1;
std::size_t CoreChannels::_trueCallChannelId = -1;

void CoreChannels::init() {
    delete _instance;
    _instance = new CoreChannels();

    _getChannelId = _instance->add("?GetChannel@A3d_ChannelGroup@@UAEPAVA3d_Channel@@H@Z");
    _getPoolNameId = _instance->add("?GetPoolName@A3d_ChannelGroup@@UAEPBDXZ");
    _getChannelNameId = _instance->add("?GetChannelName@A3d_Channel@@QAEPBDXZ");
    _trueCallChannelId = _instance->add("?CallChannel@A3d_Channel@@UAEXXZ");
}

VirtualFunction<ChannelGroup_GetChannel> CoreChannels::getChannel() {
    return _instance->_functions[_getChannelId];
}

VirtualFunction<ChannelGroup_GetPoolName> CoreChannels::getPoolName() {
    return _instance->_functions[_getPoolNameId];
}

VirtualFunction<Channel_GetChannelName> CoreChannels::getChannelName() {
    return _instance->_functions[_getChannelNameId];
}

VirtualFunction<TrueCallChannelFn> CoreChannels::trueCallChannel() {
    return _instance->_functions[_trueCallChannelId];
}

bool CoreChannels::allValid() {
    return ProxyMemoryObject::allValid(_instance);
}

CoreChannels::CoreChannels() : ProxyMemoryObject("highpoly.dll") {
}
