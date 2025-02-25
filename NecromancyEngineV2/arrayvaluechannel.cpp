#include "pch.h"

#include "arrayvaluechannel.h"

using namespace necromancy::hooks;

void ArrayValueChannel::init() {
    delete _instance;
    _instance = new ArrayValueChannel();

    _getFloatId = _instance->add("?GetFloat@Aco_Array_Value@@UAEMXZ");
}

float ArrayValueChannel::getFloat(void* object) {
    return _instance->_functions[_getFloatId](object);
}

bool ArrayValueChannel::allValid() {
    return ProxyMemoryObject::allValid(_instance);
}

ArrayValueChannel::ArrayValueChannel() : ProxyMemoryObject("DF5BF7F7-C204-4F6E-BDB8-666A53DFCC58.dll") {
}
