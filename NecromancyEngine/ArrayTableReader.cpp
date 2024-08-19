#include "pch.h"
#include "ArrayTableReader.h"
#include "DefaultCallers.hpp"
#include "StaticQuest3DFuncs.h"
#include "vtablehelpers.hpp"

using ARRAY_TABLE_NEW = void;

// functions

using ArrayTable_GetElementCount = int(__thiscall*)(void*);
using ArrayTable_GetElementAtIndex = void*(__thiscall*)(void*, int);

ArrayTableReader::ArrayTableReader(A3d_Channel* channel) : _caller(ChannelCaller<void*>::DefaultCaller::create(channel)) {
    // empty
}

std::vector<float> ArrayTableReader::readData() const
{
    ARRAY_TABLE_NEW* arrayTable = _caller->get();

    // LETS DO SOME BYTE MAGIC
    //int elementsCount = (*(int(__thiscall **)(void*))(*((void**)arrayTable) + 8))(arrayTable);
    auto elementsCount = CallVTable<int, ArrayTable_GetElementCount>(arrayTable, 8, arrayTable);

    std::vector<float> result;
    result.reserve(elementsCount);

    for (auto i { 0 }; i < elementsCount; i++)
    {
        //void* dataAt = (void*)(*(int(__thiscall **)(void*, int))(*((void**)arrayTable) + 4))(arrayTable, i);
        auto dataAt = CallVTable<void*, ArrayTable_GetElementAtIndex>(arrayTable, 4, arrayTable, i);
        result.emplace_back(Aco_FloatChannel_GetFloat(dataAt));
    }

    return result;
}
