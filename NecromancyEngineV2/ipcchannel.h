#pragma once

namespace Necromancy::Messages::ASDump {
struct ASDumpStruct;
}

namespace Necromancy::Ipc {

class IpcChannel final {
public:
    IpcChannel();
    ~IpcChannel();

    void writeBuffer(const Messages::ASDump::ASDumpStruct &data, bool flush = true) const;

private:
    void initializeSharedMemory();

    HANDLE _mutex { INVALID_HANDLE_VALUE };
    HANDLE _sharedMemoryMapping { INVALID_HANDLE_VALUE };
    LPVOID _mapView { nullptr };
};

}
