#pragma once

#include "proxymemoryobject.h"

namespace necromancy::hooks {

class ArrayValueChannel : public ProxyMemoryObject {
public:
    STATIC_DYNAMIC_CALL

    static void init();
    static float getFloat(void* object);

    static bool allValid();

protected:
    explicit ArrayValueChannel();

private:
    static ArrayValueChannel* _instance;

    static std::size_t _getFloatId;
    static std::size_t _setFloatId;
};

};
