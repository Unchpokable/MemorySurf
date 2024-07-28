#include "pch.h"
#include "ChannelReader.h"
#include "ASData.h"
#include "EngineInterfaceProxy.h"

Necromancy::ChannelReader::ChannelReader() = default;

Necromancy::ChannelReader::ChannelReader(const std::string & chName) {
    _chGuid = chName;
    _targetGroup = EngineInterfaceProxy::getEngine()->GetChannelGroup("C682A43C-22B3-4CDD-A0EA-CF1B3FAE63D5"); // whats wrong with your names dylan
}

AudiosurfData Necromancy::ChannelReader::readData() const
{
    std::vector<Aco_FloatChannel*> channels;

    for (const auto& idx: _targetChannelsIndices)
    {
        channels.push_back(reinterpret_cast<Aco_FloatChannel*>(_targetGroup->GetChannel(idx)));
    }

    return {};
}
