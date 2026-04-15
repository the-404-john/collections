#include <assert.h>
#include <limits.h>
#include <string.h>

#include <stdckdint.h>

#include "vec_slice.h"

VecSlice vec_slice_init( const Vec vec[ static 1 ],
                         size_t start, size_t end ) {
    assert( start <= end );
    assert( vec->elem_byte_size != 0 );

    return ( VecSlice ){
        .vec = vec,
        .start = start,
        .end = end
    };
}

VecSlice vec_slice_sub( const VecSlice slice[ static 1 ],
                        size_t start_offset, size_t end_offset ) {
    assert( start_offset <= end_offset );
    assert( start_offset <= SIZE_MAX - slice->start );
    assert( end_offset <= SIZE_MAX - slice->end );

    assert( slice->vec != nullptr );

    return ( VecSlice ){
        .vec = slice->vec,
        .start = slice->start + start_offset,
        .end = slice->end + end_offset
    };
}

size_t vec_slice_length( const VecSlice slice[ static 1 ] ) {
    assert( slice->end >= slice->start );
    return slice->end - slice->start;
}

size_t vec_slice_is_empty( const VecSlice slice[ static 1 ] ) {
    return vec_slice_length ( slice ) == 0;
}

void vec_slice_idx( const VecSlice slice[ static 1 ],
                    size_t idx, void *result ) {
    assert( result != nullptr );
    assert( slice->vec != nullptr );
    assert( idx < vec_slice_length( slice ) );

    assert( idx <= SIZE_MAX - slice->start );

    void *ptr;
    vec_idx_ptr( slice->vec, slice->start + idx, &ptr );

    assert( slice->vec->elem_byte_size != 0 );
    ( void ) memcpy( result, ptr, slice->vec->elem_byte_size );
}

bool vec_slice_at( const VecSlice slice[ static 1 ],
                   size_t idx, void *result ) {
    assert( result != nullptr );

    if ( idx >= vec_slice_length( slice ) )
        return false;

    if ( ckd_add( &idx, slice->start, idx ) )
        return false;

    vec_slice_idx( slice, idx, result );
    return true;
}

void vec_slice_pop_front( VecSlice slice[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( slice->start < slice->end );

    void *ptr;
    vec_idx_ptr( slice->vec, slice->start, &ptr );

    assert( slice->vec->elem_byte_size != 0 );
    ( void ) memcpy( result, ptr, slice->vec->elem_byte_size );

    slice->start += 1;
}

void vec_slice_pop_back( VecSlice slice[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( slice->vec != nullptr );
    assert( slice->start < slice->end );

    void *ptr;
    vec_idx_ptr( slice->vec, slice->end, &ptr );

    assert( slice->vec->elem_byte_size != 0 );
    ( void ) memcpy( result, ptr, slice->vec->elem_byte_size );

    slice->end -= 1;
}

bool vec_slice_try_pop_front( VecSlice slice[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( slice->start <= slice->end );

    if ( slice->start == slice->end )
        return false;

    vec_slice_pop_front( slice, result );
    return true;
}

bool vec_slice_try_pop_back( VecSlice slice[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( slice->start <= slice->end );

    if ( slice->start == slice->end )
        return false;

    vec_slice_pop_back( slice, result );
    return true;
}

void vec_slice_empty_pop_front( VecSlice slice[ static 1 ] ) {
    assert( slice->start < slice->end );
    slice->start += 1;
}

void vec_slice_empty_pop_back( VecSlice slice[ static 1 ] ) {
    assert( slice->start < slice->end );
    slice->end -= 1;
}

void vec_slice_copy( const VecSlice from[ static 1 ],
                     VecSlice to[ static 1 ] ) {
    assert( from->vec != nullptr );
    assert( from->end >= from->start );

    *to = *from;
}

size_t vec_slice_index( const VecSlice slice[ static 1 ],
                        size_t start, const void *elem ) {
    assert( elem != nullptr );
    assert( slice->vec != nullptr );

    assert( slice->start <= slice->end );
    assert( slice->end <= vec_length( slice->vec ) );

    const size_t length = vec_slice_length( slice );

    const size_t elem_size = slice->vec->elem_byte_size;
    assert( elem_size != 0 );

    for ( size_t i = start; i < length; ++i ) {
        size_t idx;
        if ( ckd_add( &idx, slice->start, i ) )
            break;

        void *ptr;
        vec_idx_ptr( slice->vec, idx, &ptr );

        if ( memcmp( ptr, elem, elem_size ) == 0 )
            return i;
    }

    return length;
}

size_t vec_slice_count( const VecSlice slice[ static 1 ],
                        const void *elem ) {
    assert( elem != nullptr );
    assert( slice->vec != nullptr );

    assert( slice->start <= slice->end );
    assert( slice->end <= vec_length( slice->vec ) );

    const size_t elem_size = slice->vec->elem_byte_size;
    assert( elem_size != 0 );

    size_t cnt = 0;

    for ( size_t i = slice->start; i < slice->end; ++i ) {
        void *ptr;
        vec_idx_ptr( slice->vec, i, &ptr );

        cnt += memcmp( ptr, elem, elem_size ) == 0;
    }

    return cnt;
}

bool vec_slice_contains( const VecSlice slice[ static 1 ],
                         const void *elem ) {
    assert( elem != nullptr );
    assert( slice->vec != nullptr );

    assert( slice->start <= slice->end );
    assert( slice->end <= vec_length( slice->vec ) );

    const size_t elem_size = slice->vec->elem_byte_size;
    assert( elem_size != 0 );

    for ( size_t i = slice->start; i < slice->end; ++i ) {
        void *ptr;
        vec_idx_ptr( slice->vec, i, &ptr );

        if ( memcmp( ptr, elem, elem_size ) == 0 )
            return true;
    }

    return false;
}

VecSliceIter vec_slice_iter_begin( const VecSlice slice[ static 1 ] ) {
    assert( slice->vec != nullptr );
    assert( slice->start <= slice->end );

    return ( VecSliceIter ){ .slice = slice, .curr_idx = slice->start };
}

VecSliceIter vec_slice_iter_end( const VecSlice slice[ static 1 ] ) {
    assert( slice->vec != nullptr );
    assert( slice->start <= slice->end );

    return ( VecSliceIter ){ .slice = slice, .curr_idx = slice->end };
}

void vec_slice_iter_next( VecSliceIter it[ static 1 ] ) {
    assert( it->curr_idx < it->slice->end );
    it->curr_idx += 1;
}

void vec_slice_iter_prev( VecSliceIter it[ static 1 ] ) {
    assert( it->curr_idx > it->slice->start );
    it->curr_idx -= 1;
}

bool vec_slice_iter_curr( VecSliceIter it[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( it->slice->vec != nullptr );

    if ( it->slice->start > it->curr_idx )
        return false;

    if ( it->curr_idx >= it->slice->end )
        return false;

    const size_t elem_size = it->slice->vec->elem_byte_size;
    assert( elem_size != 0 );

    void *ptr;
    vec_idx_ptr( it->slice->vec, it->curr_idx, &ptr );

    ( void ) memcpy( result, ptr, elem_size );

    return true;
}

