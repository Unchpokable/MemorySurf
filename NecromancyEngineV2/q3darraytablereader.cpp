#include "pch.h"

#include "q3darraytablereader.h"

using namespace Necromancy::Memory;

Q3DArrayTableReader::Q3DArrayTableReader(const std::unordered_map<int, IndexedArrayValue> &arrays) : Q3DChannelReader(nullptr) {
    _boundTableChannels = arrays;

    auto functions = HkFunctions::setup();

    _arrayValueGetFloat = functions.get<ArrayValue_GetFloat>("ArrayValue_GetFloat");
    _floatChannelGetFloat = functions.get<Aco_FloatChannel_GetFloat>("Aco_FloatChannel_GetFloat");
    _floatChannelSetFloat = functions.get<Aco_FloatChannel_SetFloat>("Aco_FloatChannel_SetFloat");
}

void Q3DArrayTableReader::addIndexedChannel(int row, A3d_Channel* arrayValue, A3d_Channel* indexer) {
    _boundTableChannels[row] = IndexedArrayValue { arrayValue, indexer };
}

float Q3DArrayTableReader::getValue(int row, float index) {
    auto indexer = _boundTableChannels[row].indexer;
    auto arrayValue = _boundTableChannels[row].arrayValue;

    auto oldIndex = _floatChannelGetFloat(indexer);

    _floatChannelSetFloat(indexer, index);

    auto value = _arrayValueGetFloat(arrayValue);

    _floatChannelSetFloat(indexer, oldIndex);

    return value;
}

std::vector<float> Q3DArrayTableReader::getValues(int row, const std::vector<float> &indexRange) {
    std::vector<float> result;

    result.reserve(indexRange.size());

    for(auto index : indexRange) {
        result.push_back(getValue(row, index));
    }

    return result;
}
