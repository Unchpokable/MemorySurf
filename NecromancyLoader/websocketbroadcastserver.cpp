#include "pch.h"

#include "websocketbroadcastserver.h"
#include "NecromancyMessages/messages.h"

const QString WebSocketBroadcastServer::_defaultServerName = "MemorySurf";

WebSocketBroadcastServer::WebSocketBroadcastServer(QObject* parent, quint16 port)
: QObject(parent), _server(new QWebSocketServer(_defaultServerName, QWebSocketServer::NonSecureMode, this)), _port(port)
{
    connect(_server, &QWebSocketServer::newConnection, this, &WebSocketBroadcastServer::onPendingConnection);
}

WebSocketBroadcastServer::~WebSocketBroadcastServer()
{
    _server->close();
    delete _server;
}

quint16 WebSocketBroadcastServer::port() const
{
    return _port;
}

void WebSocketBroadcastServer::updatePort(quint16 port)
{
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

void WebSocketBroadcastServer::messageAcquired(const SharedMemoryReader::Buffer& byteData)
{
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

void WebSocketBroadcastServer::onPendingConnection()
{
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

bool WebSocketBroadcastServer::start() const
{
    return _server->listen(QHostAddress::LocalHost, _port);
}

bool WebSocketBroadcastServer::stop()
{
    if(_server->isListening()) {
        _clients.clear();
        _server->close();
        delete _server;
        _server = new QWebSocketServer(_defaultServerName, QWebSocketServer::NonSecureMode);

        return true;
    }

    return false;
}

QString WebSocketBroadcastServer::makeJsonFromRawData(const SharedMemoryReader::Buffer& byteData)
{
    necromancy::messages::ASDump::ASDumpStruct data;
    auto result = Deserialize(byteData.data, &data);
    if(result != necromancy::messages::StatusCode::Ok) {
        return "<ERROR>"; // todo: handle deserialization errors
    }

    QJsonObject jsonData;
    jsonData["score"] = data.score;
    jsonData["trafficChainMax"] = data.trafficChainMax;
    jsonData["largestMatch"] = data.largestMatch;
    jsonData["timeElapsed"] = data.timeElapsed;

    QJsonArray totalColorsArray;

    for(std::int32_t i = 0; i < data.totalColorsUsed && i < 32; ++i) {
        totalColorsArray.append(data.totalColors[i]);
    }

    jsonData["totalColors"] = totalColorsArray;

    QJsonArray collectedColorsArray;

    for(std::int32_t i = 0; i < data.collectedColorsUsed && i < 32; ++i) {
        collectedColorsArray.append(data.collectedColors[i]);
    }

    jsonData["collectedColors"] = collectedColorsArray;

    QJsonDocument jsonDoc(jsonData);
    return jsonDoc.toJson(QJsonDocument::Compact);
}
