#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util/macros.h"
#include "buffer/buffer.h"

CircularBuffer::CircularBuffer(uint init_size) {
    max_size = MAX(init_size, BUFFER_SIZE_MIN);
    buf = (byte *)malloc(sizeof(byte) * ALIGN_SIZE(max_size + 1));
    front = 0, rear = 0;
}

CircularBuffer::~CircularBuffer() {
    free(buf);
}

uint CircularBuffer::buf_size() const {
    return max_size;
}

uint CircularBuffer::data_size() const {
    return (rear + max_size + 1 - front) % (max_size + 1);
}

uint CircularBuffer::free_size() const {
    return (front + max_size - rear) % (max_size + 1);
}

uint CircularBuffer::peek(byte *dst, uint dlen) const {
    uint len = MIN(data_size(), dlen);

    uint llen = MIN(max_size + 1 - front, len);
    memcpy(dst, buf + front, sizeof(byte) * llen);

    uint rlen = len - llen;
    memcpy(dst + llen, buf, sizeof(byte) * rlen);

    return len;
}

uint CircularBuffer::read(byte *dst, uint dlen) {
    uint len = peek(dst, dlen);
    front = (front + len) % (max_size + 1);

    return len;
}

uint CircularBuffer::write(const byte *src, uint slen) {
    if (free_size() < slen) {
        expand(data_size() + slen);
    }

    uint rlen = MIN(max_size + 1 - rear, slen);
    memcpy(buf + rear, src, sizeof(byte) * rlen);

    uint llen = slen - rlen;
    memcpy(buf, src + rlen, sizeof(byte) * llen);

    rear = (rear + slen) % (max_size + 1);

    return slen;
}

void CircularBuffer::expand(uint min_size) {
    uint ex_size = max_size;
    while (ex_size < min_size) {
        ex_size *= 2;
    }

    byte *new_buf = (byte *)malloc(sizeof(byte) * ALIGN_SIZE(ex_size + 1));
    uint len = peek(new_buf, ex_size);

    free(buf);
    
    buf = new_buf;
    max_size = ex_size;
    front = 0, rear = len;
}
