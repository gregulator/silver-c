#ifndef ZLIST_INCLUDED
#define ZLIST_INCLUDED

#include <stdlib.h>

#define ZLIST(typ) \
    struct { \
        typ *head; \
        typ *tail; \
        typ *tmp; \
        unsigned count; \
    } *

#define ZLINK(typ) \
    struct { \
        typ *next; \
    }

#define ZLIST_NEW() calloc(1, sizeof(ZLIST(void)))

#define ZLIST_FREE(list) free(list)

#define ZLIST_IS_EMPTY(list) ((list)->head == NULL)

#define ZLIST_HEAD(list) ((list)->head)

#define ZLIST_ADD_TO_FRONT(list, node) \
    ZLIST_ADD_TO_FRONT_EX((list), (node), link)

#define ZLIST_ADD_TO_FRONT_EX(list, node, linkname) \
    do { \
        (node)->linkname.next = (list)->head; \
        (list)->head = (node); \
    } while (0)

#define ZLIST_POP_FRONT(list) \
    ZLIST_POP_FRONT_EX((list), link)

#define ZLIST_POP_FRONT_EX(list, linkname) \
        ((list)->tmp = (list)->head,  \
         (list)->head = list->head->linkname.next, \
         (list)->tmp)

#define ZLIST_INSERT_AFTER(list, node, nodeToInsert) \
    ZLIST_INSERT_AFTER_EX((list), (node), (nodeToInsert), link)

#define ZLIST_INSERT_AFTER_EX(node, nodeToInsert, linkname) \
    do { \
        (nodeToInsert)->linkname.next = (node)->linkname.next; \
        (node)->(linkname).next = (nodeToInsert); \
    } while (0)

#define ZLIST_POP_AFTER_EX(node, linkname) \
    ((node)->linkname.next ?  \
        ((list)->tmp = (node)->linkname.next,  \
         ((node)->linkname.next = (node)->linkname.next->linkname.next), \
         ((list)->tmp) \
        ) \
        : \
        NULL \
    )

#define ZLIST_NEXT(node) \
    ZLIST_NEXT_EX(node, link)

#define ZLIST_NEXT_EX(node, linkname) \
        ((node)->linkname.next)

#endif
