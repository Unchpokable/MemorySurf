#include "pch.h"

#include "ProtobufSender.h"
#include "InitializationError.h"

void ProtobufSender::sendData(const ASScanData &data) const
{
    WaitForSingleObject(_mutex, INFINITE);

    uint32_t size = data.ByteSizeLong();

    std::memcpy(_mapView, &size, sizeof(uint32_t));

    byte* array = new byte[data.ByteSizeLong()];

    data.SerializeToArray(array, data.ByteSizeLong());

    std::memcpy(_mapView + sizeof(uint32_t), array, data.ByteSizeLong());

    delete[] array;

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
