#include "pch.h"

#include "acofloatchannel.h"

using namespace necromancy::hooks;

void AcoFloatChannel::init() {
    delete _instance;
    _instance = new AcoFloatChannel();

    _getFloatId = _instance->add("?GetFloat@Aco_FloatChannel@@UAEMXZ");
    _getDefaultFloatId = _instance->add("?GetDefaultFloat@Aco_FloatChannel@@UAEMXZ");
    _setFloatId = _instance->add("?SetFloat@Aco_FloatChannel@@UAEXM@Z");
}

float AcoFloatChannel::getFloat(void* object) {
    return _instance->_functions[_getFloatId](object);
}

float AcoFloatChannel::getDefaultFloat(void* object) {
    return _instance->_functions[_getDefaultFloatId](object);
}

void AcoFloatChannel::setFloat(void* object, float value) {
    _instance->_functions[_setFloatId](object, value);
}

AcoFloatChannel::AcoFloatChannel() : ProxyMemoryObject("BE69CCC4-CFC1-4362-AC81-767D199BBFC3.dll") {
}
