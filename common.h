#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef enum StrongOrdering : int8_t {
    Lesser  = -1,
    Equal   =  0,
    Greater =  1,
} PartialOrdering;

typedef bool (*CmpFn)( const void *fst, const void *snd );

#endif // COMMON_H
