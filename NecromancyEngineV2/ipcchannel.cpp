#include "pch.h"
#include "ipcchannel.h"
#include "taggedexception.hpp"
#include "../NecromancyMessages/messages.h"

using namespace Necromancy::Ipc;

IpcChannel::IpcChannel() {
    initializeSharedMemory();
}

IpcChannel::~IpcChannel() {
    WaitForSingleObject(_mutex, INFINITE);
    UnmapViewOfFile(_mapView);
    CloseHandle(_sharedMemoryMapping);
    ReleaseMutex(_mutex);
    CloseHandle(_mutex);
}

void IpcChannel::writeBuffer(const Messages::ASDump::ASDumpStruct& data, bool flush) const {
    WaitForSingleObject(_mutex, INFINITE);

    if(flush) {
        std::memset(_mapView, 0, Constants::MessageMaxSize);
    }

    auto code = BlockwiseSerialize(data, static_cast<byte*>(_mapView), Constants::MessageMaxSize);
    if(code != Messages::StatusCode::Ok) {
        throw RuntimeException("Exception during serializing scanned data into buffer");
    }

    ReleaseMutex(_mutex);
}

void IpcChannel::initializeSharedMemory() {
    SECURITY_DESCRIPTOR mutexSd;
    InitializeSecurityDescriptor(&mutexSd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&mutexSd, TRUE, NULL, FALSE);

    SECURITY_ATTRIBUTES mutexSa;
    mutexSa.nLength = sizeof(SECURITY_ATTRIBUTES);
    mutexSa.lpSecurityDescriptor = &mutexSd;
    mutexSa.bInheritHandle = FALSE;

    _mutex = CreateMutex(&mutexSa, false, Constants::MutexName);
    if(_mutex == NULL)
        throw RuntimeException("Can not create mutex");

    const wchar_t* sddl = L"D:(A;OICI;GA;;;WD)";

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;

    if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl, SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL)) {
        throw RuntimeException("Failed to create security descriptor");
    }

    _sharedMemoryMapping = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, Constants::MessageMaxSize, Constants::SharedMemoryName);

    LocalFree(sa.lpSecurityDescriptor);
    if(_sharedMemoryMapping == NULL) {
        auto error = GetLastError();
        throw RuntimeException("Unable to initialize shared memory! " + std::to_string(error));
    }

    _mapView = MapViewOfFile(_sharedMemoryMapping, FILE_MAP_ALL_ACCESS, 0, 0, Constants::MessageMaxSize);

    if(_mapView == NULL)
        throw RuntimeException("Unable to create shared memory map view");
}
