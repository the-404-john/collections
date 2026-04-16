#include <assert.h>
#include <string.h>

#include <stdlib.h>
#include <stdckdint.h>

#include "max_heap.h"

static size_t prv_parent( size_t idx, size_t result[ static 1 ] ) {
    if ( idx == 0 )
        return false;

    *result = ( idx - 1 ) / 2;
    return true;
}

static bool prv_left_child( size_t idx, size_t result[ static 1 ] ) {
    size_t left_idx;

    if ( ckd_mul( &left_idx, idx, 2 ) )
        return false;

    if ( ckd_add( &left_idx, left_idx, 1 ) )
        return false;

    *result = left_idx;
    return true;
}

static size_t prv_right_child( size_t idx, size_t result[ static 1 ] ) {
    size_t right_idx;

    if ( ckd_mul( &right_idx, idx, 2 ) )
        return false;

    if ( ckd_add( &right_idx, right_idx, 2 ) )
        return false;

    *result = right_idx;
    return true;
}

static void prv_heapify_up( MaxHeap heap[ static 1 ],
                            void *tmp, size_t idx ) {
    assert( tmp != nullptr );

    const size_t elem_size = heap->vec.elem_byte_size;
    assert( elem_size != 0 );

    while ( idx > 0 ) {
        size_t parent_idx;
        ( void ) prv_parent( idx, &parent_idx );

        void *curr_ptr, *parent_ptr;
        vec_idx_ptr( &heap->vec, idx, &curr_ptr );
        vec_idx_ptr( &heap->vec, parent_idx, &parent_ptr );

        if ( heap->fn( curr_ptr, parent_ptr ) != Greater )
            break;

        vec_buf_swap_elem_idx( &heap->vec, tmp, parent_idx, idx );
        idx = parent_idx;
    }
}

static void prv_heapify_down( MaxHeap heap[ static 1 ],
                              void *tmp, size_t idx ) {
    assert( tmp != nullptr );

    const size_t length = vec_length( &heap->vec );
    assert( idx < length );

    const size_t elem_size = heap->vec.elem_byte_size;
    assert( elem_size != 0 );

    while ( true ) {
        size_t largest_idx = idx;

        size_t left_idx;
        if ( prv_left_child( idx, &left_idx ) && left_idx < length ) {
            void *left_ptr, *largest_ptr;
            vec_idx_ptr( &heap->vec, left_idx, &left_ptr );
            vec_idx_ptr( &heap->vec, largest_idx, &largest_ptr );

            if ( heap->fn( left_ptr, largest_ptr ) == Greater )
                largest_idx = left_idx;
        }

        size_t right_idx;
        if ( prv_right_child( idx, &right_idx ) && right_idx < length ) {
            void *right_ptr, *largest_ptr;
            vec_idx_ptr( &heap->vec, right_idx, &right_ptr );
            vec_idx_ptr( &heap->vec, largest_idx, &largest_ptr );

            if ( heap->fn( right_ptr, largest_ptr ) == Greater )
                largest_idx = right_idx;
        }

        if ( largest_idx == idx )
            break;

        vec_buf_swap_elem_idx( &heap->vec, tmp, largest_idx, idx );
        idx = largest_idx;
    }
}

static bool prv_heapify_push( MaxHeap heap[ static 1 ] ) {
    const size_t elem_size = heap->vec.elem_byte_size;
    assert( elem_size != 0 );

    if ( elem_size <= VEC_ELEM_SWAP_LIMIT ) {
        // WARNING: VLA
        unsigned char buffer[ elem_size ];
        prv_heapify_up( heap, buffer, vec_length( &heap->vec) - 1 );
    } else {
        void *buffer = malloc( elem_size );
        if ( !buffer ) {
            ( void ) vec_empty_pop( &heap->vec );
            return false;
        }

        prv_heapify_up( heap, buffer, vec_length( &heap->vec) - 1 );
        free( buffer );
    }

    return true;
}

void max_heap_init( MaxHeap heap[ static 1 ],
                    size_t elem_byte_size, CmpFn fn ) {
    assert( elem_byte_size != 0 );

    vec_init( &heap->vec, elem_byte_size );
    heap->fn = fn;
}

bool max_heap_build( MaxHeap heap[ static 1 ],
                     const Vec vec[ static 1 ], CmpFn fn ) {
    assert( vec->elem_byte_size != 0 );

    MaxHeap tmp;
    max_heap_init( &tmp, vec->elem_byte_size, fn );

    if ( !vec_reserve( &tmp.vec, vec->length ) )
        return false;

    for ( size_t i = 0; i < vec->length; ++i ) {
        void *ptr;
        vec_idx_ptr( vec, i, &ptr );

        ( void ) vec_try_push( &tmp.vec, ptr );
    }

    *heap = tmp;
    return true;
}

void max_heap_clear( MaxHeap heap[ static 1 ] ) {
    vec_clear( &heap->vec );
    ( void ) memset( heap, 0, sizeof( MaxHeap ) );
}

void max_heap_swap( MaxHeap fst[ static 1 ], MaxHeap snd[ static 1 ] ) {
    MaxHeap tmp = *fst;
    *fst = *snd;
    *snd = tmp;
}

size_t max_heap_length( const MaxHeap heap[ static 1 ] ) {
    return vec_length( &heap->vec );
}

size_t max_heap_capacity( const MaxHeap heap[ static 1 ] ) {
    return vec_capacity( &heap->vec );
}

size_t max_heap_is_empty( const MaxHeap heap[ static 1 ] ) {
    return vec_is_empty( &heap->vec );
}

bool max_heap_peek( const MaxHeap heap[ static 1 ], void *result ) {
    assert( result != nullptr );

    if ( max_heap_length( heap ) == 0 )
        return false;

    void *ptr;
    vec_idx_ptr( &heap->vec, 0, &ptr );

    assert( heap->vec.elem_byte_size != 0 );
    ( void ) memcpy( result, ptr, heap->vec.elem_byte_size );

    return true;
}

bool max_heap_copy_peek( const MaxHeap heap[ static 1 ],
                         void *result[ static 1 ] ) {
    if ( max_heap_length( heap ) == 0 )
        return false;

    assert( heap->vec.elem_byte_size != 0 );

    void *new_data = malloc( heap->vec.elem_byte_size );
    if ( new_data == nullptr )
        return false;

    void *ptr;
    vec_idx_ptr( &heap->vec, 0, &ptr );

    ( void ) memcpy( new_data, ptr, heap->vec.elem_byte_size );

    *result = new_data;
    return true;
}

bool max_heap_reserve( MaxHeap heap[ static 1 ], size_t new_capacity ) {
    return vec_reserve( &heap->vec, new_capacity );
}

bool max_heap_push( MaxHeap heap[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );

    if ( !vec_push( &heap->vec, new_elem ) )
        return false;

    assert( vec_length( &heap->vec) != 0 );
    return prv_heapify_push( heap );
}

bool max_heap_try_push( MaxHeap heap[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );

    if ( !vec_try_push( &heap->vec, new_elem ) )
        return false;

    assert( vec_length( &heap->vec) != 0 );
    return prv_heapify_push( heap );
}

void max_heap_pop( MaxHeap heap[ static 1 ], void *result ) {
    assert( result != nullptr );

    const size_t length = vec_length( &heap->vec );
    assert( length != 0 );

    const size_t elem_size = heap->vec.elem_byte_size;
    assert( elem_size != 0 );

    if ( length == 1 ) {
        vec_pop( &heap->vec, result );
        return;
    }

    void *fst, *lst;
    vec_idx_ptr( &heap->vec, 0, &fst );
    vec_idx_ptr( &heap->vec, length - 1, &lst );

    ( void ) memcpy( result, fst, elem_size );
    ( void ) memcpy( fst, lst, elem_size );

    vec_empty_pop( &heap->vec );
    prv_heapify_down( heap, lst, 0 );
}

bool max_heap_try_pop( MaxHeap heap[ static 1 ], void *result ) {
    assert( result != nullptr );

    if ( vec_length( &heap->vec ) == 0 )
        return false;

    max_heap_pop( heap, result );
    return true;
}

void max_heap_empty_pop( MaxHeap heap[ static 1 ] ) {
    vec_empty_pop( &heap->vec );
}

bool max_heap_copy( const MaxHeap from[ static 1 ],
                    MaxHeap to[ static 1 ] ) {
    Vec tmp;
    vec_init( &tmp, from->vec.elem_byte_size );

    if ( !vec_copy( &from->vec, &tmp ) )
        return false;

    max_heap_init( to, from->vec.elem_byte_size, from->fn );
    to->vec = tmp;
    return true;
}
