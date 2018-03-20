#ifndef _QPT_HEADER_SELF_DEFINED_TYPES_H_
#define _QPT_HEADER_SELF_DEFINED_TYPES_H_

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long long  ulong;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef uint8   byte;
typedef uint16  word;
typedef uint32  dword;
typedef uint64  qword;

#ifndef __cplusplus
typedef uint32  bool;

#define true    1
#define false   0
#endif

#endif // types.h
