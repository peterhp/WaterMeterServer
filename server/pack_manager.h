#ifndef _QPT_HEADER_SERVER_PACKET_MANAGER_H_
#define _QPT_HEADER_SERVER_PACKET_MANAGER_H_

#include "types.h"
#include "util/macros.h"
#include "protocol/wmp/wmp.h"
#include "protocol/wm_context.h"

#include <QHash>
#include <QByteArray>
#include <QTcpSocket>

QT_USE_NAMESPACE

class PackComposer {
public:
    PackComposer(wmp_pkt *pkt);
    ~PackComposer();

    bool add(wmp_pkt *pkt);

    bool isComposed() const;
    wm_cxt *compose();

    inline bool isTimeout(uint timeout) const {
        return CUR_TIME() - timestamp > timeout;
    }

private:
    bool valid(wmp_pkt *pkt) const;

private:
    wmp_plist *plist;

    byte wm_id[WMD_SIZE_WM_ID];
    byte hub_id[WMD_SIZE_HUB_ID];

    uint timestamp;
    uint8 packet_type;
};

class PackManager {
public:
    PackManager();
    ~PackManager();

    void put(const QTcpSocket *socket, wmp_pkt *pkt);
    wm_cxt *compose(const QTcpSocket *socket);

    void remove(const QTcpSocket *socket);
    void removeAll();
    void removeAllTimeout(uint timeout);

private:
    QHash<const QTcpSocket *, PackComposer *> packs;
};

#endif // pack_manager.h
