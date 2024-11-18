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
    void send() const;

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    const Detours::HkFunctions& functions() const noexcept;

private:
    void setupChannelReaders();

    const char* _largestMatchChannelName { "LargestMatch" };
    const char* _timerChannelName { "Timer" };
    const char* _maxChainLengthChannelName { "MaxChainLength" };
    const char* _currentChainStrength { "CurrentChainStrength" };
    const char* _redsPassedChannelName { "RedsPassed" };
    const char* _yellowsPassedChannelName { "YellowsPassed" };
    const char* _scoreChannelName { "Points" };
    const char* _statsChannelName { "Stats Table" };

    std::unordered_map<std::string, Memory::Q3DFloatReader*> _floatChannels;
    Memory::Q3DArrayTableReader<Memory::Q3DFloatReader>* _statsTable;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    Messages::ASDump::ASDumpStruct _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};

};
