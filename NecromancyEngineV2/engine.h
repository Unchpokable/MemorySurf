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

    constexpr const char _scoreChannelName[] { "score" };
    constexpr const char _statsChannelName[] { "stats" };

    std::unordered_map<std::string, Memory::Q3DChannelReader*> _channels;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    ASScanData _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};

};
