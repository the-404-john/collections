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

static inline Node* prv_list_idx_ptr_from_back( const List list[ static 1 ],
                                                size_t idx ) {
    assert( !list->head == !list->tail );
    assert( idx < list->length );

    Node *curr = list->tail;

    for ( size_t i = list->length - 1; i > idx; --i ) {
        assert( curr != nullptr );
        curr = curr->prev;
    }

    return curr;
}

static inline Node* prv_list_idx_ptr_from_front( const List list[ static 1 ],
                                                 size_t idx ) {
    assert( !list->head == !list->tail );
    assert( idx < list->length );

    Node *curr = list->head;

    for ( size_t i = 0; i < idx; ++i ) {
        assert( curr != nullptr );
        curr = curr->next;
    }

    return curr;
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

void list_idx_ptr( const List list[ static 1 ], size_t idx,
                   void *result[ static 1 ] ) {
    assert( !list->head == !list->tail );
    assert( idx < list->length );

    Node *node;

    if ( idx <= list->length / 2 )
        node = prv_list_idx_ptr_from_front( list, idx );
    else
        node = prv_list_idx_ptr_from_back( list, idx );

    assert( node != nullptr );
    *result = &node->data;
}

bool list_at_ptr( const List list[ static 1 ], size_t idx,
                  void *result[ static 1 ] ) {
    assert( !list->head == !list->tail );

    if ( idx >= list->length )
        return false;

    list_idx_ptr( list, idx, result );
    return true;
}

void list_idx( const List list[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( !list->head == !list->tail );

    assert( idx < list->length );

    void *data;
    list_idx_ptr( list, idx, &data );
    ( void ) memcpy( result, data, list->elem_byte_size );
}

bool list_at( const List list[ static 1 ], size_t idx, void *result ) {
    assert( result != nullptr );
    assert( !list->head == !list->tail );

    if ( idx >= list->length )
        return false;

    list_idx( list, idx, result );
    return true;
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
    assert( !list->head == !list->tail );

    assert( idx <= list->length );

    if ( idx == 0 )
        return list_push_front( list, new_elem );

    if ( idx == list->length )
        return list_push_back( list, new_elem );

    Node *new_node;
    if ( !prv_create_node( list->elem_byte_size, new_elem, &new_node ) )
        return false;

    Node *prev;
    if ( idx <= list->length / 2 )
        prev = prv_list_idx_ptr_from_front( list, idx - 1 );
    else
        prev = prv_list_idx_ptr_from_back( list, idx - 1 );

    assert( prev != nullptr );
    assert( prev->prev != nullptr );
    assert( prev->next != nullptr );

    new_node->next = prev->next;
    new_node->prev = prev;

    prev->next = new_node;

    list->length += 1;
    return true;
}

void list_remove( List list[ static 1 ],
                  size_t idx, void *result ) {
    assert( result != nullptr );
    assert( !list->head == !list->tail );

    assert( idx < list->length );

    if ( idx == 0 ) {
        list_pop_front( list, result );
        return;
    }

    if ( idx + 1 == list->length ) {
        list_pop_back( list, result );
        return;
    }

    Node *prev;

    if ( idx - 1 <= list->length / 2 )
        prev = prv_list_idx_ptr_from_front( list, idx );
    else
        prev = prv_list_idx_ptr_from_back( list, idx );

    assert( prev != nullptr );

    Node *curr = prev->next;

    assert( curr != nullptr );
    assert( curr->next != nullptr );
    assert( curr->prev != nullptr );

    curr->next->prev = prev;
    prev->next = curr->next;

    ( void ) memcpy( result, curr->data, list->elem_byte_size );
    prv_destroy_node( curr );

    list->length -= 1;
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

    List new_list;
    list_init( &new_list, fst->elem_byte_size );

    Node dummy = {};

    Node *tail = &dummy,
         *curr_fst = fst->head,
         *curr_snd = snd->head;

    while ( curr_fst != nullptr && curr_snd != nullptr ) {
        Node *curr;

        if ( fn( curr_fst, curr_snd ) != Greater ) {
            curr = curr_fst;
            curr_fst = curr_fst->next;
        } else {
            curr = curr_snd;
            curr_snd = curr_snd->next;
        }

        curr->next = nullptr;
        curr->prev = tail;

        tail->next = curr;
        tail = curr;
    }

    if ( curr_fst != nullptr ) {
        curr_fst->prev = tail;

        tail->next = curr_fst;
        tail = fst->tail;
    }

    if ( curr_snd != nullptr ) {
        curr_snd->prev = tail;

        tail->next = curr_snd;
        tail = snd->tail;
    }

    if ( dummy.next != nullptr )
        dummy.next->prev = nullptr;

    new_list.head = dummy.next;
    new_list.tail = tail;

    assert( fst->length <= SIZE_MAX - snd->length );
    new_list.length += fst->length + snd->length;

    fst->head = fst->tail = nullptr;
    snd->head = snd->tail = nullptr;
    fst->length = snd->length = 0;

    return new_list;
}

void list_sort( List list[ static 1 ], CmpFn fn ) {
    if ( list->length <= 1 )
        return;

    const size_t mid_idx = list->length / 2;
    Node *mid = prv_list_idx_ptr_from_front( list, mid_idx );

    List left = {
        .head = list->head,
        .tail = mid->prev,
        .length = mid_idx,
        .elem_byte_size = list->elem_byte_size
    };

    List right = {
        .head = mid,
        .tail = list->tail,
        .length = list->length - mid_idx,
        .elem_byte_size = list->elem_byte_size
    };

    left.tail->next = nullptr;
    right.head->prev = nullptr;

    list->head = list->tail = nullptr;
    list->length = 0;

    list_sort( &left, fn );
    list_sort( &right, fn );

    *list = list_merge( &left, &right, fn );
}

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
