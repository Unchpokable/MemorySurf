#include "pch.h"
#include "ProtobufSender.h"

ProtobufSender::ProtobufSender() : _shouldSend(false) {
    // empty
}

void ProtobufSender::start() {
    _shouldSend = true;
    _sendingThread = std::thread(&ProtobufSender::sendingLoop, this);
}

void ProtobufSender::stop() {
    _shouldSend = false;
    if(_sendingThread.joinable())
        _sendingThread.join();
}

void ProtobufSender::sendData(const std::vector<ASScanData>& data) {
    std::lock_guard<std::mutex> lock(_mutex);

    for (const auto& it: data)
    {
        _dataQueue.push(it);
    }

    _condition.notify_all();
}

void ProtobufSender::sendingLoop() {
    while(_shouldSend)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait_for(lock, std::chrono::milliseconds(_sendInterval), [this]
        {
            return !_dataQueue.empty() || !_shouldSend;
        });

        while(!_dataQueue.empty())
        {
            auto data = _dataQueue.front();
            _dataQueue.pop();
            // todo: send data
        }
    }
}
