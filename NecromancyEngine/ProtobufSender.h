#pragma once

#include "protobuf/asdata.pb.h"

class ProtobufSender {

public:
    ProtobufSender();

    void sendData(ASScanData& data);

private:
    void initializeSharedMemory();

    const wchar_t* _mutexName = L"Global\\NecromancyShareMemMutex";
    const wchar_t* _sharedMemoryName = L"Global\\NecromancyShareMem";

    const size_t _messageMaxSize = 1024; // 1 KiB for message. Actually ScanData should weight a MUCH LESS

    HANDLE _mutex;
    HANDLE _sharedMemoryMapping;
    LPVOID _mapView;
};
