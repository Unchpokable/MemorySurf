#pragma once

#include "q3dchannelwrapper.h"
#include "signatures.h"
#include "virtualfunction.h"

namespace necromancy::memory {

class Q3DFloatWrapper final : public Q3DChannelWrapper
{
public:
    explicit Q3DFloatWrapper(A3d_Channel* target);
    virtual ~Q3DFloatWrapper() override = default;

    float get() const;
    float getDefault() const;

    void set(float value) const;

private:
    VirtualFunction<signatures::Aco_FloatChannel_SetFloat> _setFloatImpl;
    VirtualFunction<signatures::Aco_FloatChannel_GetFloat> _getFloatImpl;
    VirtualFunction<signatures::Aco_FloatChannel_GetDefaultFloat> _getDefaultFloatImpl;
};

}
