#include "pch.h"

#include "q3dchannelreader.h"

using namespace Necromancy::Memory;

Q3DChannelReader::Q3DChannelReader(A3d_Channel* target) : _functions(HkFunctions::setup()), _target(target) {
    // empty
}

Q3DChannelReader::Q3DChannelReader(const Q3DChannelReader& other) noexcept {
    *this = other;
}

Q3DChannelReader::Q3DChannelReader(Q3DChannelReader&& other) noexcept {
    *this = other;
}

Q3DChannelReader& Q3DChannelReader::operator=(const Q3DChannelReader& other) noexcept {
    _functions = other._functions;
    _target = other._target;

    return *this;
}

Q3DChannelReader& Q3DChannelReader::operator=(Q3DChannelReader&& other) noexcept {
    _functions = other._functions;
    _target = other._target;

    return *this;
}
