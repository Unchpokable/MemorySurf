#pragma once

#include "ChannelCaller.hpp"

class ArrayTableReader {
public:
    explicit ArrayTableReader(A3d_Channel* channel);

    float readAt(int i, int j) const;

private:
    ChannelCaller<void*>* _caller;
};

