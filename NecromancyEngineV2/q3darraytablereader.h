#pragma once

#include "q3dchannelreader.h"

namespace Necromancy {
namespace Memory {

static constexpr ptrdiff_t ArrayTable_GetElementsVftableOffset = 8;
static constexpr ptrdiff_t ArrayTable_GetElementAtVftableOffset = 4;

class Q3DArrayTableReader final : Q3DChannelReader
{
public:
    Q3DArrayTableReader(A3d_Channel* target);

    int getElementsCount() const;
    std::vector<void*> getElements() const;

private:
    void* getArrayTable() const;
};

}
}

