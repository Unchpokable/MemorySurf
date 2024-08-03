#pragma once

#include <string>
#include <unordered_map>
#include <vector>

template<typename T>
class ChannelCaller;

struct AudiosurfData;

namespace Necromancy
{
    class ChannelReader;
}

class Necromancy::ChannelReader {
public:
    ChannelReader();
    explicit ChannelReader(const std::string& chName);

    AudiosurfData readData() const;

    void setTargetIndices(const std::vector<int>& indices);
private:
    A3d_ChannelGroup* _target;
    std::string _chGuid;
    std::unordered_map<int> _targetChannelsIndices; // indices of Aco_FloatChannel that stores a needed data
};

