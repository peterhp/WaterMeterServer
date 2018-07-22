#ifndef _QPT_HEADER_WATER_METER_TEST_CLIENT_H_
#define _QPT_HEADER_WATER_METER_TEST_CLIENT_H_

#include "types.h"
#include "protocol/wmp/wmp.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTcpSocket>

QT_USE_NAMESPACE

#define DEFAULT_CLIENT_ID "DEFAULT"

class WMClient : public QObject {
    Q_OBJECT

public:
    explicit WMClient(const QString &id = DEFAULT_CLIENT_ID,
                      QObject *parent = Q_NULLPTR);
    ~WMClient();

    bool connectServer(const QString &host, uint16 port);
    void close();

    bool send(const QByteArray &data);

private:
    wmp_plist *construct(const QByteArray &data) const;

private slots:
    void onReceived();
    void onDisconnected();

private:
    QString client_id;

    QTcpSocket *socket;
};

#endif // client.h
