#pragma once

class A3d_Channel;

namespace necromancy::memory {

class Q3DChannelWrapper
{
public:
    Q3DChannelWrapper(A3d_Channel* target);
    virtual ~Q3DChannelWrapper() = default;

    Q3DChannelWrapper(const Q3DChannelWrapper& other) noexcept;
    Q3DChannelWrapper(Q3DChannelWrapper&& other) noexcept;

    Q3DChannelWrapper& operator=(const Q3DChannelWrapper& other) noexcept;
    Q3DChannelWrapper& operator=(Q3DChannelWrapper&& other) noexcept;

protected:
    A3d_Channel* _target;
};
}
