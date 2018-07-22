#include "server/conn_manager.h"
#include "buffer/buffer.h"

#include <QTcpSocket>
#include <QMutex>
#include <QByteArray>
#include <QHash>

QT_USE_NAMESPACE

ConnManager *ConnManager::instance = Q_NULLPTR;

ConnManager *ConnManager::getManager() {
    instLocker.lock();

    if (instance == Q_NULLPTR) {
        instance = new ConnManager;
    }

    instLocker.unlock();

    return instance;
}

void ConnManager::removeManager() {
    instLocker.lock();

    if (instance != Q_NULLPTR) {
        delete instance;
        instance = Q_NULLPTR;
    }

    instLocker.unlock();
}

ConnManager::ConnManager() {

}

ConnManager::~ConnManager() {
    this->removeAll();
}

void ConnManager::append(const QTcpSocket *socket, const QByteArray &data) {
    if (!connections.contains(socket)) {
        connections.insert(socket, new ConnPrivate);
    }

    ConnPrivate *conn = connections.value(socket);
    conn->getLocker().lock();

    conn->updateConn();
    if (!data.isEmpty()) {
        conn->getBuf()->write(data.data(), data.length());
    }

    conn->getLocker().unlock();
}

void ConnManager::remove(const QTcpSocket *socket) {
    if (connections.contains(socket)) {
        ConnPrivate *conn = connections.value(socket);
        connections.remove(socket);

        delete conn;
    }
}

void ConnManager::removeAll() {
    QHash<const QTcpSocket *, ConnPrivate *>::iterator it;
    for (it = connections.begin(); it != connections.end(); ++it) {
        delete it.value();
    }

    connections.clear();
}

void ConnManager::removeAllTimeout(uint timeout) {
    QHash<const QTcpSocket *, ConnPrivate *>::iterator it = connections.begin();
    while (it != connections.end()) {
        ConnPrivate *conn = it.value();
        if (conn->isTimeout(timeout)) {
            it = connections.erase(it);
            delete conn;
        } else {
            ++it;
        }
    }
}
