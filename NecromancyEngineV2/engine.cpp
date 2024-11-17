#include "pch.h"
#include "engine.h"

using namespace Necromancy;

NecromancyEngine::NecromancyEngine() {
    _q3dFunctions = Detours::HkFunctions::setup();

    setupChannelReaders();
}

void NecromancyEngine::dump() {
}

void NecromancyEngine::send() {
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
    auto statsGroup = _q3dEngineInterface->GetChannelGroup("StatsCollector");

    auto stats = statsGroup->GetChannel(_statsChannelName);
    _statsTable = new Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>(stats);

    auto points = statsGroup->GetChannel(_scoreChannelName);
    _floatChannels.insert_or_assign(_scoreChannelName, new Memory::Q3DFloatReader(points));
}
