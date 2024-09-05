#include "pch.h"

#include "ProtobufSender.h"
#include "InitializationError.h"

ProtobufSender::ProtobufSender() {
    initializeSharedMemory();
}

void ProtobufSender::sendData(ASScanData &data) {
    WaitForSingleObject(_mutex, INFINITE);

    std::memcpy(_mapView, &data, data.ByteSizeLong());

    ReleaseMutex(_mutex);
}

void ProtobufSender::initializeSharedMemory() {
    _mutex = CreateMutex(NULL, false, _mutexName);
    if(_mutex == NULL)
        throw InitializationError("Can not create mutex");

    _sharedMemoryMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 
        _messageMaxSize, _sharedMemoryName);

    if(_sharedMemoryMapping == NULL)
        throw InitializationError("Unable to initialize shared memory!");

    _mapView = MapViewOfFile(_sharedMemoryMapping, FILE_MAP_ALL_ACCESS, 0, 0, _messageMaxSize);

    if(_mapView == NULL)
        throw InitializationError("Unable to create shared memory map view");
}
