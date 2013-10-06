#include <ag_simplelist.h>
#include <stdlib.h>
#include <assert.h>


AgSimpleListHandle AgSimpleList_New()
{
    AgSimpleListHandle hNew;
    hNew = calloc(1, sizeof *hNew);
    return hNew;
}

AgSimpleListNodeHandle AgSimpleList_HeadNode(AgSimpleListHandle hList)
{
    return hList->_head;
}

bool AgSimpleList_IsEmpty(AgSimpleListHandle hList)
{
    return hList->_head == NULL;
}

void AgSimpleList_AddToFront_Internal(AgSimpleListHandle hList, AgSimpleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(!hNode->_dbg_hlist);
    hNode->_dbg_hlist = hList;
#endif
    hNode->_next = hList->_head;
    hList->_head = hNode;
}

AgSimpleListNodeHandle AgSimpleList_PopFront(AgSimpleListHandle hList)
{
    AgSimpleListNodeHandle tmp;
#if AG_DEBUG
    assert(hList->_head);
    hList->_head->_dbg_hlist = NULL;
#endif
    tmp = hList->_head;
    hList->_head = hList->_head->_next;
    return tmp;
}

void AgSimpleList_InsertNext_Internal(AgSimpleListNodeHandle hNode, AgSimpleListNodeHandle hNodeToInsert)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(!hNodeToInsert->_dbg_hlist);
    hNodeToInsert->_dbg_hlist = hNode->_dbg_hlist;
#endif
    hNodeToInsert->_next = hNode->_next;
    hNode->_next = hNodeToInsert;
}

AgSimpleListNodeHandle AgSimpleList_PopNext_Internal(AgSimpleListNodeHandle hNode)
{
    AgSimpleListNodeHandle tmp;
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_next);
    hNode->_next->_dbg_hlist = NULL;
#endif
    tmp = hNode->_next;
    hNode->_next = hNode->_next->_next;
    return tmp;
}

AgSimpleListNodeHandle AgSimpleList_GetNext_Internal(AgSimpleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
#endif
    return hNode->_next;
}
