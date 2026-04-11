#include <assert.h>
#include <limits.h>
#include <string.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdckdint.h>

#include "vec.h"
#include "common.h"

void vec_init( Vec vec[ static 1 ], size_t elem_byte_size ) {
    assert( elem_byte_size != 0 );

    ( void ) memset( vec, 0, sizeof( Vec ) );
    vec->elem_byte_size = elem_byte_size;
}

void vec_clear( Vec vec[ static 1 ] ) {
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec->data != nullptr )
        free( vec->data );

    ( void ) memset( vec, 0, sizeof( Vec ) );
}

void vec_swap( Vec fst[ static 1 ], Vec snd[ static 1 ] ) {
    Vec tmp = *fst;
    *fst = *snd;
    *snd = tmp;
}

size_t vec_length( const Vec vec[ static 1 ] ) {
    return vec->length;
}

size_t vec_capacity( const Vec vec[ static 1 ] ) {
    return vec->capacity;
}

size_t vec_is_empty( const Vec vec[ static 1 ] ) {
    return vec_length( vec ) == 0;
}

static inline void prv_swap_mem( size_t size,
                                 void *tmp, void *fst, void *snd ) {
    assert( size != 0 );
    assert( tmp != nullptr );
    assert( fst != nullptr );
    assert( snd != nullptr );

    ( void ) memcpy( tmp, fst, size );
    ( void ) memcpy( fst, snd, size );
    ( void ) memcpy( snd, tmp, size );
}

static inline void prv_vec_idx_ptr( const Vec vec[ static 1 ],
                                    size_t idx,
                                    void *result[ static 1 ] ) {
    assert( vec->elem_byte_size != 0 );
    assert( vec->data != nullptr );

    assert( idx <= SIZE_MAX / vec->elem_byte_size );

    idx *= vec->elem_byte_size;
    *result = ( char * ) vec->data + idx;
}

static inline bool prv_vec_change_capacity( Vec vec[ static 1 ],
                                            size_t new_capacity ) {
    assert( vec->elem_byte_size != 0 );
    assert( vec->capacity == 0 || vec->data != nullptr );

    void *new_data = reallocarray(
        vec->data,
        new_capacity,
        vec->elem_byte_size
    );

    if ( new_data == nullptr )
        return true;

    vec->data = new_data;
    vec->capacity = new_capacity;
    return false;
}

static inline bool prv_vec_grow( Vec vec[ static 1 ] ) {
    enum : size_t { GROW = 2 };

    size_t new_capacity = vec->capacity + ( vec->capacity == 0 );
    if ( ckd_mul( &new_capacity, new_capacity, GROW ) )
        return true;

    return prv_vec_change_capacity( vec, new_capacity );
}

static inline void prv_vec_push( Vec vec[ static 1 ],
                                 const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( vec->elem_byte_size != 0 );

    assert( vec->length < vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    void *ptr;
    prv_vec_idx_ptr( vec, vec->length, &ptr );

    ( void ) memcpy( ptr, new_elem, vec->elem_byte_size );
    vec->length += 1;
}

static inline void prv_vec_pop( Vec vec[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( vec->elem_byte_size != 0 );

    assert( vec->length != 0 );
    assert( vec->length <= vec->capacity );
    assert( vec->data != nullptr );

    assert( vec->length - 1 <= SIZE_MAX / vec->elem_byte_size );

    void *ptr;
    prv_vec_idx_ptr( vec, vec->length - 1, &ptr );

    ( void ) memcpy( result, ptr, vec->elem_byte_size );
    vec->length -= 1;
}

void vec_idx_ptr( const Vec vec[ static 1 ], size_t idx,
                  void *result[ static 1 ] ) {
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( idx < vec->length );
    prv_vec_idx_ptr( vec, idx, result );
}

bool vec_at_ptr( const Vec vec[ static 1 ], size_t idx,
                 void *result[ static 1 ] ) {
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( idx >= vec->length )
        return true;

    vec_idx_ptr( vec, idx, result );
    return false;
}

void vec_idx( const Vec vec[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( idx < vec->length );
    assert( vec->elem_byte_size != 0 );

    void *ptr;
    vec_idx_ptr( vec, idx, &ptr );

    ( void ) memcpy( result, ptr, vec->elem_byte_size );
}

bool vec_at( const Vec vec[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( idx >= vec->length )
        return true;

    vec_idx( vec, idx, result );
    return false;
}

bool vec_swap_elem_idx( Vec vec[ static 1 ],
                        size_t idx_1, size_t idx_2 ) {
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( idx_1 >= vec->length || idx_2 >= vec->length );

    if ( idx_1 == idx_2 )
        return false;

    void *ptr_idx_1, *ptr_idx_2;
    prv_vec_idx_ptr( vec, idx_1, &ptr_idx_1 );
    prv_vec_idx_ptr( vec, idx_2, &ptr_idx_2 );

    const size_t elem_size = vec->elem_byte_size;
    assert( elem_size != 0 );

    if ( elem_size <= VEC_ELEM_SWAP_LIMIT ) {
        // WARNING: VLA
        unsigned char buffer[ elem_size ];
        prv_swap_mem( elem_size, buffer, ptr_idx_1, ptr_idx_2 );
    } else {
        void *buffer = malloc( elem_size );
        if ( buffer == nullptr )
            return true;

        prv_swap_mem( elem_size, buffer, ptr_idx_1, ptr_idx_2 );
        free( buffer );
    }

    return false;
}

bool vec_swap_elem_at( Vec vec[ static 1 ],
                       size_t idx_1, size_t idx_2 ) {
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( idx_1 >= vec->length || idx_2 >= vec->length )
        return true;

    return vec_swap_elem_idx( vec, idx_1, idx_2 );
}

bool vec_reserve( Vec vec[ static 1 ], size_t new_capacity ) {
    assert( vec->elem_byte_size != 0 );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( new_capacity <= vec->capacity )
        return false;

    return prv_vec_change_capacity( vec, new_capacity );
}

bool vec_resize( Vec vec[ static 1 ], size_t new_length ) {
    assert( vec->elem_byte_size != 0 );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec_reserve( vec, new_length ) )
        return true;

    if ( new_length > vec->length ) {
        const size_t elem_size = vec->elem_byte_size;

        assert( new_length <= SIZE_MAX / elem_size );

        const size_t offset = new_length * elem_size;
        const size_t diff = ( new_length - vec->length ) * elem_size;

        void *ptr;
        prv_vec_idx_ptr( vec, offset, &ptr );

        ( void ) memset( ptr, 0, diff );
    }

    vec->length = new_length;
    return false;
}

bool vec_push( Vec vec[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec->length == vec->capacity && prv_vec_grow( vec ) )
        return true;

    prv_vec_push( vec, new_elem );
    return false;
}

bool vec_try_push( Vec vec[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec->length == vec->capacity )
        return true;

    prv_vec_push( vec, new_elem );
    return false;
}

void vec_pop( Vec vec[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( vec->length != 0 );
    prv_vec_pop( vec, result );
}

bool vec_try_pop( Vec vec[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec->length == 0 )
        return true;

    prv_vec_pop( vec, result );
    return false;
}

bool vec_insert( Vec vec[ static 1 ],
                 size_t idx, const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( idx <= vec->length );

    if ( idx == vec->length )
        return vec_push( vec, new_elem );

    if ( vec->length == vec->capacity && prv_vec_grow( vec ) )
        return true;

    const size_t elem_size = vec->elem_byte_size;

    assert( elem_size != 0 );
    assert( vec->length - idx <= SIZE_MAX / elem_size );
    const size_t byte_size = ( vec->length - idx ) * elem_size;

    void *ptr, *ptr_next;
    vec_idx_ptr( vec, idx, &ptr );
    vec_idx_ptr( vec, idx + 1, &ptr );

    ( void ) memmove( ptr_next, ptr, byte_size );
    ( void ) memcpy( ptr, new_elem, elem_size );

    vec->length += 1;
    return false;
}

bool vec_try_insert( Vec vec[ static 1 ],
                     size_t idx, const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( vec->length == vec->capacity )
        return true;

    vec_insert( vec, idx, new_elem );
    return false;
}

void vec_remove( Vec vec[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    assert( idx < vec->length );

    if ( idx + 1 == vec->length )
        return vec_pop( vec, result );

    const size_t elem_size = vec->elem_byte_size;

    assert( elem_size != 0 );
    assert( vec->length - idx <= SIZE_MAX / elem_size );
    const size_t byte_size = ( vec->length - idx - 1 ) * elem_size;

    void *ptr, *ptr_next;
    vec_idx_ptr( vec, idx, &ptr );
    vec_idx_ptr( vec, idx + 1, &ptr_next );

    ( void ) memcpy( result, ptr, vec->elem_byte_size );
    ( void ) memmove( ptr, ptr_next, byte_size );

    vec->length -= 1;
}

bool vec_try_remove( Vec vec[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( vec->length <= vec->capacity );
    assert( vec->capacity == 0 || vec->data != nullptr );

    if ( idx >= vec->length )
        return true;

    vec_remove( vec, idx, result );
    return true;
}

bool vec_reverse( Vec vec[ static 1 ] ) {
    const size_t length = vec->length;
    const size_t half = length / 2;

    // NOTE: Should optimize and avoid repeated allocations
    //       — use a only one single temporary buffer for swaps.
    for ( size_t i = 0; i < half; ++i )
        if ( vec_swap_elem_idx( vec, i, length - i - 1 ) )
            return true;

    return false;
}

bool vec_extend( Vec to[ static 1 ], const Vec from[ static 1 ] ) {
    assert( to->elem_byte_size == from->elem_byte_size );

    size_t new_capacity;
    if ( ckd_add( &new_capacity, to->capacity, from->capacity ) )
        return true;

    if ( vec_reserve( to, new_capacity ) )
        return true;

    for ( size_t i = 0; i < from->capacity; ++i ) {
        void *ptr;
        vec_idx_ptr( from, i, &ptr );

        ( void ) vec_try_push( to, ptr );
    }

    return false;
}

VecIter vec_iter_begin( const Vec vec[ static 1 ] ) {
    return ( VecIter ){ .vec = vec, .curr_idx = 0 };
}

VecIter vec_iter_end( const Vec vec[ static 1 ] ) {
    return ( VecIter ){ .vec = vec, .curr_idx = vec->length };
}

bool vec_iter_curr( VecIter it[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( it->vec != nullptr );
    assert( it->vec->elem_byte_size != 0 );

    assert( it->curr_idx <= it->vec->length );

    if ( it->curr_idx == it->vec->length )
        return true;

    void *ptr;
    vec_idx_ptr( it->vec, it->curr_idx, &ptr );
    ( void ) memcpy( result, ptr, it->vec->elem_byte_size );

    return false;
}

void vec_iter_next( VecIter it[ static 1 ] ) {
    assert( it->curr_idx <= it->vec->length );
    it->curr_idx += it->vec->length > it->curr_idx;
}

void vec_iter_prev( VecIter it[ static 1 ] ) {
    assert( it->curr_idx <= it->vec->length );
    it->curr_idx -= 0 != it->curr_idx;
}

static inline size_t partition( Vec vec[ static 1],
                               size_t l, size_t r,
                               CmpFn fn,
                               void *tmp ) {
    assert( tmp != nullptr );
    assert( l <= r );
    assert( r < vec->length );

    void *pivot;
    vec_idx_ptr( vec, r, &pivot );

    size_t last = l;

    for ( size_t i = 1; i < r; ++i ) {
        void *elem;
        vec_idx_ptr( vec, i, &elem );

        if ( fn( elem, pivot ) != Greater ) {
            void *last_elem;
            vec_idx_ptr( vec, last, &last_elem );

            prv_swap_mem( vec->elem_byte_size, tmp, elem, last_elem );
            last += 1;
        }
    }

    void *last_elem;
    vec_idx_ptr( vec, last, &last_elem );
    prv_swap_mem( vec->elem_byte_size, tmp, pivot, last_elem );
}

static inline void quick_sort( Vec vec[ static 1],
                               size_t l, size_t r,
                               CmpFn fn,
                               void *tmp ) {
    assert( tmp != nullptr );

    if ( l < r ) {
        size_t mid = partition( vec, l, r, fn, tmp );

        size_t limit;
        if ( ckd_sub( &limit, mid, 1 ) )
            quick_sort( vec, l, limit, fn, tmp );

        if ( ckd_add( &limit, mid, 1 ) )
            quick_sort( vec, limit, r, fn, tmp );
    }
}

bool vec_sort( Vec vec[ static 1 ], CmpFn fn ) {
    if ( vec->length <= 1 )
        return false;

    const size_t elem_size = vec->elem_byte_size;

    if ( elem_size <= VEC_ELEM_SWAP_LIMIT ) {
        // WARNING: VLA
        unsigned char buffer[ elem_size ];
        quick_sort( vec, 0, vec->length - 1, fn, buffer );

    } else {
        void *buffer = malloc( elem_size );
        if ( buffer == nullptr )
            return true;

        quick_sort( vec, 0, vec->length - 1, fn, buffer );

        free( buffer );
    }
}

bool vec_index( Vec vec[ static 1 ],
                const void *elem, size_t *result ) {
}

bool vec_count( Vec vec[ static 1 ],
                const void *elem, size_t *result ) {

}
