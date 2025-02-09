#pragma once

#include "hkfunctions.h"
#include "ipcchannel.h"
#include "q3darraytablereader.h"
#include "q3dfloatreader.h"
#include "NecromancyMessages/messages.h"

namespace Necromancy {

class NecromancyEngine final {
private:
    enum StatsChannels : int {
        StatsTotalTraffic = 3,
        Index_StatsTotalTraffic = 4,
        StatsCollectedTraffic = 8,
        Index_StatsCollectedTraffic = 16
    };

    enum BlockIndex : int {
        Purple = 0,
        Blue = 1,
        Green = 2,
        Yellow = 3,
        Red = 4,
        // specials
        White = 7,
    };

public:
    NecromancyEngine();
    ~NecromancyEngine();

    void dump();
    void send() const;

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    const Detours::HkFunctions& functions() const noexcept;
    void setupChannelReaders();

private:
    inline static const char* _largestMatchChannelName { "LargestMatch" };
    inline static const char* _timerChannelName { "Timer" };
    inline static const char* _scoreChannelName { "Points" };
    inline static const char* _statsChannelName { "Stats Table" };

    static std::unordered_map<StatsChannels, const char*> _statsTableExternalChannels;
    static std::vector<float> _allIndices;

    int getStatsCollectorIndex() const noexcept;
    A3d_Channel* findChannelNamed(const std::string& name, A3d_ChannelGroup* group) const;

    std::unordered_map<std::string, Memory::Q3DFloatReader*> _floatChannels;
    Memory::Q3DArrayTableReader* _statsTable;
    Ipc::IpcChannel _ipcChannel;
    Detours::HkFunctions _q3dFunctions;

    Messages::ASDump::ASDumpStruct _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};

};
