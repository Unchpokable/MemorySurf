#pragma once

#include "statuscode.h"

namespace Necromancy::Messages {

#pragma pack(push, 1)
struct ASDump
{
    float score;
    int statsArraySize;
    float* statsArray;
    float goldThreshold;
    float trafficChainMax;
};
#pragma pack(pop)

constexpr size_t ASDumpMessageSize = 256; // bytes
constexpr ptrdiff_t ASDump_ScoreFieldOffset = 0;
constexpr ptrdiff_t ASDump_StatsArraySizeFieldOffset = ASDump_ScoreFieldOffset + sizeof(float);
constexpr ptrdiff_t ASDump_StatsArrayFieldOffset = ASDump_StatsArraySizeFieldOffset + sizeof(int);
constexpr ptrdiff_t ASDump_GoldThresholdFieldOffset = ASDump_StatsArrayFieldOffset + sizeof(uintptr_t);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldOffset = ASDump_GoldThresholdFieldOffset + sizeof(float);

/**
 * \brief Serializes as \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout
 * \param dump Dump object to be serialized 
 * \param buffer pointer-to-pointer to buffer which given dump should be serialized
 * \return StatusCode of operation
 */
StatusCode Serialize(const ASDump& dump, byte** buffer);

/**
 * \brief 
 * \param buffer 
 * \param result 
 * \return 
 */
StatusCode Deserialize(byte* buffer, ASDump* result);

}
