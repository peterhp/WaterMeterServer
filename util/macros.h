#ifndef _QPT_HEADER_SELF_DEFINED_MACROS_H_
#define _QPT_HEADER_SELF_DEFINED_MACROS_H_

#include "types.h"

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef DIV_FLOOR
#define DIV_FLOOR(dividend, divisor) ((dividend) / (divisor))
#endif

#ifndef DIV_CEIL
#define DIV_CEIL(dividend, divisor) \
    ((dividend) / (divisor) + ((dividend) % (divisor) ? 1 : 0))
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#ifndef ALIGN_WIDTH
#define ALIGN_WIDTH sizeof(long)
#endif

#ifndef ALIGN_SIZE
#define ALIGN_SIZE(size) (ALIGN_WIDTH * DIV_CEIL(size, ALIGN_WIDTH))
#endif

#include <time.h>

#ifndef CUR_TIME
#define CUR_TIME() ((uint32)time(NULL))
#endif

#endif // macros.h
