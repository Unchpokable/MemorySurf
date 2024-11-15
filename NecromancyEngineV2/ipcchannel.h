#pragma once

#include "asdata.pb.h"

namespace Necromancy {
namespace Ipc {

class IpcChannel final {
public:
    IpcChannel();
    ~IpcChannel();

    void writeBuffer(const ASScanData &data, bool flush = true) const;

private:
    void initializeSharedMemory();

    const wchar_t* _mutexName = L"Global\\NecromancySharedMemMutex";
    const wchar_t* _sharedMemoryName = L"Global\\NecromancySharedMem";

    const std::size_t _messageMaxSize = 1024; // 1 KiB for message. Actually ScanData should weight a MUCH LESS

    HANDLE _mutex { INVALID_HANDLE_VALUE };
    HANDLE _sharedMemoryMapping { INVALID_HANDLE_VALUE };
    LPVOID _mapView { nullptr };
};

}
}