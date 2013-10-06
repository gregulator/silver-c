#ifndef AG_SIMPLELIST_INTERNAL_INCLUDED
#define AG_SIMPLELIST_INTERNAL_INCLUDED

/* 
 * Internal base structure for AgSimpleListNode.  Client code SHOULD NOT access these
 * fields directly.  Instead, use the AgSimpleList_* routines declared in AgSimpleList.h
 */
typedef struct AgSimpleListNode
{
    struct AgSimpleListNode *_next;
#if AG_DEBUG
    struct AgSimpleList *_dbg_hlist;
#endif
} AgSimpleListNode;

typedef struct AgSimpleList
{
    struct AgSimpleListNode *_head;
} AgSimpleList;

#endif
