#ifndef _QPT_HEADER_TEST_H_
#define _QPT_HEADER_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void test_crc16();

extern void test_block_buffer();
extern void test_circular_buffer();

extern void test_single_client_server();
extern void test_multi_client_server();

extern void test_wm_server();

static inline void test_all() { 
    test_crc16();

    test_block_buffer();
    test_circular_buffer();

    test_single_client_server();
    test_multi_client_server();

    test_wm_server();
}

#ifdef __cplusplus
}
#endif

#endif // test.h
