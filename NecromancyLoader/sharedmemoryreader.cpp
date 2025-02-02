#include "pch.h"
#include "sharedmemoryreader.h"
#include "NecromancyMessages/messages.h"

void SharedMemoryReader::Buffer::from(const byte* buffer, std::size_t dataSize) {
    data = new byte[dataSize];
    std::memcpy(data, buffer, dataSize);
    size = dataSize;
}

SharedMemoryReader::SharedMemoryReader(QObject* parent) : QObject(parent) {
    _pollTimer = new QTimer(this);
    _pollTimer->setInterval(_initInterval);
    _initTimer = new QElapsedTimer();

    _buffer = new byte[Necromancy::Constants::MessageMaxSize];
}

SharedMemoryReader::~SharedMemoryReader() {
    CloseHandle(_mutex);
    CloseHandle(_sharedMemoryHandle);
}

void SharedMemoryReader::enable() const {
    _pollTimer->start();
}

void SharedMemoryReader::disable() const {
    _pollTimer->stop();
}

void SharedMemoryReader::setInitTimeout(int msec) {
    _initTimeout = msec;
}

void SharedMemoryReader::setBufferReadInterval(int msec) {
    _bufferPollInterval = msec;
}

void SharedMemoryReader::startInit() {
    disconnect(_pollTimer, &QTimer::timeout, this, &SharedMemoryReader::readBuffer);
    connect(_pollTimer, &QTimer::timeout, this, &SharedMemoryReader::tryInit);
    _pollTimer->start();

    _initTimer->start();
}

void SharedMemoryReader::readBuffer() {
    WaitForSingleObject(_mutex, INFINITE);
    std::memcpy(_buffer, _smMapView, Necromancy::Constants::MessageMaxSize);
    ReleaseMutex(_mutex);

    Buffer data;
    data.from(_buffer, Necromancy::Constants::MessageMaxSize);

    emit messageAcquired(data);
}

void SharedMemoryReader::tryInit() {
    auto initialized = init();
    if(!initialized) {
        return;
    }
    auto elapsed = _initTimer->elapsed();
    if(elapsed > _initInterval)
    {
        _pollTimer->stop();
        disconnect(_pollTimer, &QTimer::timeout, this, &SharedMemoryReader::tryInit);
        emit initializationTimedOut();
        return;
    }

    _pollTimer->stop();

    disconnect(_pollTimer, &QTimer::timeout, this, &SharedMemoryReader::tryInit);
    connect(_pollTimer, &QTimer::timeout, this, &SharedMemoryReader::readBuffer);

    _pollTimer->setInterval(_bufferPollInterval);

    emit this->initialized();
}

bool SharedMemoryReader::init() {
    auto mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, Necromancy::Constants::MutexName);

    if(!mutex) {
        return false;
    }

    _mutex = mutex;

    auto memoryMap = OpenFileMapping(FILE_MAP_READ, FALSE, Necromancy::Constants::SharedMemoryName);
    if(!memoryMap) {
        return false;
    }

    _sharedMemoryHandle = memoryMap;

    _smMapView = MapViewOfFile(_sharedMemoryHandle, FILE_MAP_READ, NULL, NULL, Necromancy::Constants::MessageMaxSize);
    if(_smMapView == nullptr) {
        throw std::runtime_error("Unable to map view of file when file exists");
    }

    return true;
}
