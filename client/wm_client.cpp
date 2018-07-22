#include "types.h"
#include "client/client.h"
#include "protocol/wmp/wmp.h"
#include "protocol/wm_context.h"

#include <stdio.h>
#include <stdlib.h>

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

wmp_plist *WMClient::construct(const QByteArray &data) const {
    wm_cxt *cxt = (wm_cxt *)malloc(sizeof(wm_cxt));
    memset(cxt, 0, sizeof(wm_cxt));

    cxt->data = (byte *)malloc(sizeof(data.length()));
    memcpy(cxt->data, data.constData(), data.length());
    cxt->dlen = data.length();

    memcpy(cxt->hub_id, "TEST", strlen("TEST"));
    memcpy(cxt->wm_id, "ID", strlen("ID"));

    cxt->type = WMD_C2S_HEARTBEAT;

    wmp_plist *plist = wmp_build_packets(cxt);
    wm_cxt_destroy(&cxt);

    return plist;
}

bool WMClient::send(const QByteArray &data) {
    byte *temp = (byte *)malloc(sizeof(byte) * 1024);
    bool sent = false;

    if (socket->state() == QTcpSocket::ConnectedState) {
        wmp_plist *plist = this->construct(data);

        sent = true;
        for (int pi = 0; pi < plist->count; ++pi) {
            int dlen = wmp_serialize(temp, plist->plist[pi], 1024);

            socket->write((char *)temp, dlen);
            if (!socket->waitForBytesWritten()) {
                sent = false;
                break;
            }
        }

        destroy_wmp_packets(&plist);
    }

    free(temp);

    if (!sent) {
        qWarning() << QString("Client [%1] failed to send data to Server. Error: %2")
                    .arg(client_id)
                    .arg(socket->errorString());
    }

    return sent;
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
