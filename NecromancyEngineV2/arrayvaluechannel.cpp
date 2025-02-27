#include "pch.h"

#include "arrayvaluechannel.h"

using namespace necromancy::hooks;
using namespace necromancy::typedefs;

ArrayValueChannel* ArrayValueChannel::_instance = nullptr;
std::size_t ArrayValueChannel::_getFloatId = -1;
std::size_t ArrayValueChannel::_setFloatId = -1;

void ArrayValueChannel::init()
{
    delete _instance;
    _instance = new ArrayValueChannel();

    _getFloatId = _instance->add("?GetFloat@Aco_Array_Value@@UAEMXZ");
}

VirtualFunction<ArrayValue_GetFloat> ArrayValueChannel::getFloat()
{
    return _instance->_functions[_getFloatId];
}

bool ArrayValueChannel::allValid()
{
    return ProxyMemoryObject::allValid(_instance);
}

ArrayValueChannel::ArrayValueChannel() : ProxyMemoryObject("DF5BF7F7-C204-4F6E-BDB8-666A53DFCC58.dll")
{
}
