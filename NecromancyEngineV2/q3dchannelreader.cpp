#include "pch.h"
#include "q3dchannelreader.h"

using namespace Necromancy::Memory;

Q3DChannelReader::Q3DChannelReader(A3d_Channel* target) : _functions(HkFunctions::setup()), _target(target) {
    // empty
}
