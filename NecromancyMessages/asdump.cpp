#include "pch.h"
#include "asdump.h"
#include "messageids.h"

using namespace Necromancy::Messages;

StatusCode Necromancy::Messages::Serialize(const ASDump& dump, byte** buffer) {
    if(!buffer) return StatusCode::ErrorCausedWrongData;
    auto structPtr = reinterpret_cast<const byte*>(&dump);

    auto localBuffer = new byte[ASDumpMessageSize];
    std::memset(localBuffer, 0, ASDumpMessageSize);

    auto serializationPtr = localBuffer;

    auto messageId(MessageIds::FullDump);

    std::memcpy(serializationPtr, &messageId, sizeof(uint16_t));
    serializationPtr += sizeof(uint16_t);


    std::memcpy(serializationPtr, structPtr + ASDump_ScoreFieldOffset, sizeof(float));
    serializationPtr += sizeof(float);

    std::memcpy(serializationPtr, structPtr + ASDump_StatsArraySizeFieldOffset, sizeof(int));
    serializationPtr += sizeof(int);

    std::memcpy(serializationPtr, dump.statsArray, sizeof(float) * dump.statsArraySize);
    serializationPtr += sizeof(float) * dump.statsArraySize;

    std::memcpy(serializationPtr, structPtr + ASDump_GoldThresholdFieldOffset, sizeof(float));
    serializationPtr += sizeof(float);

    std::memcpy(serializationPtr, structPtr + ASDump_TrafficChainMaxFieldOffset, sizeof(float));
    serializationPtr += sizeof(float);

    return StatusCode::Ok;
}

StatusCode Necromancy::Messages::Deserialize(byte* buffer, ASDump* result) {
    ASDump data;

    uint16_t messageId;

    std::memcpy(&messageId, buffer, sizeof(uint16_t));

    if(messageId != static_cast<uint16_t>(MessageIds::FullDump)) {
        return StatusCode::ErrorCausedWrongData;
    }



    return StatusCode::Ok;
}
