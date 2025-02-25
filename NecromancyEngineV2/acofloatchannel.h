#pragma once

#include "proxymemoryobject.h"

namespace necromancy::hooks {

class AcoFloatChannel : public ProxyMemoryObject {
public:
    STATIC_DYNAMIC_CALL;

    static void init();
    static float getFloat(void* object);
    static float getDefaultFloat(void* object);
    static void setFloat(void* object, float value);

    static bool allValid();

protected:
    virtual ~AcoFloatChannel() override = default;
    explicit AcoFloatChannel();

private:
    static AcoFloatChannel* _instance;

    static std::size_t _getFloatId;
    static std::size_t _getDefaultFloatId;
    static std::size_t _setFloatId;
};

}

