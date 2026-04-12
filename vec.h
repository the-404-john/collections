#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#include "common.h"

constexpr size_t VEC_ELEM_SWAP_LIMIT = 4096;

typedef struct Vec {
    void *data;
    size_t elem_byte_size;

    size_t length;
    size_t capacity;
} Vec;

typedef struct VecIter {
    const Vec *vec;
    size_t curr_idx;
} VecIter;

void vec_init( Vec vec[ static 1 ], size_t elem_byte_size );
void vec_clear( Vec vec[ static 1 ] );
void vec_swap( Vec fst[ static 1 ], Vec snd[ static 1 ] );

size_t vec_length( const Vec vec[ static 1 ] );
size_t vec_capacity( const Vec vec[ static 1 ] );
size_t vec_is_empty( const Vec vec[ static 1 ] );

void vec_idx_ptr( const Vec vec[ static 1 ], size_t idx,
                  void *result[ static 1 ] );

bool vec_at_ptr( const Vec vec[ static 1 ], size_t idx,
                 void *result[ static 1 ] );

void vec_idx( const Vec vec[ static 1 ], size_t idx, void *result );
bool vec_at( const Vec vec[ static 1 ], size_t idx, void *result );

bool vec_swap_elem_idx( Vec vec[ static 1 ],
                        size_t idx_1, size_t idx_2 );

bool vec_swap_elem_at( Vec vec[ static 1 ],
                       size_t idx_1, size_t idx_2 );

bool vec_reserve( Vec vec[ static 1 ], size_t new_capacity );
bool vec_resize( Vec vec[ static 1 ], size_t new_length );

bool vec_push( Vec vec[ static 1 ], const void *new_elem );
bool vec_try_push( Vec vec[ static 1 ], const void *new_elem );

void vec_pop( Vec vec[ static 1 ], void *result );
bool vec_try_pop( Vec vec[ static 1 ], void *result );

bool vec_insert( Vec vec[ static 1 ],
                 size_t idx, const void *new_elem );
bool vec_try_insert( Vec vec[ static 1 ],
                     size_t idx, const void *new_elem );

void vec_remove( Vec vec[ static 1 ], size_t idx, void *result );
bool vec_try_remove( Vec vec[ static 1 ], size_t idx, void *result );

bool vec_copy( const Vec from[ static 1 ], Vec to[ static 1 ] );

// Algorithms
bool vec_sort( Vec vec[ static 1 ], CmpFn fn );

size_t vec_index( const Vec vec[ static 1 ],
                  size_t start, const void *elem );

size_t vec_count( const Vec vec[ static 1 ], const void *elem );

bool vec_contains( const Vec vec[ static 1 ], const void *elem );

// Iterator
VecIter vec_iter_begin( const Vec vec[ static 1 ] );
VecIter vec_iter_end( const Vec vec[ static 1 ] );

void vec_iter_next( VecIter it[ static 1 ] );
void vec_iter_prev( VecIter it[ static 1 ] );
bool vec_iter_curr( VecIter it[ static 1 ], void *result );

#endif // VEC_H
