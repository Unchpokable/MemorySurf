#pragma once

namespace Necromancy::Constants {

inline const char* MutexName = "Local\\NecromancySharedMemMutex";
inline const char* SharedMemoryName = "Local\\NecromancySharedMem";
inline constexpr std::size_t MessageMaxSize = 128; // 128 bytes for message.

}
