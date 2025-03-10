#include "pch.h"
#include "asdump.h"
#include "messageids.h"

using namespace necromancy::messages;

StatusCode ASDump::SafeSerialize(const ASDumpStruct& dump, byte** buffer)
{
    if(!buffer) return StatusCode::ErrorCausedWrongData;

    auto localBuffer = new byte[ASDumpMessageSize];
    std::memset(localBuffer, 0, ASDumpMessageSize);

    FieldwiseSerialize(dump, localBuffer, ASDumpMessageSize);

    *buffer = localBuffer;
    return StatusCode::Ok;
}

StatusCode ASDump::FieldwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize)
{
    if(bufferSize < ASDumpMessageSize) {
        return StatusCode::ErrorCausedWrongData;
    }

    if(!buffer) {
        return StatusCode::ErrorCausedWrongData;
    }

    auto structPtr = reinterpret_cast<const byte*>(&dump);
    auto serializationPtr = buffer;

    auto messageId(MessageIds::FullDump);

    #define serialize_field(buffer, source, size) \
        std::memcpy(buffer, source, size);       \
        (buffer) += (size)

    serialize_field(serializationPtr, &messageId, sizeof(uint16_t));
    serialize_field(serializationPtr, structPtr + ASDump_ScoreFieldOffset, ASDump_ScoreFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_TrafficChainMaxFieldOffset, ASDump_TrafficChainMaxFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_LargestMatchFieldOffset, ASDump_LargestMatchFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_TimestampFieldOffset, ASDump_TimestampFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_TotalColorsFieldOffset, ASDump_TotalColorsFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_TotalColorsUsedFieldOffset, ASDump_TotalColorsUsedFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_TotalColorsFieldOffset, ASDump_TotalColorsFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_CollectedColorsFieldOffset, ASDump_TotalColorsFieldSize);
    serialize_field(serializationPtr, structPtr + ASDump_CollectedColorsUsedFieldOffset, ASDump_CollectedColorsUsedFieldSize);

    #undef serialize_field

    return StatusCode::Ok;
}

StatusCode ASDump::BlockwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize)
{
    if(bufferSize < ASDumpMessageSize) {
        return StatusCode::ErrorCausedWrongData;
    }
    if(!buffer) {
        return StatusCode::ErrorCausedWrongData;
    }

    auto structPtr = reinterpret_cast<const byte*>(&dump);
    auto serializationPtr = buffer;

    auto messageId(MessageIds::FullDump);

    std::memcpy(serializationPtr, &messageId, sizeof(uint16_t));
    serializationPtr += sizeof(uint16_t);

    // fast copy chunk of stable structure data
    std::memcpy(serializationPtr, structPtr, sizeof(ASDumpStruct));

    return StatusCode::Ok;
}

StatusCode ASDump::Deserialize(const byte* buffer, ASDumpStruct* result)
{
    if(!buffer || !result) {
        return StatusCode::ErrorCausedWrongData;
    }

    uint16_t messageId;
    auto serializationPtr = buffer;

    #define deserialize_field(destination, source, size) \
        std::memcpy(destination, source, size); \
        (source) += (size)

    deserialize_field(&messageId, serializationPtr, sizeof(uint16_t));

    if(messageId != static_cast<uint16_t>(MessageIds::FullDump)) {
        return StatusCode::ErrorCausedWrongData;
    }

    ASDumpStruct data;
    auto structPtr = reinterpret_cast<byte*>(&data);

    deserialize_field(structPtr + ASDump_ScoreFieldOffset, serializationPtr, ASDump_ScoreFieldSize);
    deserialize_field(structPtr + ASDump_TrafficChainMaxFieldOffset, serializationPtr, ASDump_TrafficChainMaxFieldSize);
    deserialize_field(structPtr + ASDump_LargestMatchFieldOffset, serializationPtr, ASDump_LargestMatchFieldSize);
    deserialize_field(structPtr + ASDump_TimestampFieldOffset, serializationPtr, ASDump_TimestampFieldSize);
    deserialize_field(structPtr + ASDump_TotalColorsFieldOffset, serializationPtr, ASDump_TotalColorsFieldSize);
    deserialize_field(structPtr + ASDump_TotalColorsUsedFieldOffset, serializationPtr, ASDump_TotalColorsUsedFieldSize);
    deserialize_field(structPtr + ASDump_CollectedColorsFieldOffset, serializationPtr, ASDump_CollectedColorsFieldSize);
    deserialize_field(structPtr + ASDump_CollectedColorsUsedFieldOffset, serializationPtr, ASDump_CollectedColorsUsedFieldSize);

    #undef deserialize_field

    *result = data;

    return StatusCode::Ok;
}

void ASDump::Initialize(ASDumpStruct* dumpStruct)
{
    dumpStruct->score = 0.0;
    dumpStruct->trafficChainMax = 0.0;
    dumpStruct->largestMatch = 0.0;
}
