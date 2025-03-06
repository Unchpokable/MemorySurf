#pragma once

namespace necromancy::messages::ASDump {
struct ASDumpStruct;
}

namespace necromancy::ipc {
class IpcChannel final
{
public:
    IpcChannel();
    ~IpcChannel();

    void writeBuffer(const messages::ASDump::ASDumpStruct& data, bool flush = true);

private:
    void initializeSharedMemory();
    void setupThread();

    HANDLE _mutex { NULL };
    HANDLE _sharedMemoryMapping { NULL };
    LPVOID _mapView { nullptr };

    // threaded IPC
    std::mutex _queueMutex;
    std::condition_variable _queueCondition;
    std::queue<messages::ASDump::ASDumpStruct> _messages;
    bool _threadRunning = true;
    std::thread _ipcThread;
};
}
