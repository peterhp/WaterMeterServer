#include "types.h"
#include "protocol/wmp/wmp.h"
#include "protocol/wm_context.h"
#include "server/pack_manager.h"

#include <stdlib.h>
#include <string.h>

#include <QHash>
#include <QTcpSocket>

QT_USE_NAMESPACE

PackComposer::PackComposer(wmp_pkt *pkt) {
    plist = (wmp_plist *)malloc(sizeof(wmp_plist));
    memset(plist, 0, sizeof(wmp_plist));

    memcpy(hub_id, pkt->hub_id, WMD_SIZE_HUB_ID);
    memcpy(wm_id, pkt->wm_id, WMD_SIZE_WM_ID);
    timestamp = pkt->timestamp;
    packet_type = pkt->packet_type;

    wmp_compose_packet(plist, pkt);
}

PackComposer::~PackComposer() {
    destroy_wmp_packets(&plist);
}

bool PackComposer::add(wmp_pkt *pkt) {
    return valid(pkt) && wmp_compose_packet(plist, pkt) >= 0;
}

bool PackComposer::valid(wmp_pkt *pkt) const {
    return !memcmp(hub_id, pkt->hub_id, WMD_SIZE_HUB_ID) &&
            !memcmp(wm_id, pkt->wm_id, WMD_SIZE_WM_ID) &&
            packet_type == pkt->packet_type;
}

bool PackComposer::isComposed() const {
    return wmp_is_composed(plist);
}

wm_cxt *PackComposer::compose() {
    wm_cxt *cxt = (wm_cxt *)malloc(sizeof(wm_cxt));
    wmp_compose_data(cxt, plist);
    return cxt;
}


/**************************************************************************/

PackManager::PackManager() {

}

PackManager::~PackManager() {
    this->removeAll();
}

void PackManager::put(const QTcpSocket *socket, wmp_pkt *pkt) {
    if (!packs.contains(socket)) {
        packs.insert(socket, new PackComposer(pkt));
        return ;
    }

    if (!packs.value(socket)->add(pkt)) {
        delete pkt;
    }
}

wm_cxt *PackManager::compose(const QTcpSocket *socket) {
    if (packs.contains(socket)) {
        PackComposer *composer = packs.value(socket);
        if (composer->isComposed()) {
            packs.remove(socket);

            wm_cxt *cxt = composer->compose();
            delete composer;
            return cxt;
        }
    }

    return NULL;
}

void PackManager::remove(const QTcpSocket *socket) {
    if (packs.contains(socket)) {
        PackComposer *composer = packs.value(socket);
        packs.remove(socket);

        delete composer;
    }
}

void PackManager::removeAll() {
    QHash<const QTcpSocket *, PackComposer *>::iterator it;
    for (it = packs.begin(); it != packs.end(); ++it) {
        delete it.value();
    }

    packs.clear();
}

void PackManager::removeAllTimeout(uint timeout) {
    QHash<const QTcpSocket *, PackComposer *>::iterator it = packs.begin();
    while (it != packs.end()) {
        PackComposer *composer = it.value();
        if (composer->isTimeout(timeout)) {
            it = packs.erase(it);
            delete composer;
        } else {
            ++it;
        }
    }
}
