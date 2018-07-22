#ifndef _QPT_HEADER_WATER_METER_DATA_CONTEXT_H_
#define _QPT_HEADER_WATER_METER_DATA_CONTEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define WMD_SIZE_WM_ID      8
#define WMD_SIZE_HUB_ID     8

#define WMD_C2S_HEARTBEAT   0x01

#define WMD_S2C_HEARTBEAT   0x81

typedef struct wm_context {
    byte wm_id[WMD_SIZE_WM_ID];
    byte hub_id[WMD_SIZE_HUB_ID];

    uint8 type;

    uint dlen;
    byte *data;
} wm_cxt;

extern void wm_cxt_destroy(wm_cxt **cxt);

#ifdef __cplusplus
}
#endif

#endif // wm_data.h
