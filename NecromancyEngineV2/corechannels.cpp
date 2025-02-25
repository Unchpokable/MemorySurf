#include "pch.h"

#include "corechannels.h"

using namespace necromancy::hooks;

void CoreChannels::init() {
    delete _instance;
    _instance = new CoreChannels();

    _getChannelId = _instance->add("?GetChannel@A3d_ChannelGroup@@UAEPAVA3d_Channel@@H@Z");
    _getPoolNameId = _instance->add("?GetPoolName@A3d_ChannelGroup@@UAEPBDXZ");
    _getChannelNameId = _instance->add("?GetChannelName@A3d_Channel@@QAEPBDXZ");
    _trueCallChannelId = _instance->add("?CallChannel@A3d_Channel@@UAEXXZ");
}

const char* CoreChannels::getPoolName(void* object) {
    return _instance->_functions[_getPoolNameId](object);
}

const char* CoreChannels::getChannelName(void *object) {
    return _instance->_functions[_getChannelNameId](object);
}

void CoreChannels::trueCallChannel(void *object) {
    _instance->_functions[_trueCallChannelId](object);
}

CoreChannels::CoreChannels() : ProxyMemoryObject("highpoly.dll") {
}
