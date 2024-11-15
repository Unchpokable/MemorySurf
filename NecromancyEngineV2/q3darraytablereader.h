#pragma once

#include "q3dchannelreader.h"
#include "q3dfloatreader.h"

namespace Necromancy {
namespace Memory {

static constexpr ptrdiff_t ArrayTable_GetElementsVftableOffset = 8;
static constexpr ptrdiff_t ArrayTable_GetElementAtVftableOffset = 4;

template<typename InternalReader = Q3DFloatReader>
class Q3DArrayTableReader final : Q3DChannelReader
{
    using InternalReaderData = typename InternalReader::DataType;
    using TypedVector = std::vector<InternalReaderData>;

public:
    Q3DArrayTableReader(A3d_Channel* target);
    virtual ~Q3DArrayTableReader() override = default;

    int getElementsCount() const;
    std::vector<void*> getElements() const;

    TypedVector getElementsTyped() const;

private:
    void* getArrayTable() const;
    void setupReaders();

    std::unordered_map<int, InternalReader*> _readers;
};

}
}

