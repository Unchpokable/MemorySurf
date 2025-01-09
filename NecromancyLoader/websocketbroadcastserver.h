#pragma once

#include <QObject>

#include "sharedmemoryreader.h"
#include "propertymacro.h"

class WebSocketBroadcastServer final : public QObject {
    Q_OBJECT

public:
    WebSocketBroadcastServer(QObject* parent, quint16 port);
    virtual ~WebSocketBroadcastServer() override;

    Q_DISABLE_COPY_MOVE(WebSocketBroadcastServer)

    void updatePort(quint16 port);

    AUTO_PROPERTY(int, packetSkip)

    bool start() const;

private slots:
    void messageAcquired(const SharedMemoryReader::Buffer& byteData);
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
