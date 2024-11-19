#include "pch.h"
#include "engine.h"

using namespace Necromancy;

NecromancyEngine::NecromancyEngine() {
    _q3dFunctions = Detours::HkFunctions::setup();
    Initialize(&_dumped, 36); // todo: place here actual dumped array size
    //setupChannelReaders();
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
    auto statsGroup = _q3dEngineInterface->GetChannelGroup("StatCollector");

    auto stats = statsGroup->GetChannel(_statsChannelName);
    _statsTable = new Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>(stats);

    auto points = statsGroup->GetChannel(_scoreChannelName);
    _floatChannels.insert_or_assign(_scoreChannelName, new Memory::Q3DFloatReader(points));

    auto largestMatch = statsGroup->GetChannel(_largestMatchChannelName);
    _floatChannels.insert_or_assign(_largestMatchChannelName, new Memory::Q3DFloatReader(largestMatch));

    auto timer = statsGroup->GetChannel(_timerChannelName);
    _floatChannels.insert_or_assign(_timerChannelName, new Memory::Q3DFloatReader(timer));
}
