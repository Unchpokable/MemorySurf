#pragma once

#include "proxymemoryobject.h"

namespace necromancy::hooks {

class AcoFloatChannel : public ProxyMemoryObject {
public:
    static void setup();
    static float getFloat(void* object);
    static float getDefaultFloat(void* object);
    static void setFloat(void* object, float value);

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

