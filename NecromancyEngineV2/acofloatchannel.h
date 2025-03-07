#pragma once

#include "proxymemoryobject.h"
#include "signatures.h"
#include "virtualfunction.h"

namespace necromancy::hooks {
using namespace necromancy;

class AcoFloatChannel : public ProxyMemoryObject
{
public:
    STATIC_DYNAMIC_CALL

    static void init();
    static VirtualFunction<signatures::Aco_FloatChannel_GetFloat> getFloat();
    static VirtualFunction<signatures::Aco_FloatChannel_GetDefaultFloat> getDefaultFloat();
    static VirtualFunction<signatures::Aco_FloatChannel_SetFloat> setFloat();

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
