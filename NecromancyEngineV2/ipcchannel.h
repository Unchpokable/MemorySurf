#pragma once

namespace necromancy::Messages::ASDump {
struct ASDumpStruct;
}

namespace necromancy::Ipc {

class IpcChannel final {
public:
    IpcChannel();
    ~IpcChannel();

    void writeBuffer(const Messages::ASDump::ASDumpStruct &data, bool flush = true) const;

private:
    void initializeSharedMemory();

    HANDLE _mutex { NULL };
    HANDLE _sharedMemoryMapping { NULL };
    LPVOID _mapView { nullptr };
};

}
