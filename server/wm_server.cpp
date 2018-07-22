#include "types.h"
#include "buffer/buffer.h"
#include "protocol/wmp/wmp.h"
#include "server/server.h"
#include "server/conn_manager.h"

#include <stdlib.h>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

QT_USE_NAMESPACE

WMServer::WMServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    timer = new QTimer(this);
    conns = ConnManager::getManager();
    packs = new PackManager;
}

WMServer::~WMServer() {
    this->close();
    conns->removeManager();
    delete packs;
}

bool WMServer::start(uint16 port) {
    if (!server->listen(QHostAddress::AnyIPv4, port)) {
        qCritical() << QString("Water-Meter Server failed to startup on port [%1]! Error: %2")
                    .arg(port)
                    .arg(server->errorString());
        return false;
    }

    qDebug() << QString("Water-Meter Server started up on port [%1].")
                .arg(port);

    connect(server, &QTcpServer::newConnection, this, &WMServer::onConnected);

    timer->start(200);
    connect(timer, &QTimer::timeout, this, &WMServer::onTimer);

    return true;
}

void WMServer::close() {
    if (server->isListening()) {
        server->close();

        qDebug() << QString("Water-Meter Server shut down.");

        disconnect(server, &QTcpServer::newConnection, this, &WMServer::onConnected);

        timer->stop();
        disconnect(timer, &QTimer::timeout, this, &WMServer::onTimer);
    }
}

void WMServer::onConnected() {
    QTcpSocket *socket = server->nextPendingConnection();

    qDebug() << QString("Client [%1:%2] has been CONNECTED.")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

    conns->append(socket);

    connect(socket, &QTcpSocket::readyRead, this, &WMServer::onReceived);
    connect(socket, &QTcpSocket::disconnected, this, &WMServer::onDisconnected);
}

void WMServer::onReceived() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = socket->readAll();

    qDebug() << QString("Received data from Client [%1:%2]. Data: \n\t%3")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort())
                .arg(QString(data));

    conns->append(socket, data);

    this->parsePacket(socket);
    wm_cxt *cxt = this->composeData(socket);
    if (cxt) {
        socket->write(QString("Reply from Server to Client [%1:%2].")
                      .arg(socket->peerAddress().toString())
                      .arg(socket->peerPort())
                      .toUtf8());
    }

    wm_cxt_destroy(&cxt);
}

void WMServer::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    qDebug() << QString("Client [%1:%2] has been DISCONNECTED.")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

    socket->close();

    conns->remove(socket);
    packs->remove(socket);

    disconnect(socket, &QTcpSocket::readyRead, this, &WMServer::onReceived);
    disconnect(socket, &QTcpSocket::disconnected, this, &WMServer::onDisconnected);
}

void WMServer::onTimer() {
    // Remove dead socket connections
    conns->removeAllTimeout(DEFAULT_CONN_TIMEOUT);
    packs->removeAllTimeout(DEFAULT_CONN_TIMEOUT);
}

void WMServer::parsePacket(const QTcpSocket *socket) {
    Buffer *buf = conns->getBuffer(socket);

    byte temp[1024];
    while (buf->data_size() >= wmp_pack_min_size()) {
        buf->peek(temp, WMP_HEAD_LEN);
        uint pack_size = wmp_pack_size(temp, WMP_HEAD_LEN);
        if (buf->data_size() < pack_size) {
            break;
        }

        // Read the whole packet
        buf->read(temp, pack_size);
        wmp_pkt *packet = (wmp_pkt *)malloc(sizeof(wmp_pkt));

        // Parse & validate the packet
        if (wmp_deserialize(packet, temp, pack_size) > 0) {
            packs->put(socket, packet);
        } else {
            free(packet);
        }
    }
}

wm_cxt *WMServer::composeData(const QTcpSocket *socket) {
    return packs->compose(socket);
}
