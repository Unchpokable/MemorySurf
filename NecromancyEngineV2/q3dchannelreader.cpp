#include "pch.h"

#include "q3dchannelwrapper.h"

using namespace necromancy::memory;

Q3DChannelWrapper::Q3DChannelWrapper(A3d_Channel* target) : _target(target)
{
    // empty
}

Q3DChannelWrapper::Q3DChannelWrapper(const Q3DChannelWrapper& other) noexcept
{
    *this = other;
}

Q3DChannelWrapper::Q3DChannelWrapper(Q3DChannelWrapper&& other) noexcept
{
    *this = other;
}

Q3DChannelWrapper& Q3DChannelWrapper::operator=(const Q3DChannelWrapper& other) noexcept
{
    if(this == &other) {
        return *this;
    }

    _target = other._target;

    return *this;
}

Q3DChannelWrapper& Q3DChannelWrapper::operator=(Q3DChannelWrapper&& other) noexcept
{
    _target = other._target;

    return *this;
}
