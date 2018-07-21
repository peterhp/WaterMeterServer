#ifndef _QPT_HEADER_DEFINED_TYPES_H_
#define _QPT_HEADER_DEFINED_TYPES_H_

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;

typedef uint8               byte;
typedef uint16              word;
typedef uint32              dword;
typedef uint64              qword;

#ifndef __cplusplus
typedef byte bool;
#define true    ((byte)1)
#define false   ((byte)0)
#endif

#endif // types.h
