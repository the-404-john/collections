#include <assert.h>
#include <string.h>

#include <stdlib.h>
#include <stdckdint.h>

#include "list.h"

static inline bool prv_create_node( size_t elem_size, const void *new_elem,
                                    Node *result[ static 1 ] ) {
    assert( elem_size != 0 );
    assert( new_elem != nullptr );

    size_t byte_size;
    if ( ckd_add( &byte_size, sizeof( Node ), elem_size ) )
        return false;

    Node *node = ( Node * ) malloc( byte_size );
    if ( !node )
        return false;

    ( void ) memcpy( node->data, new_elem, elem_size );
    node->prev = node->next = nullptr;

    *result = node;
    return true;
}

static inline void prv_destroy_node( Node *node ) {
    assert( node != nullptr );
    free( node );
}

void list_init( List list[ static 1 ], size_t elem_byte_size ) {
    ( void ) memset( list, 0, sizeof( List ) );
    list->elem_byte_size = elem_byte_size;
}

void list_clear( List list[ static 1 ] ) {
    assert( !list->head == !list->tail );

    if ( list->head != nullptr ) {
        Node *next;

        for ( Node *curr = list->head; curr; curr = next ) {
            next = curr->next;
            prv_destroy_node( curr );
        }
    }

    ( void ) memset( list, 0, sizeof( List ) );
}

void list_swap( List fst[ static 1 ], List snd[ static 1 ] ) {
    List tmp = *fst;
    *fst = *snd;
    *snd = tmp;
}

size_t list_length( const List list[ static 1 ] ) {
    return list->length;
}

bool list_is_empty( const List list[ static 1 ] ) {
    return list->length == 0;
}

bool list_push_back( List list[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( !list->head == !list->tail );

    Node *new_node;
    if ( !prv_create_node( list->elem_byte_size, new_elem, &new_node ) )
        return false;

    if ( list->head == nullptr ) {
        list->head = new_node;
    } else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
    }

    list->tail = new_node;
    list->length += 1;
    return true;
}

bool list_push_front( List list[ static 1 ], const void *new_elem ) {
    assert( new_elem != nullptr );
    assert( !list->head == !list->tail );

    Node *new_node;
    if ( !prv_create_node( list->elem_byte_size, new_elem, &new_node ) )
        return false;

    if ( list->tail == nullptr ) {
        list->tail = new_node;
    } else {
        list->head->prev = new_node;
        new_node->next = list->head;
    }

    list->head = new_node;
    list->length += 1;
    return true;
}

bool list_pop_back( List list[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( !list->head == !list->tail );

    if ( list->tail == nullptr )
        return false;

    Node *node = list->tail;
    list->tail = list->tail->prev;

    if ( list->tail == nullptr ) {
        list->head = nullptr;
    } else {
        list->tail->next = nullptr;
    }

    ( void ) memcpy( result, node->data, list->elem_byte_size );
    prv_destroy_node( node );

    list->length -= 1;
    return true;
}

bool list_pop_front( List list[ static 1 ], void *result ) {
    assert( result != nullptr );
    assert( !list->head == !list->tail );
    assert( list->elem_byte_size != 0 );

    if ( list->head == nullptr )
        return false;

    Node *node = list->head;
    list->head = list->head->next;

    if ( list->head == nullptr ) {
        list->tail = nullptr;
    } else {
        list->head->prev = nullptr;
    }

    ( void ) memcpy( result, node->data, list->elem_byte_size );
    prv_destroy_node( node );

    list->length -= 1;
    return true;
}

bool list_insert( List list[ static 1 ],
                  size_t idx, const void *new_elem ) {
    assert( new_elem != nullptr );

    // TODO:
}

bool list_remove( List list[ static 1 ],
                  size_t idx, void *result ) {
    assert( result != nullptr );

    // TODO:
}

bool list_copy( const List from[ static 1 ], List to[ static 1 ] ) {
    assert( !from->head == !from->tail );

    Node *curr = from->head;
    const size_t elem_size = from->elem_byte_size;

    assert( elem_size != 0 );

    list_init( to, elem_size );

    for ( size_t i = 0; i < from->length; ++i ) {
        assert( curr != nullptr );

        if ( !list_push_back( to, curr->data ) ) {
            list_clear( to );
            return false;
        }
    }

    return true;
}

List list_merge( List fst[ static 1 ], List snd[ static 1 ], CmpFn fn ) {
    assert( !fst->head == !fst->tail );
    assert( !snd->head == !snd->tail );
    assert( fst->elem_byte_size == snd->elem_byte_size );

    const size_t elem_size = fst->elem_byte_size;

    List new_list;
    list_init( &new_list, elem_size );

    // TODO:

    return new_list;
}

bool list_sort( List list[ static 1 ], CmpFn fn );

size_t list_index( const List list[ static 1 ],
                   size_t start, const void *elem ) {
    assert( elem != nullptr );
    assert( !list->head == !list->tail );

    if ( start >= list->length )
        return list->length;

    Node *curr = list->head;

    for ( size_t i = 0; i < start; ++i ) {
        assert( curr != nullptr );
        curr = curr->next;
    }

    const size_t elem_size = list->elem_byte_size;
    assert( elem_size != 0 );

    for ( size_t i = 0; i < list->length; ++i ) {
        assert( curr != nullptr );

        if ( memcmp( curr->data, elem, elem_size ) == 0 )
            return i;

        curr = curr->next;
    }

    return list->length;
}

size_t list_count( const List list[ static 1 ], const void *elem ) {
    assert( elem != nullptr );
    assert( !list->head == !list->tail );

    size_t cnt = 0;
    Node *curr = list->head;

    const size_t elem_size = list->elem_byte_size;
    assert( elem_size != 0 );

    for ( size_t i = 0; i < list->length; ++i ) {
        assert( curr != nullptr );

        cnt += memcmp( curr->data, elem, elem_size ) == 0;
        curr = curr->next;
    }

    return cnt;
}

bool list_contains( const List list[ static 1 ], const void *elem ) {
    assert( elem != nullptr );
    assert( !list->head == !list->tail );

    Node *curr = list->head;

    const size_t elem_size = list->elem_byte_size;
    assert( elem_size != 0 );

    for ( size_t i = 0; i < list->length; ++i ) {
        assert( curr != nullptr );

        if ( memcmp( curr->data, elem, elem_size ) == 0 )
            return true;

        curr = curr->next;
    }

    return false;
}

ListIter list_iter_begin( const List list[ static 1 ] ) {
    return ( ListIter ){ .list = list, .curr_node = list->head };
}

ListIter list_iter_end( const List list[ static 1 ] ) {
    return ( ListIter ){ .list = list, .curr_node = list->tail };
}

void list_iter_next( ListIter it[ static 1 ] ) {
    if ( it->curr_node != nullptr )
        it->curr_node = it->curr_node->next;
}

void list_iter_prev( ListIter it[ static 1 ] ) {
    if ( it->curr_node != nullptr )
        it->curr_node = it->curr_node->prev;
}

void list_iter_saturating_next( ListIter it[ static 1 ] ) {
    if ( it->curr_node != nullptr && it->curr_node->next != nullptr )
        it->curr_node = it->curr_node->next;
}

void list_iter_saturating_prev( ListIter it[ static 1 ] ) {
    if ( it->curr_node != nullptr && it->curr_node->prev != nullptr )
        it->curr_node = it->curr_node->prev;
}

bool list_iter_curr( ListIter it[ static 1 ], void *result ) {
    assert( result != nullptr );

    if ( it->curr_node == nullptr )
        return false;

    const size_t elem_size = it->list->elem_byte_size;
    assert( elem_size != 0 );

    ( void ) memcpy( result, it->curr_node->data, elem_size );
    return true;
}
