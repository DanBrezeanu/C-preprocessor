#ifndef __INT_TYPES_H__
#define __INT_TYPES_H__

#define ENOEXISTS 0x1

typedef unsigned char          uint8t;
typedef char                   int8t;
typedef unsigned short         uint16t;
typedef short                  int16t;
typedef unsigned int           uint32t;
typedef int                    int32t;
typedef unsigned long long int uint64t;
typedef long long int          int64t;

typedef enum Bool {
    false = 0,
    true = 1
} Bool;


#endif