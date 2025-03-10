#include "pch.h"

#include "engine.h"
#include "genericutils.h"
#include "load.h"
#include "logger.h"

#include "acofloatchannel.h"
#include "arrayvaluechannel.h"
#include "corechannels.h"

using namespace necromancy;

static constexpr const char* statsCollectorChannelGroup = "StatCollector";

std::unordered_map<NecromancyEngine::StatsChannels, const char*> NecromancyEngine::_statsTableExternalChannels {
    { StatsTotalTraffic, "Stats: TrafficColorCounts" },
    { Index_StatsTotalTraffic, "Index_TrafficColorCounts" },
    { StatsCollectedTraffic, "Stats: CollectedColorCounts" },
    { Index_StatsCollectedTraffic, "Index_CollectedColorCounts" }
};

std::vector<float> NecromancyEngine::_allIndices {
    Purple, Blue, Green, Yellow, Red, White
};

NecromancyEngine::NecromancyEngine(): _statsTable(nullptr)
{
    Initialize(&_dumped); // todo: place here actual dumped array size

    hooks::CoreChannels::init();
    Logger::logCondition(hooks::CoreChannels::allValid(), "Accessing to core game engine functions");

    hooks::AcoFloatChannel::init();
    Logger::logCondition(hooks::AcoFloatChannel::allValid(), "Accessing to Float Channel functions");

    hooks::ArrayValueChannel::init();
    Logger::logCondition(hooks::ArrayValueChannel::allValid(), "Accessing to Array Value functions");
}

NecromancyEngine::~NecromancyEngine() = default;

void NecromancyEngine::dump()
{
    auto totalTraffic = _statsTable->getValues(StatsTotalTraffic, _allIndices);
    auto collectedTraffic = _statsTable->getValues(StatsCollectedTraffic, _allIndices);

    assert(totalTraffic.used < 32);
    assert(collectedTraffic.used < 32);

    std::memcpy(_dumped.totalColors, totalTraffic.data, totalTraffic.used);
    std::memcpy(_dumped.collectedColors, collectedTraffic.data, collectedTraffic.used);

    _dumped.totalColorsUsed = totalTraffic.used;
    _dumped.collectedColorsUsed = collectedTraffic.used;

    _dumped.score = _floatChannels.at(_scoreChannelName)->get();
    _dumped.largestMatch = _floatChannels.at(_largestMatchChannelName)->get();
    _dumped.timeElapsed = _floatChannels.at(_timerChannelName)->get();
}

void NecromancyEngine::send()
{
    _ipcChannel.writeBuffer(_dumped);
}

const EngineInterface* NecromancyEngine::engineInterface() const noexcept
{
    return _q3dEngineInterface;
}

void NecromancyEngine::setQ3DEngineInterface(EngineInterface* q3dInterface)
{
    _q3dEngineInterface = q3dInterface;
}

void NecromancyEngine::setupChannelReaders()
{
    auto statsGroupIdx = getStatsCollectorIndex();
    auto statsGroup = _q3dEngineInterface->GetChannelGroup(statsGroupIdx);

    Logger::logCondition(notNull(statsGroup), "Stats channel group not null");

    _statsTable = new memory::Q3DArrayTableReader();

    auto totalTrafficChannel = findChannelNamed(_statsTableExternalChannels[StatsTotalTraffic], statsGroup);
    auto indexTotalTrafficChannel = findChannelNamed(_statsTableExternalChannels[Index_StatsTotalTraffic], statsGroup);

    bool totalTrafficOk = totalTrafficChannel && indexTotalTrafficChannel;
    Logger::logCondition(totalTrafficOk, "Track traffic available");
    if(!totalTrafficOk) {
        Unload(NULL);
    }

    _statsTable->addIndexedChannel(StatsTotalTraffic, totalTrafficChannel, indexTotalTrafficChannel);

    auto collectedTrafficChannel = findChannelNamed(_statsTableExternalChannels[StatsCollectedTraffic], statsGroup);
    auto indexCollectedTrafficChannel = findChannelNamed(_statsTableExternalChannels[Index_StatsCollectedTraffic], statsGroup);

    bool collectedTrafficOk = collectedTrafficChannel && indexCollectedTrafficChannel;
    Logger::logCondition(collectedTrafficOk, "Track traffic available");
    if(!collectedTrafficOk) {
        Unload(NULL);
    }

    _statsTable->addIndexedChannel(StatsCollectedTraffic, collectedTrafficChannel, indexCollectedTrafficChannel);

    auto points = findChannelNamed(_scoreChannelName, statsGroup);
    Logger::logCondition(notNull(points), "points table available");
    _floatChannels.insert_or_assign(_scoreChannelName, new memory::Q3DFloatWrapper(points));

    auto largestMatch = findChannelNamed(_largestMatchChannelName, statsGroup);
    Logger::logCondition(notNull(largestMatch), "largest match available");
    _floatChannels.insert_or_assign(_largestMatchChannelName, new memory::Q3DFloatWrapper(largestMatch));

    auto timer = findChannelNamed(_timerChannelName, statsGroup);
    Logger::logCondition(notNull(timer), "timer available");
    _floatChannels.insert_or_assign(_timerChannelName, new memory::Q3DFloatWrapper(timer));

    Logger::forceWrite();
}

A3d_Channel* NecromancyEngine::findChannelNamed(const std::string& name, A3d_ChannelGroup* group)
{
    constexpr auto maxChannel = 10'000;
    auto getChannel = hooks::CoreChannels::getChannel();
    auto getChannelName = hooks::CoreChannels::getChannelName();

    for(std::int32_t i { 0 }; i < maxChannel; i++) {
        auto channel = getChannel(group, i);
        auto channelName = getChannelName(channel);
        if(std::strcmp(channelName, name.c_str()) == 0) {
            return static_cast<A3d_Channel*>(channel);
        }
    }

    return nullptr;
}

int NecromancyEngine::getStatsCollectorIndex() const noexcept
{
    constexpr auto maxChannelGroup = 1024;
    auto getPoolName = hooks::CoreChannels::getPoolName();

    for(auto i { 0 }; i < maxChannelGroup; i++) {
        auto channel = _q3dEngineInterface->GetChannelGroup(i);
        auto poolName = getPoolName(channel);
        if(std::strcmp(poolName, statsCollectorChannelGroup) == 0) {
            return i;
        }
    }

    return -1;
}
