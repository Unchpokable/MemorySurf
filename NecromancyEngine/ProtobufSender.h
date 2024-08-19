#pragma once

#include <queue>
#include <mutex>
#include "protobuf/asdata.pb.h"

class ProtobufSender {

public:
    ProtobufSender();

    void start();
    void stop();

    void sendData(const std::vector<ASScanData>& data);

private:
    void sendingLoop();


    int _sendInterval = 10;
    std::queue<ASScanData> _dataQueue;

    // sync
    std::mutex _mutex;
    std::condition_variable _condition;
    std::thread _sendingThread;

    bool _shouldSend;
};

