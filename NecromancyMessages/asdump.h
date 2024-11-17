#pragma once

#include "statuscode.h"

namespace Necromancy::Messages::ASDump {

#pragma pack(push, 1)
struct ASDumpStruct
{
    float score;
    int statsArraySize;
    float* statsArray;
    float goldThreshold;
    float trafficChainMax;
};
#pragma pack(pop)

constexpr size_t ASDumpMessageSize = 64; // bytes
constexpr ptrdiff_t ASDump_ScoreFieldOffset = 0;
constexpr ptrdiff_t ASDump_StatsArraySizeFieldOffset = ASDump_ScoreFieldOffset + sizeof(float);
constexpr ptrdiff_t ASDump_StatsArrayFieldOffset = ASDump_StatsArraySizeFieldOffset + sizeof(int);
constexpr ptrdiff_t ASDump_GoldThresholdFieldOffset = ASDump_StatsArrayFieldOffset + sizeof(uintptr_t);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldOffset = ASDump_GoldThresholdFieldOffset + sizeof(float);

/**
 * \brief Serializes an \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout
 * \param dump Dump object to be serialized 
 * \param buffer pointer-to-pointer to buffer which given dump should be serialized
 * \return StatusCode of operation
 */
StatusCode Serialize(const ASDumpStruct& dump, byte** buffer);

/**
 * \brief Deserializes an \c ASDump object from given memory buffer to given ptr to object
 * \param buffer buffer contains ASDump data
 * \param result \c ASDump object where given buffer will be deserialized
 * \return StatusCode of operation
 */
StatusCode Deserialize(const byte* buffer, ASDumpStruct* result);

}
