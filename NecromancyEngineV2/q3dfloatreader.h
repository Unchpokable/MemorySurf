#pragma once

#include "q3dprimitivereader.h"
#include "q3dchannelreader.h"

namespace necromancy::Memory {

class Q3DFloatReader final : public Q3DChannelReader, public Q3DPrimitiveReader<float>
{
public:
    explicit Q3DFloatReader(A3d_Channel* target);
    virtual ~Q3DFloatReader() override = default;

    virtual float get() const override;
    float getDefault() const;

    virtual void set(float value) const override;
};

}
