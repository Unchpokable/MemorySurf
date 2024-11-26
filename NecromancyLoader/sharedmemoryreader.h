#pragma once

#include "windefprettify.h"

class SharedMemoryReader final : QObject {
    Q_OBJECT

public:
    struct Buffer {
        byte* data;
        std::size_t size;

        void free() const {
            delete data;
        }
    };

    SharedMemoryReader(QObject* parent = nullptr);
    virtual ~SharedMemoryReader() override = default;

    void enable() const;
    void disable() const;

    void setInitTimeout(int msec);
    void setBufferReadInterval(int msec);
    void startInit();

signals:
    void initialized();
    void initializationTimedOut();
    void messageAcquired(Buffer buffer);

private slots:
    void readBuffer();
    void tryInit();

private:
    bool init();

    int _initTimeout { 10000 };
    int _bufferPollInterval { 25 };
    int _initInterval { 100 };

    // initialization

    QElapsedTimer* _initTimer;

    //

    QTimer* _pollTimer;

    WinHandle _mutex;
    WinHandle _sharedMemoryHandle;
    VoidPtr _smMapView;
};
