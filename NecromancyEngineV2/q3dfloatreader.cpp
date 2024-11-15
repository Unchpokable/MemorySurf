#include "pch.h"
#include "q3dfloatreader.h"

using namespace Necromancy::Memory;

Q3DFloatReader::Q3DFloatReader(A3d_Channel* target) : Q3DChannelReader(target) {
    // empty
}

float Q3DFloatReader::get() const {
    return _functions.get<Aco_FloatChannel_GetFloat>()(_target);
}

float Q3DFloatReader::getDefault() const {
    return _functions.get<Aco_FloatChannel_GetDefaultFloat>()(_target);
}

void Q3DFloatReader::set(float value) const {
    _functions.get<Aco_FloatChannel_SetFloat>()(_target, value);
}
