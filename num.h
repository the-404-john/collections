#ifndef NUM_H
#define NUM_H

#include <stddef.h>
#include <stdint.h>

#include "common.h"

typedef struct Num {
    uint32_t *digits;
    size_t length;
    size_t capacity;
    bool signed;
} Num;

bool num_add_assign( Num fst[ static 1], const Num snd[ static 1 ] );
bool num_sub_assign( Num fst[ static 1], const Num snd[ static 1 ] );
bool num_mul_assign( Num fst[ static 1], const Num snd[ static 1 ] );
bool num_div_assign( Num fst[ static 1], const Num snd[ static 1 ] );
bool num_rem_assign( Num fst[ static 1], const Num snd[ static 1 ] );

bool num_rem_euclid_assign( Num fst[ static 1], const Num snd[ static 1 ] );

bool num_add( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_sub( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_mul( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_div( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_rem( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_rem_euclid( const Num fst[ static 1 ], const Num snd[ static 1 ],
                     Num result[ static 1 ] );

bool num_pow( const Num num[ static 1 ], const Num exp[ static 1 ],
              Num result[ static 1 ] );

bool num_abs( const Num num[ static 1 ], Num result[ static 1 ] );

bool num_min( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

bool num_max( const Num fst[ static 1 ], const Num snd[ static 1 ],
              Num result[ static 1 ] );

Num num_abs_fast( Num num[ static 1 ] );

Num num_min_fast( Num fst[ static 1 ], Num snd[ static 1 ] );
Num num_max_fast( Num fst[ static 1 ], Num snd[ static 1 ] );

#endif // NUM_H
