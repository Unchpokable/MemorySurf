#pragma once

#include "hkfunctions.h"

namespace Necromancy {
namespace Memory {

using namespace Necromancy::Detours;

class Q3DChannelReader {
public:
    Q3DChannelReader(A3d_Channel* target);
    ~Q3DChannelReader() = default;

    Q3DChannelReader(const Q3DChannelReader& other) noexcept = default;
    Q3DChannelReader(Q3DChannelReader&& other) noexcept = default;

    Q3DChannelReader& operator=(const Q3DChannelReader& other) noexcept = default;
    Q3DChannelReader& operator=(Q3DChannelReader&& other) noexcept = default;

protected:
    HkFunctions _functions;
    A3d_Channel* _target;
};

}
}


