#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "util/crc.h"
#include "test/test.h"

static const char *data = "CRC test data";
static const uint dlen = strlen(data);
static const uint16 data_crc16 = 0xA76A;

void test_crc16() {
    uint16 checksum = crc16((const byte *)data, dlen);
    printf("CRC-16/MODBUS value of [%s] is [%X].\n", data, checksum);

    assert(checksum == data_crc16);
}
