#ifndef _QPT_HEADER_TEST_H_
#define _QPT_HEADER_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void test_crc16();

extern void test_block_buffer();
extern void test_circular_buffer();

static inline void test_all() { 
    test_crc16();

    test_block_buffer();
    test_circular_buffer();
}

#ifdef __cplusplus
}
#endif

#endif // test.h
