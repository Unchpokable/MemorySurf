#include "pch.h"

#include "engine.h"
#include "logger.h"
#include "genericutils.h"

using namespace Necromancy;

static constexpr const char* statsCollectorChannelGroup = "StatCollector";

NecromancyEngine::NecromancyEngine(): _statsTable(nullptr) {
    _q3dFunctions = Detours::HkFunctions::setup();
    Initialize(&_dumped, 36); // todo: place here actual dumped array size
}

NecromancyEngine::~NecromancyEngine() {
    Free(&_dumped);
}

void NecromancyEngine::dump() {
    auto stats = _statsTable->getElementsTyped();
    std::memcpy(_dumped.statsArray, stats.data(), sizeof(float) * stats.size());

    _dumped.score = _floatChannels.at(_scoreChannelName)->get();
    _dumped.largestMatch = _floatChannels.at(_largestMatchChannelName)->get();
    _dumped.timestamp = _floatChannels.at(_timerChannelName)->get();
}

void NecromancyEngine::send() const {
    _ipcChannel.writeBuffer(_dumped);
}

const EngineInterface* NecromancyEngine::engineInterface() const noexcept {
    return _q3dEngineInterface;
}

void NecromancyEngine::setQ3DEngineInterface(EngineInterface* q3dInterface) {
    _q3dEngineInterface = q3dInterface;
}

const Detours::HkFunctions& NecromancyEngine::functions() const noexcept {
    return _q3dFunctions;
}

void NecromancyEngine::setupChannelReaders() {
    auto statsGroupIdx = getStatsCollectorIndex();
    auto statsGroup = _q3dEngineInterface->GetChannelGroup(statsGroupIdx);

    Logger::logCondition(notNull(statsGroup), "Stats ChannelGroup not null");

    auto stats = findChannelNamed(_statsChannelName, statsGroup);
    Logger::logCondition(notNull(stats), "Stats table available");
    _statsTable = new Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>(stats);


    auto points = findChannelNamed(_scoreChannelName, statsGroup);
    Logger::logCondition(notNull(points), "points table available");
    _floatChannels.insert_or_assign(_scoreChannelName, new Memory::Q3DFloatReader(points));

    auto largestMatch = findChannelNamed(_largestMatchChannelName, statsGroup);
    Logger::logCondition(notNull(largestMatch), "largest match available");
    _floatChannels.insert_or_assign(_largestMatchChannelName, new Memory::Q3DFloatReader(largestMatch));

    auto timer = findChannelNamed(_timerChannelName, statsGroup);
    Logger::logCondition(notNull(timer), "timer available");
    _floatChannels.insert_or_assign(_timerChannelName, new Memory::Q3DFloatReader(timer));

    Logger::forceWrite();
}

int NecromancyEngine::getStatsCollectorIndex() const noexcept {
    constexpr auto maxChannelGroup = 1024;

    auto getPoolNameFunc = _q3dFunctions.get<Typedefs::ChannelGroup_GetPoolName>("ChannelGroup_GetPoolName");

    for(auto i { 0 }; i < maxChannelGroup; i++) {
        auto channel = _q3dEngineInterface->GetChannelGroup(i);
        auto poolName = getPoolNameFunc(channel);
        if(std::strcmp(poolName, statsCollectorChannelGroup) == 0) {
            return i;
        }
    }

    return -1;
}

A3d_Channel* NecromancyEngine::findChannelNamed(const std::string& name, A3d_ChannelGroup* group) const {
    constexpr auto maxChannel = 10'000;

    auto getChannelByIndex = _q3dFunctions.get<Typedefs::ChannelGroup_GetChannel>("ChannelGroup_GetChannel");
    auto getChannelName = _q3dFunctions.get<Typedefs::Channel_GetChannelName>("Channel_GetChannelName");

    for(auto i { 0 }; i < maxChannel; i++) {
        auto channel = getChannelByIndex(group, i);
        auto channelName = getChannelName(channel);
        if(std::strcmp(channelName, name.c_str()) == 0) {
            return channel;
        }
    }

    return nullptr;
}
