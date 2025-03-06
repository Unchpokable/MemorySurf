#include "pch.h"

#include "ipcchannel.h"
#include "load.h"
#include "logger.h"
#include "NecromancyMessages/messages.h"

using namespace necromancy::ipc;

IpcChannel::IpcChannel()
{
    initializeSharedMemory();
    setupThread();
}

IpcChannel::~IpcChannel()
{
    _threadRunning = false;
    _queueCondition.notify_one();

    if(_ipcThread.joinable()) {
        _ipcThread.join();
    }

    UnmapViewOfFile(_mapView);
    CloseHandle(_sharedMemoryMapping);
    CloseHandle(_mutex);
}

void IpcChannel::writeBuffer(const messages::ASDump::ASDumpStruct& data, bool flush)
{
    {
        std::lock_guard<std::mutex> lock(_queueMutex);
        _messages.push(data);
    }

    _queueCondition.notify_one();
}

void IpcChannel::initializeSharedMemory()
{
    SECURITY_DESCRIPTOR mutexSd;
    InitializeSecurityDescriptor(&mutexSd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&mutexSd, TRUE, NULL, FALSE);

    SECURITY_ATTRIBUTES mutexSa;
    mutexSa.nLength = sizeof(SECURITY_ATTRIBUTES);
    mutexSa.lpSecurityDescriptor = &mutexSd;
    mutexSa.bInheritHandle = FALSE;

    _mutex = CreateMutex(&mutexSa, false, constants::MutexName);
    if(_mutex == NULL) {
        auto error = GetLastError();
        Logger::panic("Memory", "Unable to create mutex: " + std::to_string(error));
        Unload(nullptr);
    }

    const wchar_t* sddl = L"D:(A;OICI;GA;;;WD)";

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;

    if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl, SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL)) {
        Logger::panic("Memory", "Error during converting SDDL");
        Unload(nullptr);
    }

    _sharedMemoryMapping = CreateFileMapping(INVALID_HANDLE_VALUE, &sa, PAGE_READWRITE, 0, constants::MessageMaxSize, constants::SharedMemoryName);

    LocalFree(sa.lpSecurityDescriptor);
    if(_sharedMemoryMapping == NULL) {
        auto error = GetLastError();
        Logger::panic("Memory", "Unable to initialize shared memory! GetLastError code: " + std::to_string(error));
        Unload(nullptr);
    }

    _mapView = MapViewOfFile(_sharedMemoryMapping, FILE_MAP_ALL_ACCESS, 0, 0, constants::MessageMaxSize);

    if(_mapView == NULL) {
        auto error = GetLastError();
        Logger::panic("Memory", "Unable to create shared memory map view: " + std::to_string(error));
        Unload(nullptr);
    }

    Logger::info("IPC Channel initialized");
}

void IpcChannel::setupThread() {
    _ipcThread = std::thread([this]() {
        while(_threadRunning) {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _queueCondition.wait(lock, [this] { return !_messages.empty() || !_threadRunning; });

            while(!_messages.empty()) {
                auto data = _messages.front();
                _messages.pop();

                lock.unlock();

                if(WaitForSingleObject(_mutex, INFINITE) == WAIT_OBJECT_0) {
                    std::memset(_mapView, 0, constants::MessageMaxSize);

                    auto code = BlockwiseSerialize(data, static_cast<byte*>(_mapView), constants::MessageMaxSize);
                    ReleaseMutex(_mutex);

                    if(code != messages::StatusCode::Ok) {
                        Logger::panic("IPC Buffer", "Exception during serializing scanned data into buffer");
                    }
                }

                lock.lock();
            }
        }
    });
}
