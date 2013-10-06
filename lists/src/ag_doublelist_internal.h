#ifndef AG_DOUBLELIST_INTERNAL_INCLUDED
#define AG_DOUBLELIST_INTERNAL_INCLUDED

/* 
 * Internal base structure for AgDoubleListNode.  Client code SHOULD NOT access these
 * fields directly.  Instead, use the AgDoubleList_* routines declared in AgDoubleList.h
 */
typedef struct AgDoubleListNode
{
    struct AgDoubleListNode *_next;
    struct AgDoubleListNode *_prev;
#if AG_DEBUG
    struct AgDoubleList *_dbg_hlist;
#endif
} AgDoubleListNode;

typedef struct AgDoubleList
{
    struct AgDoubleListNode *_head;
    struct AgDoubleListNode *_tail;
    unsigned _count;
} AgDoubleList;

#endif
