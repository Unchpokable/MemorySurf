#pragma once

#include "q3dchannelreader.h"
#include "signatures.h"
#include "virtualfunction.h"

namespace necromancy::memory {

class Q3DFloatReader final : public Q3DChannelReader
{
public:
    explicit Q3DFloatReader(A3d_Channel* target);
    virtual ~Q3DFloatReader() override = default;

    float get() const;
    float getDefault() const;

    void set(float value) const;

private:
    VirtualFunction<signatures::Aco_FloatChannel_SetFloat> _setFloatImpl;
    VirtualFunction<signatures::Aco_FloatChannel_GetFloat> _getFloatImpl;
    VirtualFunction<signatures::Aco_FloatChannel_GetDefaultFloat> _getDefaultFloatImpl;
};

}
