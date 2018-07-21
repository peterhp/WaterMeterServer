#include <QCoreApplication>
#include <QObject>
#include <QList>
#include <QTimer>

QT_USE_NAMESPACE

#include "server/server.h"
#include "client/client.h"
#include "test/test.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 10000

static int argc = 1;
static char *argv[] = {"test"};

static WMServer *start_server(QObject *parent = Q_NULLPTR) {
    WMServer *server = new WMServer(parent);
    server->start(SERVER_PORT);
    return server;
}

static WMClient *start_client(
        const QString &id = "TEST",
        QObject *parent = Q_NULLPTR) {
    WMClient *client = new WMClient(id, parent);
    client->connectServer(SERVER_HOST, SERVER_PORT);
    return client;
}

void test_single_client_server() {
    QCoreApplication app(argc, argv);

    start_server(&app);

    WMClient *client = start_client("TEST", &app);
    // Client sends data to server, then server replies back.
    client->send("Data to test communication between server & single client.");

    // Application will quit in 1000ms.
    QTimer::singleShot(1000, &app, SLOT(quit()));

    app.exec();
}

void test_multi_client_server() {
    QCoreApplication app(argc, argv);

    start_server(&app);

    QList<WMClient *> clients;
    for (int i = 0; i < 5; ++i) {
        QString cid = QString("TC%1").arg(i + 1);
        clients.append(start_client(cid, &app));
    }

    for (int i = 0; i < 5; ++i) {
        clients[i]->send("First data from multi-clients to server.");
    }

    for (int i = 4; i >=0; --i) {
        clients[i]->send("Second data from multi-clients to server.");
    }

    // Application will quit in 1000ms.
    QTimer::singleShot(1000, &app, SLOT(quit()));

    app.exec();
}
