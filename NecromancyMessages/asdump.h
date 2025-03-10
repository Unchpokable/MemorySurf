#pragma once

#include <cstdint>

#include "constants.h"
#include "statuscode.h"

namespace necromancy::messages::ASDump {
#pragma pack(push, 1)
struct ASDumpStruct
{
    float score;
    float trafficChainMax;
    float largestMatch;
    float timeElapsed;
    float totalColors[32];
    std::int32_t totalColorsUsed;
    float collectedColors[32];
    std::int32_t collectedColorsUsed;
};
#pragma pack(pop)

constexpr std::size_t ASDumpMessageSize = constants::MessageMaxSize; // bytes
constexpr std::ptrdiff_t ASDump_ScoreFieldOffset = offsetof(ASDumpStruct, score);
constexpr std::ptrdiff_t ASDump_TrafficChainMaxFieldOffset = offsetof(ASDumpStruct, trafficChainMax);
constexpr std::ptrdiff_t ASDump_LargestMatchFieldOffset = offsetof(ASDumpStruct, largestMatch);
constexpr std::ptrdiff_t ASDump_TimestampFieldOffset = offsetof(ASDumpStruct, timeElapsed);
constexpr std::ptrdiff_t ASDump_TotalColorsFieldOffset = offsetof(ASDumpStruct, totalColors);
constexpr std::ptrdiff_t ASDump_TotalColorsUsedFieldOffset = offsetof(ASDumpStruct, totalColorsUsed);
constexpr std::ptrdiff_t ASDump_CollectedColorsFieldOffset = offsetof(ASDumpStruct, collectedColors);
constexpr std::ptrdiff_t ASDump_CollectedColorsUsedFieldOffset = offsetof(ASDumpStruct, collectedColorsUsed);

constexpr std::ptrdiff_t ASDump_ScoreFieldSize = sizeof(ASDumpStruct::score);
constexpr std::ptrdiff_t ASDump_TrafficChainMaxFieldSize = sizeof(ASDumpStruct::trafficChainMax);
constexpr std::ptrdiff_t ASDump_LargestMatchFieldSize = sizeof(ASDumpStruct::largestMatch);
constexpr std::ptrdiff_t ASDump_TimestampFieldSize = sizeof(ASDumpStruct::timeElapsed);
constexpr std::ptrdiff_t ASDump_TotalColorsFieldSize = sizeof(ASDumpStruct::totalColors);
constexpr std::ptrdiff_t ASDump_TotalColorsUsedFieldSize = sizeof(ASDumpStruct::totalColorsUsed);
constexpr std::ptrdiff_t ASDump_CollectedColorsFieldSize = sizeof(ASDumpStruct::collectedColors);
constexpr std::ptrdiff_t ASDump_CollectedColorsUsedFieldSize = sizeof(ASDumpStruct::collectedColorsUsed);

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
 */
void Initialize(ASDumpStruct* dumpStruct);
}
