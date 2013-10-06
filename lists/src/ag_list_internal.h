#ifndef AG_LIST_INTERNAL_INCLUDED
#define AG_LIST_INTERNAL_INCLUDED

/* 
 * Internal base structure for AgListNode.  Client code SHOULD NOT access these
 * fields directly.  Instead, use the AgList_* routines declared in AgList.h
 */
typedef struct AgListNode
{
    struct AgListNode *_next;
#if AG_DEBUG
    struct AgList *_dbg_hlist;
#endif
} AgListNode;

typedef struct AgList
{
    struct AgListNode *_head;
    struct AgListNode *_tail;
    unsigned _count;
} AgList;

#endif
