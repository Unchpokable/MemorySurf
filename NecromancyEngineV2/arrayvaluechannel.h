#pragma once

#include "proxymemoryobject.h"

namespace necromancy::hooks {

class ArrayValueChannel : public ProxyMemoryObject {
public:
    static void init();
    static float getFloat(void* object);

protected:
    explicit ArrayValueChannel();

private:
    static ArrayValueChannel* _instance;

    static std::size_t _getFloatId;
    static std::size_t _setFloatId;
};

};
