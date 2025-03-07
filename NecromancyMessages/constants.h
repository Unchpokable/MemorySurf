#pragma once

namespace necromancy::constants {
inline const char* MutexName = "Local\\NecromancySharedMemMutex";
inline const char* SharedMemoryName = "Local\\NecromancySharedMem";
inline constexpr std::size_t MessageMaxSize = 2048; // 2 KiB for message.
inline constexpr std::size_t FullStatsArraySize = 12;
}
