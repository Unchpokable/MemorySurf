#include "pch.h"

#include "acofloatchannel.h"

using namespace necromancy::hooks;
using namespace necromancy::typedefs;

AcoFloatChannel* AcoFloatChannel::_instance = nullptr;
std::size_t AcoFloatChannel::_getFloatId = -1;
std::size_t AcoFloatChannel::_getDefaultFloatId = -1;
std::size_t AcoFloatChannel::_setFloatId = -1;

void AcoFloatChannel::init()
{
    delete _instance;
    _instance = new AcoFloatChannel();

    _getFloatId = _instance->add("?GetFloat@Aco_FloatChannel@@UAEMXZ");
    _getDefaultFloatId = _instance->add("?GetDefaultFloat@Aco_FloatChannel@@UAEMXZ");
    _setFloatId = _instance->add("?SetFloat@Aco_FloatChannel@@UAEXM@Z");
}

VirtualFunction<Aco_FloatChannel_GetFloat> AcoFloatChannel::getFloat()
{
    return _instance->_functions[_getFloatId];
}

VirtualFunction<Aco_FloatChannel_GetDefaultFloat> AcoFloatChannel::getDefaultFloat()
{
    return _instance->_functions[_getDefaultFloatId];
}

VirtualFunction<Aco_FloatChannel_SetFloat> AcoFloatChannel::setFloat()
{
    return _instance->_functions[_setFloatId];
}

bool AcoFloatChannel::allValid()
{
    return ProxyMemoryObject::allValid(_instance);
}

AcoFloatChannel::AcoFloatChannel() : ProxyMemoryObject("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll")
{
}
