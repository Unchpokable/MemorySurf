#include "pch.h"
#include "ArrayTableReader.h"
#include "DefaultCallers.hpp"

using ARRAY_TABLE_NEW = void;

ArrayTableReader::ArrayTableReader(A3d_Channel* channel) : _caller(ChannelCaller<void*>::DefaultCaller::create(channel)) {
    // empty
}

float ArrayTableReader::readAt(int i, int j) const
{
    ARRAY_TABLE_NEW* returned = _caller->get();

    // lets assume if ARRAY_TABLE_NEW contains float** table as first field

    void* row = (returned + i);
    auto val = static_cast<float*>(row + j);
    return *val;
}
