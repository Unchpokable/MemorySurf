#pragma once

#include "hkfunctions.h"
#include "ipcchannel.h"
#include "q3dfloatreader.h"
#include "q3darraytablereader.h"
#include "../NecromancyMessages/messages.h"

namespace Necromancy {

class NecromancyEngine final {
public:
    NecromancyEngine();
    ~NecromancyEngine();

    void dump();
    void send();

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    const Detours::HkFunctions& functions() const noexcept;

private:
    void setupChannelReaders();

    const GUID Aco_ArrayValueGuid { 0xDF5BF7F7, 0xC204, 0x4F6E, { 0xBD, 0xB8, 0x66, 0x6A, 0x53, 0xDF, 0xCC, 0x58 } };

    const char* _scoreChannelName { "Points" };
    const char* _statsChannelName { "Stats Table" };

    std::unordered_map<std::string, Memory::Q3DChannelReader*> _floatChannels;
    Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>* _statsTable;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    Messages::ASDump::ASDumpStruct _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};

};
