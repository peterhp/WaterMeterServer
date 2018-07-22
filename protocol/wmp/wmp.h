#ifndef _QPT_HEADER_WATER_METER_PROTOCOL_H_
#define _QPT_HEADER_WATER_METER_PROTOCOL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#include "protocol/wm_context.h"

#define WMP_PREFIX  0xFEFE
#define WMP_POSTFIX 0xEDED

#define PROTO_WMP   0x53

#define WMP_HEAD_LEN        64
#define WMP_TAIL_LEN        2
#define WMP_MAX_DATA_SIZE   800

typedef struct wmp_packet {
    uint16  prefix;         // FE FE
    uint8   protocol_type;  // 53
    uint8   packet_type;

    uint32  timestamp;

    byte    wm_id[WMD_SIZE_WM_ID];
    byte    hub_id[WMD_SIZE_HUB_ID];

    byte    signature[32];

    uint8   packet_count;
    uint8   packet_number;
    uint16  reserved;

    uint16  data_len;
    uint16  check_sum;

    byte    data[WMP_MAX_DATA_SIZE];

    uint16  postfix;        // ED ED
} wmp_pkt;

extern int wmp_pack_min_size();

extern int wmp_pack_size(const byte *hdr, uint len);

extern int wmp_serialize(byte *buf, const wmp_pkt *pkt, uint blen);
extern int wmp_deserialize(wmp_pkt *pkt, const byte *data, uint blen);

typedef struct wmp_packet_list {
    uint32 count;
    wmp_pkt **plist;
} wmp_plist;

#define WMP_COMPOSE_UPDATED     0x1
#define WMP_COMPOSE_COMPLETED   0x2
#define WMP_COMPOSE_FAILED      -1

// build wmp packets for data to send
extern wmp_plist *wmp_build_packets(const wm_cxt *cxt);
// compose data from received packets
extern int wmp_compose_packet(wmp_plist *plist, wmp_pkt *pkt);
extern void wmp_compose_data(wm_cxt *cxt, wmp_plist *plist);
extern bool wmp_is_composed(const wmp_plist *plist);

extern void destroy_wmp_packets(wmp_plist **plist);

#ifdef __cplusplus
}
#endif

#endif // wmp.h
