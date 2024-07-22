#pragma once

#include <QObject>
#include <Windows.h>

namespace Addresses
{
    constexpr DWORD MEM_GM { 0 };
    constexpr DWORD MEM_BLOCK_RED { 0 };
    constexpr DWORD MEM_BLOCK_YELLOW { 0 };
    constexpr DWORD MEM_BLOCK_GREEN { 0 };
    constexpr DWORD MEM_BLOCK_BLUE { 0 };
    constexpr DWORD MEM_BLOCK_PURPLE { 0 };
    constexpr DWORD MEM_SCORE { 0 };
}

namespace Patterns
{
    inline BYTE MEM_PAT_GM[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_BLOCK_RED[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_BLOCK_YELLOW[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_BLOCK_GREEN[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_BLOCK_BLUE[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_BLOCK_PURPLE[] = { 0xFF, 0xFF, 0xFF, 0xFF };
    inline BYTE MEM_PAT_SCORE[] = { 0xFF, 0xFF, 0xFF, 0xFF };
}

struct AudiosurfData
{
    DWORD GoldMedalThreshold;
    DWORD BlockStatRed;
    DWORD BlockStatYellow;
    DWORD BlockStatGreen;
    DWORD BlockStatBlue;
    DWORD BlockStatPurple;
    DWORD Score;
};

class AudiosurfMemoryScanner  : public QObject
{
    Q_OBJECT

public:
    AudiosurfMemoryScanner(QObject *parent);
    ~AudiosurfMemoryScanner();
};
