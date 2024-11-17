#pragma once

#include "hkfunctions.h"
#include "ipcchannel.h"
#include "q3dfloatreader.h"
#include "q3darraytablereader.h"

namespace Necromancy {

class NecromancyEngine final {
public:
    NecromancyEngine();

    void dump();
    void send();

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    const Detours::HkFunctions& functions() const noexcept;

private:
    void setupChannelReaders();

    const char* _scoreChannelName { "Points" };
    const char* _statsChannelName { "Stats Table" };

    std::unordered_map<std::string, Memory::Q3DChannelReader*> _floatChannels;
    Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>* _statsTable;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    EngineInterface* _q3dEngineInterface = nullptr;
};

};
