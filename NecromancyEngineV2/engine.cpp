#include "pch.h"
#include "engine.h"

NecromancyEngine::NecromancyEngine() {
    _q3dFunctions = Necromancy::Detours::HkFunctions::setup();

    setupChannelReaders();
}

void NecromancyEngine::dump() {
    // todo: read data from channels and load it into _dumped
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

const Necromancy::Detours::HkFunctions& NecromancyEngine::functions() const noexcept {
    return _q3dFunctions;
}

void NecromancyEngine::setupChannelReaders() {
}
