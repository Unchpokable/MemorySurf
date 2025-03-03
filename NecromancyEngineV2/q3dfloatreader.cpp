#include "pch.h"

#include "q3dfloatreader.h"
#include "acofloatchannel.h"

using namespace necromancy::memory;
using namespace necromancy::hooks;

Q3DFloatReader::Q3DFloatReader(A3d_Channel* target)
    : Q3DChannelReader(target),
    _setFloatImpl(AcoFloatChannel::setFloat()),
    _getFloatImpl(AcoFloatChannel::getFloat()),
    _getDefaultFloatImpl(AcoFloatChannel::getDefaultFloat())
{
}

float Q3DFloatReader::get() const
{
    return _getFloatImpl(_target);
}

float Q3DFloatReader::getDefault() const
{
    return _getDefaultFloatImpl(_target);
}

void Q3DFloatReader::set(float value) const
{
   _setFloatImpl(_target, value);
}
