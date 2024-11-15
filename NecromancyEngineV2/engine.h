#pragma once

#include "hkfunctions.h"
#include "ipcchannel.h"
#include "q3dfloatreader.h"
#include "q3darraytablereader.h"
#include "asdata.pb.h"

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

    const char _scoreChannelName[6] { "score" };
    const char _statsChannelName[6] { "stats" };

    std::unordered_map<std::string, Memory::Q3DChannelReader*> _floatChannels;
    Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>* _statsTable;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    ASScanData _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};

};
