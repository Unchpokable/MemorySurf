#include "pch.h"
#include "ChannelReader.h"
#include "ASData.h"
#include "EngineInterfaceProxy.h"
#include "ChannelCaller.hpp"
#include "DefaultCallers.hpp"
#include "StaticQuest3DFuncs.h"
#include "protobuf/asdata.pb.h"

Necromancy::ChannelReader::ChannelReader() = default;

Necromancy::ChannelReader::ChannelReader(const std::string& chName) {
    _chGuid = chName;
    _target = EngineInterfaceProxy::getEngine()->GetChannelGroup("StatsCollector.cgr");


}

ASScanData Necromancy::ChannelReader::readData() const
{
    return {};
}

