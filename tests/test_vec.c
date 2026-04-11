#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../vec.h"

Test( vec, init_clear_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );
    vec_clear( &vec );
}

Test( vec, init_clear_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );
    vec_clear( &vec );

    vec_init( &vec, sizeof( int64_t ) );
    vec_clear( &vec );

    vec_init( &vec, sizeof( int8_t ) );
    vec_clear( &vec );
}

Test( vec, init_clear_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );
    vec_clear( &vec );
    vec_clear( &vec );
}

Test( vec, length, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );

    vec_clear( &vec );
}

Test( vec, capacity, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_capacity( &vec ), 0 );

    vec_clear( &vec );
}

Test( vec, is_empty_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert( vec_is_empty( &vec ) );

    vec_clear( &vec );
}

Test( vec, push_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    vec_clear( &vec );
}

Test( vec, push_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 3 );
    cr_assert( vec_capacity( &vec ) >= 3 );

    vec_clear( &vec );
}

Test( vec, push_pop_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, push_pop_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, push_pop_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in_1 = 42;
    const int32_t in_2 = 69;
    const int32_t in_3 = 6174;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in_1 ) );
    cr_assert_not( vec_push( &vec, &in_2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 69 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_push( &vec, &in_3 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 6174 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, push_pop_04, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in_1 = 42;
    const int32_t in_2 = 69;
    const int32_t in_3 = 6174;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in_1 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    cr_assert_not( vec_push( &vec, &in_2 ) );
    cr_assert_not( vec_push( &vec, &in_3 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 6174 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 0;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 69 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, push_pop_05, .timeout = 1 ) {
    enum : int32_t { limit = 64 };

    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    for ( int32_t i = 0; i < limit; ++i ) {
        cr_assert_not( vec_push( &vec, &i ) );

        cr_assert_eq( vec_length( &vec ), i + 1 );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i + 1 );
    }

    for ( int32_t i = limit; i > 0; --i ) {
        cr_assert_eq( vec_length( &vec ), i );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i );

        int32_t out;
        vec_pop( &vec, &out );
        cr_assert_eq( out, i - 1 );

        cr_assert_eq( vec_length( &vec ), i - 1 );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i - 1 );
    }

    vec_clear( &vec );
}

Test( vec, push_pop_06, .timeout = 1 ) {
    enum : int32_t { limit = 64 };

    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    for ( int32_t i = 0; i < limit; ++i ) {
        cr_assert_not( vec_push( &vec, &i ) );
        cr_assert_not( vec_push( &vec, &i ) );

        cr_assert_eq( vec_length( &vec ), i + 2 );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i + 2 );

        int32_t out;
        vec_pop( &vec, &out );
        cr_assert_eq( out, i );

        cr_assert_eq( vec_length( &vec ), i + 1 );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i + 2 );
    }

    cr_assert_eq( vec_length( &vec ), limit );
    cr_assert( vec_capacity( &vec ) >= ( size_t ) limit );

    for ( int32_t i = limit; i > 0; --i ) {
        int32_t out;
        vec_pop( &vec, &out );
        cr_assert_eq( out, i - 1 );

        cr_assert_eq( vec_length( &vec ), i - 1 );
        cr_assert( vec_capacity( &vec ) >= ( size_t ) i - 1 );
    }

    vec_clear( &vec );
}

Test( vec, reserve_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_capacity( &vec ), 0 );
    cr_assert_not( vec_reserve( &vec, 0 ) );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    vec_clear( &vec );
}

Test( vec, reserve_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_capacity( &vec ), 0 );
    cr_assert_not( vec_reserve( &vec, 1 ) );
    cr_assert_eq( vec_capacity( &vec ), 1 );

    vec_clear( &vec );
}

Test( vec, reserve_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_reserve( &vec, 128 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 128 );

    vec_clear( &vec );
}

Test( vec, reserve_04, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_reserve( &vec, 1 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 1 );

    cr_assert_not( vec_reserve( &vec, 128 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 128 );

    cr_assert_not( vec_reserve( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 128 );

    vec_clear( &vec );
}

Test( vec, reserve_push_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_reserve( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert_eq( vec_capacity( &vec ), 2 );

    vec_clear( &vec );
}

Test( vec, reserve_push_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_reserve( &vec, 128 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 128 );

    vec_clear( &vec );
}

Test( vec, reserve_push_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_reserve( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert_eq( vec_capacity( &vec ), 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert_eq( vec_capacity( &vec ), 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 3 );
    cr_assert( vec_capacity( &vec ) >= 3 );

    vec_clear( &vec );
}

Test( vec, resize_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 0 ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    vec_clear( &vec );
}

Test( vec, resize_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 1 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    vec_clear( &vec );
}

Test( vec, resize_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 1 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_resize( &vec, 128 ) );

    cr_assert_eq( vec_length( &vec ), 128 );
    cr_assert( vec_capacity( &vec ) >= 128 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    vec_clear( &vec );
}

Test( vec, resize_pop_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 1 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, resize_pop_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, resize_pop_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_resize( &vec, 2 ) );
    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, resize_push_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 1 ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    vec_clear( &vec );
}

Test( vec, resize_push_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 3 );
    cr_assert( vec_capacity( &vec ) >= 3 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 4 );
    cr_assert( vec_capacity( &vec ) >= 4 );

    vec_clear( &vec );
}

Test( vec, resize_push_03, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 3 );
    cr_assert( vec_capacity( &vec ) >= 3 );

    cr_assert_not( vec_resize( &vec, 8 ) );

    cr_assert_eq( vec_length( &vec ), 8 );
    cr_assert( vec_capacity( &vec ) >= 8 );

    vec_clear( &vec );
}

Test( vec, resize_push_04, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_resize( &vec, 8 ) );

    cr_assert_eq( vec_length( &vec ), 8 );
    cr_assert( vec_capacity( &vec ) >= 8 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 9 );
    cr_assert( vec_capacity( &vec ) >= 9 );

    vec_clear( &vec );
}

Test( vec, resize_push_pop_01, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    vec_clear( &vec );
}

Test( vec, resize_push_pop_02, .timeout = 1 ) {
    Vec vec;
    vec_init( &vec, sizeof( int32_t ) );

    int32_t out;
    const int32_t in = 42;

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert_eq( vec_capacity( &vec ), 0 );

    cr_assert_not( vec_push( &vec, &in ) );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    cr_assert_not( vec_resize( &vec, 2 ) );

    cr_assert_eq( vec_length( &vec ), 2 );
    cr_assert( vec_capacity( &vec ) >= 2 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 0 );

    cr_assert_eq( vec_length( &vec ), 1 );
    cr_assert( vec_capacity( &vec ) >= 1 );

    out = 1;
    vec_pop( &vec, &out );
    cr_assert_eq( out, 42 );

    cr_assert_eq( vec_length( &vec ), 0 );
    cr_assert( vec_capacity( &vec ) >= 0 );

    vec_clear( &vec );
}

Test( vec, try_push_01, .timeout = 1 ) {
}

Test( vec, try_push_02, .timeout = 1 ) {
}

Test( vec, try_push_03, .timeout = 1 ) {
}

Test( vec, try_pop_01, .timeout = 1 ) {
}

Test( vec, try_pop_02, .timeout = 1 ) {
}

Test( vec, try_pop_03, .timeout = 1 ) {
}

Test( vec, swap, .timeout = 1 ) {
}

// TODO: Test idx, at, idx_ptr, at_pts, swap_elem_idx, swap_elem_idx,
//       insert, remove
