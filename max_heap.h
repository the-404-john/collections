#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <stddef.h>

#include "vec.h"
#include "common.h"

typedef struct MaxHeap {
    Vec vec;
    CmpFn fn;
} MaxHeap;

void max_heap_init( MaxHeap heap[ static 1 ],
                    size_t elem_byte_size, CmpFn fn );

void max_heap_clear( MaxHeap heap[ static 1 ] );
void max_heap_swap( MaxHeap fst[ static 1 ], MaxHeap snd[ static 1 ] );

size_t max_heap_length( const MaxHeap heap[ static 1 ] );
size_t max_heap_capacity( const MaxHeap heap[ static 1 ] );
size_t max_heap_is_empty( const MaxHeap heap[ static 1 ] );

bool max_heap_peek( const MaxHeap heap[ static 1 ], void *result );
bool max_heap_copy_peek( const MaxHeap heap[ static 1 ],
                         void *result[ static 1 ] );

bool max_heap_reserve( MaxHeap heap[ static 1 ], size_t new_capacity );

bool max_heap_push( MaxHeap heap[ static 1 ], const void *new_elem );
bool max_heap_try_push( MaxHeap heap[ static 1 ], const void *new_elem );

void max_heap_pop( MaxHeap heap[ static 1 ], void *result );
bool max_heap_try_pop( MaxHeap heap[ static 1 ], void *result );
void max_heap_empty_pop( MaxHeap heap[ static 1 ] );

bool max_heap_copy( const MaxHeap from[ static 1 ],
                    MaxHeap to[ static 1 ] );

#endif // MAX_HEAP_H
