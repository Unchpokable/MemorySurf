#pragma once

#include "q3dchannelreader.h"

namespace necromancy::memory {
class Q3DFloatReader final : public Q3DChannelReader
{
public:
    explicit Q3DFloatReader(A3d_Channel* target);
    virtual ~Q3DFloatReader() override = default;

    float get() const;
    float getDefault() const;

    void set(float value) const;
};
}
