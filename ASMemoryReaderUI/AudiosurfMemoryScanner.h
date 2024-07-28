#pragma once

#include <QObject>
#include <Windows.h>
#include "TaggedException.hpp"

using LogicException = TaggedException<struct LogicExceptionTag>;

class MemoryReader;

namespace Addresses
{
    constexpr std::ptrdiff_t MEM_GM{0};
    constexpr std::ptrdiff_t MEM_BLOCK_RED{0};
    constexpr std::ptrdiff_t MEM_BLOCK_YELLOW{0};
    constexpr std::ptrdiff_t MEM_BLOCK_GREEN{0};
    constexpr std::ptrdiff_t MEM_BLOCK_BLUE{0};
    constexpr std::ptrdiff_t MEM_BLOCK_PURPLE{0};
    constexpr std::ptrdiff_t MEM_SCORE{0}; 

    constexpr char dllName[] = "Currently undefined Dylan's randomly named DLL";
}

namespace Patterns
{
    inline BYTE MEM_PAT_GM[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_BLOCK_RED[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_BLOCK_YELLOW[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_BLOCK_GREEN[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_BLOCK_BLUE[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_BLOCK_PURPLE[] = {0xFF, 0xFF, 0xFF, 0xFF};
    inline BYTE MEM_PAT_SCORE[] = {0xFF, 0xFF, 0xFF, 0xFF};
}

struct AudiosurfData
{
    // looks like audiosurf uses float variables to store EVERYTHING
    float GoldMedalThreshold;
    float BlockStatRed;
    float BlockStatYellow;
    float BlockStatGreen;
    float BlockStatBlue;
    float BlockStatPurple;
    float Score;
};

class AudiosurfMemoryScanner : public QObject
{
    Q_OBJECT

public:
    enum class ReadingMode
    {
        MemoryOffset,
        MemoryPattern
    };

    AudiosurfMemoryScanner(QObject* parent);
    ~AudiosurfMemoryScanner() override;

    void SetReadingModeToMemoryOffsets()
    {
        _selectedMode = ReadingMode::MemoryOffset;
    }

    void setReadingModeToMemoryPatterns()
    {
        _selectedMode = ReadingMode::MemoryPattern;
    }

    AudiosurfData performFullScan();

private:
    AudiosurfData performFullScanMemOffsetImpl();
    AudiosurfData performFullScanMemPatternsImpl();

    ReadingMode _selectedMode;

    MemoryReader* _engine;
};
