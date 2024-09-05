#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "protobuf/asdata.pb.h"

template<typename T>
class ChannelCaller;

namespace Necromancy
{
    constexpr const char* StatsTableChannelName = "Stats Table";
    constexpr const char* PointsChannelName = "Points";
    constexpr const char* OverfillsChannelName = "Overfills";

    enum class Channels
    {
        StatsTable,
        Points,
        Overfills,

        //
        Count
    };

    class ChannelReader;
}

class Necromancy::ChannelReader {
public:
    ChannelReader();
    explicit ChannelReader(const std::string& chName);

    ASScanData readData() const;

private:
    A3d_ChannelGroup* _target;
    std::string _chGuid;

    std::unordered_map<Channels, const char*> _channelsNames = {
        { Channels::StatsTable, StatsTableChannelName },
        { Channels::Points, PointsChannelName },
        { Channels::Overfills, OverfillsChannelName }
    };

    std::unordered_map<Channels, ChannelCaller<float>> _floatChannels;
    std::unordered_map<Channels, ChannelCaller<void*>> _tableChannels;

    constexpr static std::array<Channels, static_cast<std::size_t>(Channels::Count)> _channels = {
        Channels::StatsTable, Channels::Points, Channels::Overfills
    };
};

