#ifndef _QPT_HEADER_WATER_METER_SERVER_H_
#define _QPT_HEADER_WATER_METER_SERVER_H_

#include "types.h"
#include "config.h"

#include <QObject>
#include <QTcpServer>

QT_USE_NAMESPACE

class WMServer : public QObject {
    Q_OBJECT

public:
    explicit WMServer(QObject *parent = Q_NULLPTR);
    ~WMServer();

    bool start(uint16 port = WM_SERVER_PORT);
    void close();

private slots:
    void onConnected();
    void onReceived();
    void onDisconnected();

private:
    QTcpServer *server;
};

#endif // server.h
