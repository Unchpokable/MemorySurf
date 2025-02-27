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

    void writeBuffer(const messages::ASDump::ASDumpStruct& data, bool flush = true) const;

private:
    void initializeSharedMemory();

    HANDLE _mutex { NULL };
    HANDLE _sharedMemoryMapping { NULL };
    LPVOID _mapView { nullptr };
};
}
