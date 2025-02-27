#include "pch.h"

#include "q3darraytablereader.h"

#include "acofloatchannel.h"
#include "arrayvaluechannel.h"

using namespace necromancy::memory;
using namespace necromancy::hooks;

Q3DArrayTableReader::Q3DArrayTableReader(const std::unordered_map<int, IndexedArrayValue> &arrays) : Q3DChannelReader(nullptr) {
    _boundTableChannels = arrays;
}

void Q3DArrayTableReader::addIndexedChannel(int row, A3d_Channel* arrayValue, A3d_Channel* indexer) {
    _boundTableChannels[row] = IndexedArrayValue { arrayValue, indexer };
}

float Q3DArrayTableReader::getValue(int row, float index) {
    auto indexer = _boundTableChannels[row].indexer;
    auto arrayValue = _boundTableChannels[row].arrayValue;

    float oldIndex = AcoFloatChannel::getFloat()(indexer);

    AcoFloatChannel::setFloat()(indexer, index);

    float value = ArrayValueChannel::getFloat()(arrayValue);

    AcoFloatChannel::setFloat()(indexer, oldIndex);

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
