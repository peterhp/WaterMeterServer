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

static QString build_data() {
    QString data = QString();
    for (int i = 0; i < 50; ++i) {
        data.append(QString("<LINE%1 It's test data /LINE%1>").arg(i));
    }
    return data;
}

void test_wm_server() {
    QCoreApplication app(argc, argv);

    start_server(&app);

    WMClient *client = start_client("TEST", &app);
    // Client sends data to server, then server replies back.
    client->send("<LINE%1 It's test data /LINE%1>");

    // Application will quit in 1000ms.
    QTimer::singleShot(1000, &app, SLOT(quit()));

    app.exec();
}
