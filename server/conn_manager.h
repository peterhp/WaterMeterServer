#ifndef _QPT_HEADER_SERVER_CONNECTION_MANAGER_H_
#define _QPT_HEADER_SERVER_CONNECTION_MANAGER_H_

#include "types.h"
#include "util/macros.h"
#include "buffer/buffer.h"

#include <QTcpSocket>
#include <QMutex>
#include <QByteArray>
#include <QHash>

QT_USE_NAMESPACE

#define TIMESTAMP() (QDateTime::currentMSecsSinceEpoch())

// It's a singleton class to manage all connections.
class ConnManager {
public:
    static ConnManager *getManager();
    static void removeManager();

private:
    ConnManager();
    ~ConnManager();

    static ConnManager *instance;
    static QMutex instLocker;

private:
    class ConnPrivate {
    public:
        inline ConnPrivate() {
            buf = new CircularBuffer();
            lastConn = CUR_TIME();
        }
        inline ~ConnPrivate() {
            rwLocker.lock();
            delete buf;
            rwLocker.unlock();
        }

        inline Buffer *getBuf() const {return buf;}
        inline QMutex &getLocker() const {return rwLocker;}

        inline void updateConn() {lastConn = CUR_TIME();}
        inline bool isTimeout(uint timeout) const {
            return (CUR_TIME() - lastConn) > timeout;
        }

    private:
        Buffer *buf;
        QMutex rwLocker;

        uint lastConn;
    };

public:
    inline bool exists(const QTcpSocket *socket) const {
        return connections.contains(socket);
    }

    void append(const QTcpSocket *socket, const QByteArray &data = QByteArray());
    void remove(const QTcpSocket *socket);
    void removeAll();

    inline bool isTimeout(const QTcpSocket *socket, uint timeout) const {
        return connections.contains(socket) &&
                connections.value(socket)->isTimeout(timeout);
    }
    void removeAllTimeout(uint timeout);

private:
    QHash<const QTcpSocket *, ConnPrivate *> connections;
};

#endif // conn_manager.h
