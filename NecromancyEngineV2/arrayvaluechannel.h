#pragma once

#include "proxymemoryobject.h"
#include "signatures.h"
#include "virtualfunction.h"

namespace necromancy::hooks {
class ArrayValueChannel : public ProxyMemoryObject
{
public:
    STATIC_DYNAMIC_CALL

    static void init();
    static VirtualFunction<signatures::ArrayValue_GetFloat> getFloat();

    static bool allValid();

protected:
    explicit ArrayValueChannel();

private:
    static ArrayValueChannel* _instance;

    static std::size_t _getFloatId;
    static std::size_t _setFloatId;
};
};
