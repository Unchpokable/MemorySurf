#include "pch.h"
#include "ThisToHostChannel.h"

ThisToHostChannel::ThisToHostChannel() : _channel("Necromancy_sender_channel"), _shouldSend(false) {
    // empty
}

void ThisToHostChannel::start() {
    _shouldSend = true;
    _sendingThread = std::thread(&ThisToHostChannel::sendingLoop, this);
}

void ThisToHostChannel::stop() {
    _shouldSend = false;
    if(_sendingThread.joinable())
        _sendingThread.join();
}

void ThisToHostChannel::sendData(const std::vector<AudiosurfData>& data) {
    std::lock_guard<std::mutex> lock(_mutex);

    for (const auto& it: data)
    {
        _dataQueue.push(it);
    }

    _condition.notify_all();
}

void ThisToHostChannel::sendingLoop() {
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
            _channel.send(&data, sizeof(AudiosurfData));
        }
    }
}
