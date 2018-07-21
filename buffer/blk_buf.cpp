#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util/macros.h"
#include "buffer/buffer.h"

BlockBuffer::BlockBuffer(uint init_size) {
    max_size = MAX(init_size, BUFFER_SIZE_MIN);
    buf = (byte *)malloc(sizeof(byte) * ALIGN_SIZE(max_size));
    size = 0;
}

BlockBuffer::~BlockBuffer() {
    free(buf);
}

uint BlockBuffer::buf_size() const {
    return max_size;
}

uint BlockBuffer::data_size() const {
    return size;
}

uint BlockBuffer::free_size() const {
    return max_size - size;
}

uint BlockBuffer::peek(byte *dst, uint dlen) const {
    uint len = MIN(size, dlen);

    memcpy(dst, buf, sizeof(byte) * len);
    return len;
}

uint BlockBuffer::read(byte *dst, uint dlen) {
    uint len = MIN(size, dlen);

    memcpy(dst, buf, sizeof(byte) * len);

    size -= len;
    memcpy(buf, buf + len, sizeof(byte) * size);

    return len;
}

uint BlockBuffer::write(const byte *src, uint slen) {
    if (size + slen > max_size) {
        expand(size + slen);
    }

    memcpy(buf + size, src, sizeof(byte) * slen);
    size += slen;

    return slen;
}

void BlockBuffer::expand(uint min_size) {
    while (max_size < min_size) {
        max_size *= 2;
    }

    buf = (byte *)realloc(buf, sizeof(byte) * ALIGN_SIZE(max_size));
}
