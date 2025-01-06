#pragma once

#include <QObject>

#include "sharedmemoryreader.h"

class WebSocketBroadcastServer final : public QObject {
    Q_OBJECT

public:
    WebSocketBroadcastServer(QObject* parent, qint16 port);
    virtual ~WebSocketBroadcastServer() override;

    Q_DISABLE_COPY_MOVE(WebSocketBroadcastServer)

    void updatePort(qint16 port);

private slots:
    void messageAcquired(const SharedMemoryReader::Buffer& byteData);
    void onPendingConnection();

private:
    static QString makeJsonFromRawData(const SharedMemoryReader::Buffer& byteData);
    bool start() const;

    QWebSocketServer* _server;
    QSet<QWebSocket*> _clients;
    qint16 _port;
};
