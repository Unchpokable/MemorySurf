#include "WebSocketBroadcastService.h"
#include "AudiosurfMemoryScanner.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

WebSocketBroadcastService::WebSocketBroadcastService(quint16 port, QObject *parent)
    : QObject(parent), _server(new QWebSocketServer("ASReader server", QWebSocketServer::NonSecureMode, this)), _port(port)
{
    if (_server->listen(QHostAddress::AnyIPv4, port))
    {
        connect(_server, &QWebSocketServer::newConnection, this, &WebSocketBroadcastService::onNewConnection);
    }
}

WebSocketBroadcastService::~WebSocketBroadcastService()
{
    _server->close();
    qDeleteAll(_clients.begin(), _clients.end());
}

void WebSocketBroadcastService::broadcastMessage(const QString & message) const {
    for (const auto client: _clients)
    {
        client->sendTextMessage(message);
    }
}

void WebSocketBroadcastService::processMessage(const QString & message) {
    Q_UNUSED(message);
}

void WebSocketBroadcastService::socketDisconnected() {
    auto client = qobject_cast<QWebSocket*>(sender());
    if (client)
    {
        _clients.removeAll(client);
        client->deleteLater();
    }
}

QString WebSocketBroadcastService::FormatJson(const AudiosurfData & data) {
    QJsonObject jsonObject;

    QJsonArray blocksArray;
    QJsonObject blockRed, blockYellow, blockGreen, blockBlue, blockPurple;

    blockRed["red"] = (int)data.BlockStatRed;
    blockYellow["yellow"] = (int)data.BlockStatYellow;
    blockGreen["green"] = (int)data.BlockStatGreen;
    blockBlue["blue"] = (int)data.BlockStatBlue;
    blockPurple["purple"] = (int)data.BlockStatPurple;

    blocksArray.append(blockRed);
    blocksArray.append(blockYellow);
    blocksArray.append(blockGreen);
    blocksArray.append(blockBlue);
    blocksArray.append(blockPurple);

    jsonObject["blocks"] = blocksArray;

    jsonObject["score"] = (int)data.Score;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString timestamp = currentDateTime.toString("mm:ss:zzz");
    jsonObject["timestamp"] = timestamp;

    QJsonDocument jsonDoc(jsonObject);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    return jsonString;
}

void WebSocketBroadcastService::onNewConnection()
{
    const auto socket = _server->nextPendingConnection();

    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketBroadcastService::processMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketBroadcastService::socketDisconnected);

    _clients.push_back(socket);
}
