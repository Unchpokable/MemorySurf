#pragma once

#include "ChannelCaller.hpp"

class ArrayTableReader {
public:
    explicit ArrayTableReader(A3d_Channel* channel);

    std::vector<float> readData() const;

private:
    ChannelCaller<void*>* _caller;
};

