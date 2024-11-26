#pragma once

namespace Necromancy::Constants {

inline const wchar_t* MutexName = L"Local\\NecromancySharedMemMutex";
inline const wchar_t* SharedMemoryName = L"Local\\NecromancySharedMem";
inline constexpr std::size_t MessageMaxSize = 128; // 128 bytes for message.

}
