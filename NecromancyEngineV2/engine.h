#pragma once

#include "ipcchannel.h"
#include "q3darraytablereader.h"
#include "q3dfloatreader.h"
#include "NecromancyMessages/messages.h"

namespace necromancy {
class NecromancyEngine final
{
    enum StatsChannels : int
    {
        StatsTotalTraffic           = 3,
        Index_StatsTotalTraffic     = 4,
        StatsCollectedTraffic       = 8,
        Index_StatsCollectedTraffic = 16
    };

    enum BlockIndex : int
    {
        Purple = 0,
        Blue   = 1,
        Green  = 2,
        Yellow = 3,
        Red    = 4,
        // specials
        White = 7,
    };

public:
    NecromancyEngine();
    ~NecromancyEngine();

    void dump();
    void send();

    const EngineInterface* engineInterface() const noexcept;
    void setQ3DEngineInterface(EngineInterface* q3dInterface);

    void setupChannelReaders();

private:
    inline static const char* _largestMatchChannelName { "LargestMatch" };
    inline static const char* _timerChannelName { "Timer" };
    inline static const char* _scoreChannelName { "Points" };
    inline static const char* _statsChannelName { "Stats Table" };

    // generics

    static A3d_Channel* findChannelNamed(const std::string& name, A3d_ChannelGroup* group);

    static std::unordered_map<StatsChannels, const char*> _statsTableExternalChannels;
    static std::vector<float> _allIndices;

    int getStatsCollectorIndex() const noexcept;

    std::unordered_map<std::string, memory::Q3DFloatReader*> _floatChannels;
    memory::Q3DArrayTableReader* _statsTable;
    ipc::IpcChannel _ipcChannel;

    messages::ASDump::ASDumpStruct _dumped;
    EngineInterface* _q3dEngineInterface = nullptr;
};
};
