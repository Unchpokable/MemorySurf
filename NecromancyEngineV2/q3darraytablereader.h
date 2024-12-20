#pragma once

#include "q3dchannelreader.h"
#include "q3dfloatreader.h"
#include "vftableutils.hpp"

#define VTABLE_INDEX(offset) ((offset) / sizeof(std::uintptr_t))

namespace Necromancy::Memory {

static constexpr ptrdiff_t ArrayTable_GetElementsVftableIdx = VTABLE_INDEX(8);
static constexpr ptrdiff_t ArrayTable_GetElementAtVftableIdx = VTABLE_INDEX(4);

template<ValidPrimitiveReader InternalReader = Q3DFloatReader>
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

template<ValidPrimitiveReader InternalReader>
Q3DArrayTableReader<InternalReader>::Q3DArrayTableReader(A3d_Channel* target) : Q3DChannelReader(target) {
    setupReaders();
}

template<ValidPrimitiveReader InternalReader>
int Q3DArrayTableReader<InternalReader>::getElementsCount() const {
    auto table = getArrayTable();

    return CallVTable<int, ArrayTable_GetElementCount>(table, ArrayTable_GetElementsVftableIdx, table);
}

template<ValidPrimitiveReader InternalReader>
std::vector<void*> Q3DArrayTableReader<InternalReader>::getElements() const {
    std::vector<void*> data {};

    auto elementsCount = getElementsCount();
    data.resize(elementsCount);

    auto table = getArrayTable();

    for(int i { 0 }; i < elementsCount; i++) {
        data.push_back(CallVTable<void*, ArrayTable_GetElementAtIndex>(table, ArrayTable_GetElementAtVftableIdx, table, i));
    }

    return data;
}

template<ValidPrimitiveReader InternalReader>
typename Q3DArrayTableReader<InternalReader>::TypedVector Q3DArrayTableReader<InternalReader>::getElementsTyped() const {
    TypedVector result;

    for(const std::pair<int, InternalReader*>& channelReader : _readers) {
        auto reader = dynamic_cast<Q3DPrimitiveReader<InternalReaderData>*>(channelReader.second);
        result.push_back(reader->get());
    }

    return result;
}

template<ValidPrimitiveReader InternalReader>
void* Q3DArrayTableReader<InternalReader>::getArrayTable() const {
    return _functions.get<Aco_ArrayTable_GetTable>("Aco_ArrayTable_GetTable")(_target);
}

template<ValidPrimitiveReader InternalReader>
void Q3DArrayTableReader<InternalReader>::setupReaders() {
    auto arrayTable = getArrayTable();

    auto itemsCount = getElementsCount();

    for(auto itemIdx { 0 }; itemIdx < itemsCount; itemIdx++) {
        auto channel = CallVTable<void*, ArrayTable_GetElementAtIndex>(arrayTable, ArrayTable_GetElementAtVftableIdx, arrayTable, itemIdx);
        _readers.insert_or_assign(itemIdx, new InternalReader(reinterpret_cast<A3d_Channel*>(channel)));
    }
}

}

