#pragma once

#include "proxymemoryobject.h"
#include "typedefs.h"
#include "virtualfunction.h"

namespace necromancy::hooks {

using namespace necromancy;

class AcoFloatChannel : public ProxyMemoryObject {
public:
    STATIC_DYNAMIC_CALL

    static void init();
    static VirtualFunction<typedefs::Aco_FloatChannel_GetFloat> getFloat();
    static VirtualFunction<typedefs::Aco_FloatChannel_GetDefaultFloat> getDefaultFloat();
    static VirtualFunction<typedefs::Aco_FloatChannel_SetFloat> setFloat();

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

