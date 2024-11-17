#include "pch.h"
#include "ipcchannel.h"

#include "taggedexception.hpp"

using namespace Necromancy::Ipc;

IpcChannel::IpcChannel() {
    initializeSharedMemory();
}

IpcChannel::~IpcChannel() {
    WaitForSingleObject(_mutex, INFINITE);
    ReleaseMutex(_mutex);
    CloseHandle(_mutex);
    UnmapViewOfFile(_mapView);
    CloseHandle(_sharedMemoryMapping);
}

void IpcChannel::writeBuffer(const ASScanData& data, bool flush) const {
    WaitForSingleObject(_mutex, INFINITE);

    if(flush) {
        std::memset(_mapView, 0, _messageMaxSize);
    }

    ReleaseMutex(_mutex);
}

void IpcChannel::initializeSharedMemory() {
    _mutex = CreateMutex(NULL, false, _mutexName);
    if(_mutex == NULL)
        throw RuntimeException("Can not create mutex");

    _sharedMemoryMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        _messageMaxSize, _sharedMemoryName);

    if(_sharedMemoryMapping == NULL)
        throw RuntimeException("Unable to initialize shared memory!");

    _mapView = MapViewOfFile(_sharedMemoryMapping, FILE_MAP_ALL_ACCESS, 0, 0, _messageMaxSize);

    if(_mapView == NULL)
        throw RuntimeException("Unable to create shared memory map view");
}
