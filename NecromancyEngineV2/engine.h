#pragma once

#include "hook.h"
#include "hkfunctions.h"
#include "q3dfloatreader.h"
#include "q3darraytablereader.h"
#include "q3ddetours.h"

class NecromancyEngine final
{
public:
    NecromancyEngine();

    void dump();
    void send();

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    const Necromancy::Detours::HkFunctions& functions() const noexcept;

private:
    void setupChannelReaders();

    std::unordered_map<std::string, Necromancy::Memory::Q3DChannelReader> _channels;
    // TODO: IPC handles

    Necromancy::Detours::HkFunctions _q3dFunctions;
    EngineInterface* _q3dEngineInterface = nullptr;
};
