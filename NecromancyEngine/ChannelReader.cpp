#include "pch.h"
#include "ChannelReader.h"
#include "DefaultCallers.hpp"
#include "EngineInterfaceProxy.h"
#include "ChannelCaller.hpp"
#include "StaticQuest3DFuncs.h"
#include "protobuf/asdata.pb.h"

Necromancy::ChannelReader::ChannelReader() = default;

Necromancy::ChannelReader::ChannelReader(const std::string& chName) {
    _chGuid = chName;
    _target = EngineInterfaceProxy::getEngine()->GetChannelGroup("StatsCollector.cgr");

    _scoreTableReader = new ArrayTableReader(_target->GetChannel(StatsTableChannelName));
    _floatChannels[Channels::Points] = ChannelCaller<float>::DefaultCaller::create(_target->GetChannel(PointsChannelName));
    _floatChannels[Channels::Overfills] = ChannelCaller<float>::DefaultCaller::create(_target->GetChannel(OverfillsChannelName));
}

ASScanData Necromancy::ChannelReader::readData() const
{
    ASScanData data;
    auto score = _floatChannels.at(Channels::Points)->get();
    data.set_score(score);
    auto table = _scoreTableReader->readData();
    for (int i { 0 }; i < table.size(); i++)
    {
        data.set_array_data(i, table[i]);
    }
    data.set_rows(6);
    data.set_cols(6);
    data.set_gold_threshold(45600); // TODO: find this channel
    return data;
}

