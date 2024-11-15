#include "pch.h"
#include "q3darraytablereader.h"
#include "q3dprimitivereader.h"
#include "vftableutils.hpp"

using namespace Necromancy::Memory;

template<typename InternalReader>
Q3DArrayTableReader<InternalReader>::Q3DArrayTableReader(A3d_Channel* target) : Q3DChannelReader(target) {
    // empty
}

template<typename InternalReader>
int Q3DArrayTableReader<InternalReader>::getElementsCount() const {
    auto table = getArrayTable();

    return CallVTable<int, ArrayTable_GetElementCount>(table, ArrayTable_GetElementsVftableOffset, table);
}

template<typename InternalReader>
std::vector<void*> Q3DArrayTableReader<InternalReader>::getElements() const {
    std::vector<void*> data {};

    auto elementsCount = getElementsCount();
    data.resize(elementsCount);

    auto table = getArrayTable();

    for(int i { 0 }; i < elementsCount; i++) {
        data.push_back(CallVTable<void*, ArrayTable_GetElementAtIndex>(table, ArrayTable_GetElementAtVftableOffset, table, i));
    }

    return data;
}

template<typename InternalReader>
std::vector<typename InternalReader::DataType> Q3DArrayTableReader<InternalReader>::getElementsTyped() const {
    std::vector<typename InternalReader::DataType> result;

    for(const std::pair<int, InternalReader*> channelReader : _readers) {
        auto reader = dynamic_cast<Q3DPrimitiveReader<typename InternalReader::DataType>*>(channelReader.second);
        result.push_back(reader->get());
    }

    return result;
}

template<typename InternalReader>
void* Q3DArrayTableReader<InternalReader>::getArrayTable() const {
    return _functions.get<Aco_ArrayTable_GetTable>()(_target);
}

template<typename InternalReader>
void Q3DArrayTableReader<InternalReader>::setupReaders() {
    auto arrayTable = getArrayTable();

    auto itemsCount = getElementsCount();

    for(auto itemIdx : itemsCount) {
        auto channel = CallVTable<void*, ArrayTable_GetElementAtIndex>(arrayTable, ArrayTable_GetElementAtVftableOffset, arrayTable, itemIdx);
        _readers.insert_or_assign(itemIdx, new InternalReader(reinterpret_cast<A3d_Channel*>(channel)));
    }
}
