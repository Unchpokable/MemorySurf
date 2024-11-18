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

    const wchar_t* _mutexName = L"Local\\NecromancySharedMemMutex";
    const wchar_t* _sharedMemoryName = L"Local\\NecromancySharedMem";

    const std::size_t _messageMaxSize = 128; // 128 bytes for message. Actually ASDumpStruct should weight a MUCH LESS

    HANDLE _mutex { INVALID_HANDLE_VALUE };
    HANDLE _sharedMemoryMapping { INVALID_HANDLE_VALUE };
    LPVOID _mapView { nullptr };
};

}
