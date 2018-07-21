#include "types.h"
#include "client/client.h"

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QDebug>

QT_USE_NAMESPACE

WMClient::WMClient(const QString &id, QObject *parent) : QObject(parent) {
    client_id = id;
    socket = new QTcpSocket(this);
}

WMClient::~WMClient() {
    this->close();
}

bool WMClient::connectServer(const QString &host, uint16 port) {
    socket->connectToHost(host, port);

    if (!socket->waitForConnected()) {
        qCritical() << QString("Client [%1] failed to connect Server! Error: %2")
                    .arg(client_id)
                    .arg(socket->errorString());
        return false;
    }

    qDebug() << QString("Client [%1] connected Server.")
                .arg(client_id);

    connect(socket, &QTcpSocket::readyRead, this, &WMClient::onReceived);
    connect(socket, &QTcpSocket::disconnected, this, &WMClient::onDisconnected);

    return true;
}

void WMClient::close() {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
}

bool WMClient::send(const QByteArray &data) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(data);
        if (socket->waitForBytesWritten()) {
            return true;
        }
    }

    qWarning() << QString("Client [%1] failed to send data to Server. Error: %2")
                .arg(client_id)
                .arg(socket->errorString());

    return false;
}

void WMClient::onReceived() {
    QByteArray data = socket->readAll();

    qDebug() << QString("Client [%1] received data from Server. Data: \n\t%2")
                .arg(client_id)
                .arg(QString(data));
}

void WMClient::onDisconnected() {
    qDebug() << QString("Client [%1] disconnected Server.")
                .arg(client_id);

    socket->close();

    disconnect(socket, &QTcpSocket::readyRead, this, &WMClient::onReceived);
    disconnect(socket, &QTcpSocket::disconnected, this, &WMClient::onDisconnected);
}
