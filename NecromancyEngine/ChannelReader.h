#pragma once

#include <string>
#include <vector>

struct AudiosurfData;

namespace Necromancy
{
    class ChannelReader;
}

class Necromancy::ChannelReader {
public:
    ChannelReader();
    explicit ChannelReader(const std::string& chName);

    AudiosurfData readData();
private:
    std::string _chGuid;
    A3d_ChannelGroup* _targetGroup;

    std::vector<int> _targetChannelsIndices; // indices of Aco_FloatChannel that stores a needed data
};

