#include "pch.h"
#include "engine.h"

using namespace Necromancy;

NecromancyEngine::NecromancyEngine() {
    _q3dFunctions = Detours::HkFunctions::setup();

    setupChannelReaders();
}

void NecromancyEngine::dump() {
    auto score = dynamic_cast<Memory::Q3DFloatReader*>(_floatChannels.at(_scoreChannelName));

    _dumped.set_score(score->get());

    std::vector<float> statsItems = _statsTable->getElementsTyped();

    for(auto i { 0 }; i < statsItems.size(); i++) {
        _dumped.set_array_data(i, statsItems[i]);
    }
}

void NecromancyEngine::send() {
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
    auto statsGroup = _q3dEngineInterface->GetChannelGroup("PlayerStats");

    auto stats = statsGroup->GetChannel(_statsChannelName);
    _statsTable = new Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>(stats);

    auto points = statsGroup->GetChannel(_scoreChannelName);
    _floatChannels.insert_or_assign(_scoreChannelName, new Memory::Q3DFloatReader(points));
}
