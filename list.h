#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#include "common.h"

typedef struct Node {
    struct Node *next;
    struct Node *prev;
    unsigned char data[];
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    size_t length;
    size_t elem_byte_size;
} List;

typedef struct ListIter {
    const List *list;
    Node *curr_node;
} ListIter;

void list_init( List list[ static 1 ], size_t elem_byte_size );
void list_clear( List list[ static 1 ] );
void list_swap( List fst[ static 1 ], List snd[ static 1 ] );

size_t list_length( const List list[ static 1 ] );
bool list_is_empty( const List list[ static 1 ] );

bool list_push_back( List list[ static 1 ], const void *new_elem );
bool list_push_front( List list[ static 1 ], const void *new_elem );

bool list_pop_back( List list[ static 1 ], void *result );
bool list_pop_front( List list[ static 1 ], void *result );

bool list_insert( List list[ static 1 ],
                  size_t idx, const void *new_elem );

bool list_remove( List list[ static 1 ],
                  size_t idx, void *result );

bool list_copy( const List from[ static 1 ], List to[ static 1 ] );

// Algorithms
List list_merge( List fst[ static 1 ], List snd[ static 1 ], CmpFn fn );
bool list_sort( List list[ static 1 ], CmpFn fn );

size_t list_index( const List list[ static 1 ],
                   size_t start, const void *elem );

size_t list_count( const List list[ static 1 ], const void *elem );

bool list_contains( const List list[ static 1 ], const void *elem );

// Iterator
ListIter list_iter_begin( const List list[ static 1 ] );
ListIter list_iter_end( const List list[ static 1 ] );

void list_iter_next( ListIter it[ static 1 ] );
void list_iter_prev( ListIter it[ static 1 ] );

void list_iter_saturating_next( ListIter it[ static 1 ] );
void list_iter_saturating_prev( ListIter it[ static 1 ] );

bool list_iter_curr( ListIter it[ static 1 ], void *result );

#endif // LIST_H
