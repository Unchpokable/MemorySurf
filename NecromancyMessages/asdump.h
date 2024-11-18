#pragma once

#include "statuscode.h"

namespace Necromancy::Messages::ASDump {

struct ASDumpStruct
{
    float score;
    int statsArraySize;
    float* statsArray;
    float goldThreshold;
    float trafficChainMax;
};

constexpr size_t ASDumpMessageSize = 64; // bytes
constexpr ptrdiff_t ASDump_ScoreFieldOffset = offsetof(ASDumpStruct, score);
constexpr ptrdiff_t ASDump_StatsArraySizeFieldOffset = offsetof(ASDumpStruct, statsArraySize);
constexpr ptrdiff_t ASDump_StatsArrayFieldOffset = offsetof(ASDumpStruct, statsArray);
constexpr ptrdiff_t ASDump_GoldThresholdFieldOffset = offsetof(ASDumpStruct, goldThreshold);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldOffset = offsetof(ASDumpStruct, trafficChainMax);

constexpr ptrdiff_t ASDump_ScoreFieldSize = sizeof(ASDumpStruct::score);
constexpr ptrdiff_t ASDump_StatsArraySizeFieldSize = sizeof(ASDumpStruct::statsArraySize);
constexpr ptrdiff_t ASDump_StatsArrayFieldSize = sizeof(ASDumpStruct::statsArray);
constexpr ptrdiff_t ASDump_GoldThresholdFieldSize = sizeof(ASDumpStruct::goldThreshold);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldSize = sizeof(ASDumpStruct::trafficChainMax);

/**
 * \brief Serializes an \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout.
 * This function serializes object to local buffer and then reassign original pointer to new buffer
 * \param dump Dump object to be serialized 
 * \param buffer pointer-to-pointer to buffer which given dump should be serialized
 * \return StatusCode of operation
 */
StatusCode Serialize(const ASDumpStruct& dump, byte** buffer);

/**
 * \brief Serializes an \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout.
 * This function serializes given object directly to given buffer
 * \param dump Dump object to be serialized
 * \param buffer buffer which given dump should be serialized
 * \param bufferSize size (in bytes) of given buffer. Be sure that this value is an actual size of given buffer to avoid UB
 * \return StatusCode of operation
 */
StatusCode SerializeDirect(const ASDumpStruct& dump, byte* buffer, size_t bufferSize);

/**
 * \brief Deserializes an \c ASDump object from given memory buffer to given ptr to object
 * \param buffer buffer contains ASDump data
 * \param result \c ASDump object where given buffer will be deserialized
 * \return StatusCode of operation
 */
StatusCode Deserialize(const byte* buffer, ASDumpStruct* result);

void Initialize(ASDumpStruct* dumpStruct, int statsArraySize);
void Free(ASDumpStruct* dumpStruct);

}
