#include "types.h"
#include "server/server.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

QT_USE_NAMESPACE

WMServer::WMServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
}

WMServer::~WMServer() {
    this->close();
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

    return true;
}

void WMServer::close() {
    if (server->isListening()) {
        server->close();

        qDebug() << QString("Water-Meter Server shut down.");

        disconnect(server, &QTcpServer::newConnection, this, &WMServer::onConnected);
    }
}

void WMServer::onConnected() {
    QTcpSocket *socket = server->nextPendingConnection();

    qDebug() << QString("Client [%1:%2] has been CONNECTED.")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

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

    socket->write(QString("Reply from Server to Client [%1:%2].")
                  .arg(socket->peerAddress().toString())
                  .arg(socket->peerPort())
                  .toUtf8());
}

void WMServer::onDisconnected() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    qDebug() << QString("Client [%1:%2] has been DISCONNECTED.")
                .arg(socket->peerAddress().toString())
                .arg(socket->peerPort());

    socket->close();

    disconnect(socket, &QTcpSocket::readyRead, this, &WMServer::onReceived);
    disconnect(socket, &QTcpSocket::disconnected, this, &WMServer::onDisconnected);
}