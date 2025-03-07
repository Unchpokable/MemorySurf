#include "pch.h"

#include "acofloatchannel.h"
#include "q3dfloatwrapper.h"

using namespace necromancy::memory;
using namespace necromancy::hooks;

Q3DFloatWrapper::Q3DFloatWrapper(A3d_Channel* target)
    : Q3DChannelWrapper(target),
    _setFloatImpl(AcoFloatChannel::setFloat()),
    _getFloatImpl(AcoFloatChannel::getFloat()),
    _getDefaultFloatImpl(AcoFloatChannel::getDefaultFloat())
{
}

float Q3DFloatWrapper::get() const
{
    return _getFloatImpl(_target);
}

float Q3DFloatWrapper::getDefault() const
{
    return _getDefaultFloatImpl(_target);
}

void Q3DFloatWrapper::set(float value) const
{
   _setFloatImpl(_target, value);
}
