#pragma once

#include <QObject>

class WebSocketBroadcastService  : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketBroadcastService(QObject *parent);
    ~WebSocketBroadcastService() override;
};
