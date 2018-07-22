#include <QCoreApplication>

#include "test/test.h"
#include "server/server.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    WMServer server(&app);
    server.start(10000);

    return app.exec();

//    test_wm_server();

//    return 0;
}
