#ifndef _QPT_HEADER_BUFFER_H_
#define _QPT_HEADER_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#include <stdlib.h>
#include <string.h>

#define BUF_SIZE_1K     1024
#define BUF_SIZE_2K     (BUF_SIZE_1K * 2)
#define BUF_SIZE_4K     (BUF_SIZE_1K * 4)
#define BUF_SIZE_8K     (BUF_SIZE_1K * 8)
#define BUF_SIZE_16K    (BUF_SIZE_1K * 16)

#define BUF_SIZE_PAGE   BUF_SIZE_4K

extern typedef struct _circular_buffer {
    void *data;
    uint max_size;

    uint head;
    uint tail;

    bool empty;
} CirBuffer;

extern inline uint cb_calc_free_size(const CirBuffer *buf) {
    return (buf->empty ? buf->max_size :
            (buf->head - buf->tail + buf->max_size) % buf->max_size);
}

extern inline uint cb_calc_data_size(const CirBuffer *buf) {
    return buf->max_size - cb_calc_free_size(buf);
}

extern void cb_init_buf(CirBuffer *buf, int size);
extern void cb_destory_buf(CirBuffer *buf);
extern void cb_clear_buf(CirBuffer *buf);

#ifdef __cplusplus
}
#endif

#endif // buffer.h
