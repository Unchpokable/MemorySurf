#include "pch.h"

#include "websocketbroadcastserver.h"
#include "NecromancyMessages/messages.h"

const QString WebSocketBroadcastServer::_defaultServerName = "MemorySurf broadcast server";

WebSocketBroadcastServer::WebSocketBroadcastServer(QObject *parent, quint16 port)
    : QObject(parent), _server(new QWebSocketServer(_defaultServerName, QWebSocketServer::NonSecureMode, this)), _port(port) {
    connect(_server, &QWebSocketServer::newConnection, this, &WebSocketBroadcastServer::onPendingConnection);
}

WebSocketBroadcastServer::~WebSocketBroadcastServer() {
    _server->close();
    delete _server;
}

void WebSocketBroadcastServer::updatePort(quint16 port) {
    if(_server->isListening()) {
        _server->close();
    }
    delete _server;

    _server = new QWebSocketServer(_defaultServerName, QWebSocketServer::NonSecureMode);
    if(!_server->listen(QHostAddress::LocalHost, port)) {
        throw std::runtime_error("Unable to run the server");
    }
    _port = port;
}

void WebSocketBroadcastServer::messageAcquired(const SharedMemoryReader::Buffer& byteData) {
    if(static_cast<bool>(_packetSkip)) {
        _currentPacketSkipCount++;
        if(_currentPacketSkipCount <= _packetSkip) {
            return;
        }
    }

    auto json = makeJsonFromRawData(byteData);

    for(auto client : _clients) {
        if(client->isValid()) {
            client->sendTextMessage(json);
        }
    }
}

void WebSocketBroadcastServer::onPendingConnection() {
    auto client = _server->nextPendingConnection();
    if(!client) {
        return;
    }

    _clients.insert(client);
    connect(client, &QWebSocket::disconnected, this, [this, client]() {
        _clients.remove(client);
        client->deleteLater();
    });
}

bool WebSocketBroadcastServer::start() const {
    return _server->listen(QHostAddress::LocalHost, _port);
}

QString WebSocketBroadcastServer::makeJsonFromRawData(const SharedMemoryReader::Buffer& byteData) {
    Necromancy::Messages::ASDump::ASDumpStruct data;
    auto result = Deserialize(byteData.data, &data);
    if(result != Necromancy::Messages::StatusCode::Ok) {
        return "<ERROR>"; // todo: handle deserialization errors
    }

    QJsonObject jsonData;
    jsonData["score"] = data.score;
    jsonData["statsArraySize"] = data.statsArraySize;
    jsonData["goldThreshold"] = data.goldThreshold;
    jsonData["trafficChainMax"] = data.trafficChainMax;
    jsonData["largestMatch"] = data.largestMatch;
    jsonData["timestamp"] = data.timestamp;

    QJsonArray statsJsonArray;
    for(std::int32_t i = 0; i < data.statsArraySize; ++i) {
        statsJsonArray.append(data.statsArray[i]);
    }
    jsonData["statsArray"] = statsJsonArray;

    QJsonDocument jsonDoc(jsonData);
    return jsonDoc.toJson(QJsonDocument::Compact);
}
