#pragma once

#include "windefprettify.h"
#include "NecromancyMessages/asdump.h"

class SharedMemoryReader final : public QObject {
    Q_OBJECT

public:
    ///@brief Service message structure contains data buffer read from memory buffer from game
    struct Buffer {
        byte* data;
        std::size_t size;

        void free() const {
            delete data;
        }

    private:
        friend class SharedMemoryReader;
        void from(const byte* buffer, std::size_t dataSize);
    };

    explicit SharedMemoryReader(QObject* parent = nullptr);
    virtual ~SharedMemoryReader() override = default;

    SharedMemoryReader(const SharedMemoryReader&) = delete;
    SharedMemoryReader(SharedMemoryReader&&) = delete;
    SharedMemoryReader& operator=(const SharedMemoryReader&) = delete;
    SharedMemoryReader& operator=(SharedMemoryReader&&) = delete;

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

    // data
    byte* _buffer;
};
