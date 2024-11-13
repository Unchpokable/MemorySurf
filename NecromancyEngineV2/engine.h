#pragma once

#include "hkfunctions.h"
#include "ipcchannel.h"
#include "q3dfloatreader.h"
#include "q3darraytablereader.h"
#include "asdata.pb.h"

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
    Necromancy::Ipc::IpcChannel _ipcChannel;
    Necromancy::Detours::HkFunctions _q3dFunctions;

    ASScanData _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};
