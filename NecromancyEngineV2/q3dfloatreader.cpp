#include "pch.h"

#include "q3dfloatreader.h"
#include "acofloatchannel.h"

using namespace necromancy::memory;
using namespace necromancy::hooks;

Q3DFloatReader::Q3DFloatReader(A3d_Channel* target) : Q3DChannelReader(target) {
    // empty
}

float Q3DFloatReader::get() const {
    return AcoFloatChannel::getFloat()(_target);
}

float Q3DFloatReader::getDefault() const {
    return AcoFloatChannel::getDefaultFloat()(_target);
}

void Q3DFloatReader::set(float value) const {
    AcoFloatChannel::setFloat()(_target, value);
}
