#pragma once

namespace necromancy::memory {

class Q3DChannelReader
{
public:
    Q3DChannelReader(A3d_Channel* target);
    virtual ~Q3DChannelReader() = default;

    Q3DChannelReader(const Q3DChannelReader& other) noexcept;
    Q3DChannelReader(Q3DChannelReader&& other) noexcept;

    Q3DChannelReader& operator=(const Q3DChannelReader& other) noexcept;
    Q3DChannelReader& operator=(Q3DChannelReader&& other) noexcept;

protected:
    A3d_Channel* _target;
};
}
