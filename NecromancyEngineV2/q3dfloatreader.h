#pragma once

#include "q3dchannelreader.h"

namespace Necromancy {
namespace Memory {

class Q3DFloatReader final : public Q3DChannelReader
{
public:
    Q3DFloatReader(A3d_Channel* target);

    float get() const;
    float getDefault() const;

    void set(float value) const;
};

}
}
