#pragma once

#include <queue>
#include <mutex>
#include <Windows.h>
#include "protobuf/asdata.pb.h"

class ProtobufSender {

public:
    ProtobufSender() = default;

    static void sendData(ASScanData& data);
};

