#pragma once

#include <ipc.h>
#include <queue>
#include <mutex>

#include "ASData.h"

class ThisToHostChannel {

public:
    ThisToHostChannel();

    void start();
    void stop();

    void sendData(const std::vector<AudiosurfData>& data);

private:
    void sendingLoop();

    ipc::channel _channel;

    int _sendInterval = 10;
    std::queue<AudiosurfData> _dataQueue;

    // sync
    std::mutex _mutex;
    std::condition_variable _condition;
    std::thread _sendingThread;

    bool _shouldSend;
};

