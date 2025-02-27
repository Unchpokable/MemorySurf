#pragma once

#include <QObject>

#include "propertymacro.h"
#include "sharedmemoryreader.h"

class WebSocketBroadcastServer final : public QObject
{
    Q_OBJECT

public:
    WebSocketBroadcastServer(QObject* parent, quint16 port);
    virtual ~WebSocketBroadcastServer() override;

    Q_DISABLE_COPY_MOVE(WebSocketBroadcastServer)

    AUTO_PROPERTY(int, packetSkip)

    quint16 port() const;
    void updatePort(quint16 port);

    bool start() const;
    bool stop();

public slots:
    void messageAcquired(const SharedMemoryReader::Buffer& byteData);

private slots:
    void onPendingConnection();

private:
    static const QString _defaultServerName;
    static QString makeJsonFromRawData(const SharedMemoryReader::Buffer& byteData);

    // server

    QWebSocketServer* _server;
    QSet<QWebSocket*> _clients;
    quint16 _port;

    //
    int _currentPacketSkipCount;
};
