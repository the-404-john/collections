#ifndef VEC_SLICE_H
#define VEC_SLICE_H

#include <stddef.h>

#include "vec.h"
#include "common.h"

typedef struct VecSlice {
    const Vec *vec;
    size_t start;
    size_t end;
} VecSlice;

typedef struct VecSliceIter {
    const VecSlice *slice;
    size_t curr_idx;
} VecSliceIter;

VecSlice vec_slice_init( const Vec vec[ static 1 ],
                         size_t start, size_t end );

VecSlice vec_slice_sub( const VecSlice slice[ static 1 ],
                        size_t start_offset, size_t end_offset );

size_t vec_slice_length( const VecSlice slice[ static 1 ] );
size_t vec_slice_is_empty( const VecSlice slice[ static 1 ] );

void vec_slice_idx( const VecSlice slice[ static 1 ],
                    size_t idx, void *result );

bool vec_slice_at( const VecSlice slice[ static 1 ],
                   size_t idx, void *result );

void vec_slice_pop_front( VecSlice slice[ static 1 ], void *result );
void vec_slice_pop_back( VecSlice slice[ static 1 ], void *result );

bool vec_slice_try_pop_front( VecSlice slice[ static 1 ], void *result );
bool vec_slice_try_pop_back( VecSlice slice[ static 1 ], void *result );

void vec_slice_empty_pop_front( VecSlice slice[ static 1 ] );
void vec_slice_empty_pop_back( VecSlice slice[ static 1 ] );

void vec_slice_copy( const VecSlice from[ static 1 ],
                     VecSlice to[ static 1 ] );

// Algorithms
size_t vec_slice_index( const VecSlice slice[ static 1 ],
                        size_t start, const void *elem );

size_t vec_slice_count( const VecSlice slice[ static 1 ],
                        const void *elem );

bool vec_slice_contains( const VecSlice slice[ static 1 ],
                         const void *elem );

// Iterator
VecSliceIter vec_slice_iter_begin( const VecSlice slice[ static 1 ] );
VecSliceIter vec_slice_iter_end( const VecSlice slice[ static 1 ] );

void vec_slice_iter_next( VecSliceIter it[ static 1 ] );
void vec_slice_iter_prev( VecSliceIter it[ static 1 ] );
bool vec_slice_iter_curr( VecSliceIter it[ static 1 ], void *result );

#endif // VEC_SLICE_H
