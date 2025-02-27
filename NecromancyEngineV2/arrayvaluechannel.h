#pragma once

#include "proxymemoryobject.h"
#include "virtualfunction.h"
#include "typedefs.h"

namespace necromancy::hooks {
class ArrayValueChannel : public ProxyMemoryObject
{
public:
    STATIC_DYNAMIC_CALL

    static void init();
    static VirtualFunction<typedefs::ArrayValue_GetFloat> getFloat();

    static bool allValid();

protected:
    explicit ArrayValueChannel();

private:
    static ArrayValueChannel* _instance;

    static std::size_t _getFloatId;
    static std::size_t _setFloatId;
};
};
