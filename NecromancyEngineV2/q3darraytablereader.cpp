#include "pch.h"
#include "q3darraytablereader.h"
#include "vftableutils.hpp"

using namespace Necromancy::Memory;

Q3DArrayTableReader::Q3DArrayTableReader(A3d_Channel* target) : Q3DChannelReader(target) {
    // empty
}

int Q3DArrayTableReader::getElementsCount() const {
    auto table = getArrayTable();

    return CallVTable<int, ArrayTable_GetElementCount>(table, ArrayTable_GetElementsVftableOffset, table);
}

std::vector<void*> Q3DArrayTableReader::getElements() const {
    std::vector<void*> data {};

    auto elementsCount = getElementsCount();
    data.resize(elementsCount);

    auto table = getArrayTable();

    for(int i { 0 }; i < elementsCount; i++)
    {
        data.push_back(CallVTable<void*, ArrayTable_GetElementAtIndex>(table, ArrayTable_GetElementAtVftableOffset, table, i));
    }

    return data;
}

void* Q3DArrayTableReader::getArrayTable() const {
    return _functions.get<Aco_ArrayTable_GetTable>()(_target);
}

