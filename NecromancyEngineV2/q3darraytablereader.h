#pragma once

#include "inplacevector.h"
#include "q3dchannelwrapper.h"

#include "NecromancyMessages/constants.h"

namespace necromancy::memory {
/* Idea of this class now is:
- All columns or at least most interesting columns of ArrayTable placed in separate channels as `Array Value` and after all this `Array Value` exists channel called `Index_ofSomething` like
Stats: CollectedColorCounts <--- this is a channel number 8 in StatsCollector.cgr
this is an `Array Value` channel which (according to IDA) have a single valuable function `GetFloat(void)`. As it has a `void` parameter, looks like indexing of this array controlled by other one channel
AND
then we have:
Index_CollectedColorCounts as a float channel which value changes every time when a player collects a block on the road

for now looks like there is a following indices:
0 - purple count
1 - blue count
2 - green count
3 - yellow count
4 - red count
7 - (why lol???) white count

So idea is:
bind a pair of array value channel and its indexer with some internal for `NecromancyEngine` Identifier and getting a needed value through changing an indexer value and calling `GetFloat` of array value bound with indexer

MUST keep this order:

1 - store current index value of indexer channel
2 - change indexer channel value to that we need
3 - get float for new index
4 - reset index to initial to keep game functionality
*/

class Q3DArrayTableReader final : public Q3DChannelWrapper
{
public:
    struct IndexedArrayValue
    {
        A3d_Channel* arrayValue;
        A3d_Channel* indexer;
    };

    Q3DArrayTableReader(const std::unordered_map<int, IndexedArrayValue>& arrays = {});
    virtual ~Q3DArrayTableReader() override = default;

    void addIndexedChannel(int row, A3d_Channel* arrayValue, A3d_Channel* indexer);
    float getValue(int row, float index); // lol Quest3D using a float for indexing LMFAO 

    InPlaceVector<float> getValues(int row, const std::vector<float> &indexRange);

private:
    std::unordered_map<int, IndexedArrayValue> _boundTableChannels;
};
}
