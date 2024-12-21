#pragma once

#include "statuscode.h"
#include <cstdint>

#include "constants.h"

namespace Necromancy::Messages::ASDump {

#pragma pack(push, 1)
struct ASDumpStruct
{
    float score;
    int32_t statsArraySize;
    float goldThreshold;
    float trafficChainMax;
    float largestMatch;
    float timestamp; // msec
    float* statsArray;
};
#pragma pack(pop)

constexpr size_t ASDumpMessageSize = Constants::MessageMaxSize; // bytes
constexpr ptrdiff_t ASDump_ScoreFieldOffset = offsetof(ASDumpStruct, score);
constexpr ptrdiff_t ASDump_StatsArraySizeFieldOffset = offsetof(ASDumpStruct, statsArraySize);
constexpr ptrdiff_t ASDump_GoldThresholdFieldOffset = offsetof(ASDumpStruct, goldThreshold);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldOffset = offsetof(ASDumpStruct, trafficChainMax);
constexpr ptrdiff_t ASDump_LargestMatchFieldOffset = offsetof(ASDumpStruct, largestMatch);
constexpr ptrdiff_t ASDump_TimestampFieldOffset = offsetof(ASDumpStruct, timestamp);
constexpr ptrdiff_t ASDump_StatsArrayFieldOffset = offsetof(ASDumpStruct, statsArray);

constexpr ptrdiff_t ASDump_ScoreFieldSize = sizeof(ASDumpStruct::score);
constexpr ptrdiff_t ASDump_StatsArraySizeFieldSize = sizeof(ASDumpStruct::statsArraySize);
constexpr ptrdiff_t ASDump_GoldThresholdFieldSize = sizeof(ASDumpStruct::goldThreshold);
constexpr ptrdiff_t ASDump_TrafficChainMaxFieldSize = sizeof(ASDumpStruct::trafficChainMax);
constexpr ptrdiff_t ASDump_LargestMatchFieldSize = sizeof(ASDumpStruct::largestMatch);
constexpr ptrdiff_t ASDump_TimestampFieldSize = sizeof(ASDumpStruct::timestamp);
constexpr ptrdiff_t ASDump_StatsArrayFieldSize = sizeof(ASDumpStruct::statsArray);

/**
 * \brief Serializes an \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout.
 * This function serializes object to local buffer and then reassign original pointer to new buffer
 * \param dump Dump object to be serialized 
 * \param buffer pointer-to-pointer to buffer which given dump should be serialized. This buffer will be reassigned by buffer allocated \c Serialize function
 * \return StatusCode of operation
 */
StatusCode SafeSerialize(const ASDumpStruct& dump, byte** buffer);

/**
 * \brief Serializes an \c ASDump object to given memory buffer using linear and byte-to-byte field bytes layout.
 * This function serializes given object directly to given buffer
 * \param dump Dump object to be serialized
 * \param buffer buffer which given dump should be serialized. This function will write data directly to this buffer
 * \param bufferSize size (in bytes) of given buffer. Be sure that this value is an actual size of given buffer to avoid UB
 * \return StatusCode of operation
 */
StatusCode FieldwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize);

/**
 * \brief Serialized an \c ASDumpStruct object to given memory buffer using copying-block-by-block strategy. It means that all fields which insensitive
 * to application architecture and placed one-by-one in memory will by copied by single memory block and then array will be copied by solid block
 * \param dump object to be serialized
 * \param buffer buffer which given dump should be serialized. This function will write data directly to this buffer
 * \param bufferSize size (in bytes) of given buffer. Be sure that this value is an actual size of given buffer to avoid UB
 * \return StatusCode of operation
 */
StatusCode BlockwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize);

/**
 * \brief Deserializes an \c ASDump object from given memory buffer to given ptr to object
 * \param buffer buffer contains ASDump data
 * \param result \c ASDump object where given buffer will be deserialized
 * \return StatusCode of operation
 */
StatusCode Deserialize(const byte* buffer, ASDumpStruct* result);

/**
 * \brief Initializes a new \c ASDumpStruct struct object by setting all fields to 0 and initialize array of \c ASDumpStruct::statsArray by given size
 * \param dumpStruct pointer to newly created \c ASDumpStruct
 * \param statsArraySize size of \c ASDumpStruct::statsArray array
 */
void Initialize(ASDumpStruct* dumpStruct, int statsArraySize);

/**
 * \brief Frees a memory used by \c ASDumpStruct struct object
 * \param dumpStruct pointer to \c ASDumpStruct object
 */
void Free(ASDumpStruct* dumpStruct);

}
