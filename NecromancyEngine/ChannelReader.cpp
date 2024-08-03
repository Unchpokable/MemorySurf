#include "pch.h"
#include "ChannelReader.h"
#include "ASData.h"
#include "EngineInterfaceProxy.h"
#include "ChannelCaller.hpp"

#include "StaticQuest3DFuncs.h"

Necromancy::ChannelReader::ChannelReader() = default;

Necromancy::ChannelReader::ChannelReader(const std::string & chName) {
    _chGuid = chName;
    _target = EngineInterfaceProxy::getEngine()->GetChannelGroup("C682A43C-22B3-4CDD-A0EA-CF1B3FAE63D5"); // whats wrong with your names dylan
}

AudiosurfData Necromancy::ChannelReader::readData() const
{
    std::vector<ChannelCaller<float>*> channels;

    for (const auto& idx: _targetChannelsIndices)
    {
        auto reader = new ChannelCaller<float>();
        reader->assignChannel(reinterpret_cast<Aco_FloatChannel*>(_target->GetChannel(idx)));
        reader->assignGetFunc(Aco_FloatChannel_GetFloat);
        reader->assignGetDefault(Aco_FloatChannel_GetDefaultFloat);
        reader->assignSet(Aco_FloatChannel_SetFloat);
    }

    return {};
}

void Necromancy::ChannelReader::setTargetIndices(const std::vector<int>& indices) {
    _targetChannelsIndices = indices;
}
