#ifndef _QPT_HEADER_WATER_METER_SERVER_H_
#define _QPT_HEADER_WATER_METER_SERVER_H_

#include "types.h"
#include "config.h"
#include "protocol/wm_context.h"
#include "server/conn_manager.h"
#include "server/pack_manager.h"

#include <QObject>
#include <QTimer>
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

    void onTimer();

private:
    QTcpServer *server;
    QTimer *timer;

    ConnManager *conns;

private:
    void parsePacket(const QTcpSocket *socket);
    wm_cxt *composeData(const QTcpSocket *socket);

    PackManager *packs;
};

#endif // server.h
