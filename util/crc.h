#ifndef _QPT_HEADER_CHECK_SUM_CRC_H_
#define _QPT_HEADER_CHECK_SUM_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

// CRC-16/MODBUS
extern uint16 crc16(const byte *data, uint dlen);

#ifdef __cplusplus
}
#endif

#endif // crc.h
