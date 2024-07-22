#pragma once

#include <QWebSocket>
#include <QObject>
#include <QWebSocketServer>
#include <vector>

struct AudiosurfData;

class WebSocketBroadcastService  : public QObject
{
    Q_OBJECT

public:
    WebSocketBroadcastService(quint16 port, QObject *parent = nullptr);
    ~WebSocketBroadcastService();

public slots:
    void broadcastMessage(const QString& message) const;

private slots:
    void onNewConnection();
    void processMessage(const QString& message);
    void socketDisconnected();

private:
    QString FormatJson(const AudiosurfData& data);

    QWebSocketServer* _server;
    QList<QWebSocket*> _clients;

    quint16 _port;
};
