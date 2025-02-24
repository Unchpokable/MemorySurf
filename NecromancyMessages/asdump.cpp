#include "pch.h"
#include "asdump.h"
#include "messageids.h"

using namespace necromancy::Messages;

StatusCode ASDump::SafeSerialize(const ASDumpStruct& dump, byte** buffer) {
    if(!buffer) return StatusCode::ErrorCausedWrongData;

    auto localBuffer = new byte[ASDumpMessageSize];
    std::memset(localBuffer, 0, ASDumpMessageSize);

    FieldwiseSerialize(dump, localBuffer, ASDumpMessageSize);

    *buffer = localBuffer;
    return StatusCode::Ok;
}

StatusCode ASDump::FieldwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize) {
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

    std::memcpy(serializationPtr, structPtr + ASDump_ScoreFieldOffset, ASDump_ScoreFieldOffset);
    serializationPtr += ASDump_ScoreFieldSize;

    std::memcpy(serializationPtr, structPtr + ASDump_StatsArraySizeFieldOffset, ASDump_StatsArraySizeFieldSize);
    serializationPtr += ASDump_StatsArraySizeFieldSize;

    std::memcpy(serializationPtr, structPtr + ASDump_GoldThresholdFieldOffset, ASDump_GoldThresholdFieldSize);
    serializationPtr += ASDump_GoldThresholdFieldSize;

    std::memcpy(serializationPtr, structPtr + ASDump_TrafficChainMaxFieldOffset, ASDump_TrafficChainMaxFieldSize);
    serializationPtr += ASDump_TrafficChainMaxFieldSize;

    std::memcpy(serializationPtr, structPtr + ASDump_LargestMatchFieldOffset, ASDump_LargestMatchFieldSize);
    serializationPtr += ASDump_LargestMatchFieldSize;

    std::memcpy(serializationPtr, structPtr + ASDump_TimestampFieldOffset, ASDump_TimestampFieldSize);
    serializationPtr += ASDump_TimestampFieldSize;

    std::memcpy(serializationPtr, dump.statsArray, sizeof(float) * dump.statsArraySize);

    return StatusCode::Ok;
}

StatusCode ASDump::BlockwiseSerialize(const ASDumpStruct& dump, byte* buffer, size_t bufferSize) {
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
    std::memcpy(serializationPtr, structPtr, ASDump_TimestampFieldOffset + ASDump_TimestampFieldSize);

    serializationPtr += ASDump_TimestampFieldOffset + ASDump_TimestampFieldSize;

    std::memcpy(serializationPtr, dump.statsArray, sizeof(float) * dump.statsArraySize);

    return StatusCode::Ok;

}

StatusCode ASDump::Deserialize(const byte* buffer, ASDumpStruct* result) {
    ASDumpStruct data;

    uint16_t messageId;

    std::memcpy(&messageId, buffer, sizeof(uint16_t));

    if(messageId != static_cast<uint16_t>(MessageIds::FullDump)) {
        return StatusCode::ErrorCausedWrongData;
    }

    auto structPtr = reinterpret_cast<byte*>(&data);
    auto serializationPtr = buffer;
    serializationPtr += sizeof(uint16_t);

    std::memcpy(structPtr + ASDump_ScoreFieldOffset, serializationPtr, ASDump_ScoreFieldSize);
    serializationPtr += ASDump_ScoreFieldSize;

    std::memcpy(structPtr + ASDump_StatsArraySizeFieldOffset, serializationPtr, ASDump_StatsArraySizeFieldSize);
    serializationPtr += ASDump_StatsArraySizeFieldSize;


    std::memcpy(structPtr + ASDump_GoldThresholdFieldOffset, serializationPtr, ASDump_GoldThresholdFieldSize);
    serializationPtr += ASDump_GoldThresholdFieldSize;

    std::memcpy(structPtr + ASDump_TrafficChainMaxFieldOffset, serializationPtr, ASDump_TrafficChainMaxFieldSize);

    serializationPtr += ASDump_TrafficChainMaxFieldSize;
    std::memcpy(structPtr + ASDump_LargestMatchFieldOffset, serializationPtr, ASDump_LargestMatchFieldSize);

    serializationPtr += ASDump_LargestMatchFieldSize;
    std::memcpy(structPtr + ASDump_TimestampFieldOffset, serializationPtr, ASDump_TimestampFieldSize);

    serializationPtr += ASDump_TimestampFieldSize;
    // copy array
    float* arrayBuffer = new float[data.statsArraySize]; // should be filled already
    std::memcpy(arrayBuffer, serializationPtr, sizeof(float) * data.statsArraySize);
    data.statsArray = arrayBuffer;


    *result = data;
    return StatusCode::Ok;
}


void ASDump::Initialize(ASDumpStruct* dumpStruct, int statsArraySize) {
    dumpStruct->score = 0.0;
    dumpStruct->statsArraySize = statsArraySize;
    dumpStruct->statsArray = new float[statsArraySize];
    dumpStruct->goldThreshold = 0.0;
    dumpStruct->trafficChainMax = 0.0;
    dumpStruct->largestMatch = 0.0;
}

void ASDump::Free(ASDumpStruct* dumpStruct) {
    delete[] dumpStruct->statsArray;
    dumpStruct->statsArray = nullptr;
}


