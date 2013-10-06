#include <ag_list.h>
#include <stdlib.h>
#include <assert.h>


AgListHandle AgList_New()
{
    AgListHandle hNew;
    hNew = calloc(1, sizeof *hNew);
    return hNew;
}

AgListNodeHandle AgList_HeadNode(AgListHandle hList)
{
    return hList->_head;
}

AgListNodeHandle AgList_TailNode(AgListHandle hList)
{
    return hList->_tail;
}

bool AgList_IsEmpty(AgListHandle hList)
{
    return hList->_head == NULL;
}

unsigned AgList_NumItems(AgListHandle hList)
{
    return hList->_count;
}

void AgList_AddToFront_Internal(AgListHandle hList, AgListNodeHandle hNode)
{
#if AG_DEBUG
    assert(!hNode->_dbg_hlist);
    hNode->_dbg_hlist = hList;
#endif
    hNode->_next = hList->_head;
    hList->_head = hNode;
    if (!hList->_tail)
        hList->_tail = hNode;
    hList->_count++;
}

void AgList_Append_Internal(AgListHandle hList, AgListNodeHandle hNode)
{
#if AG_DEBUG
    assert(!hNode->_dbg_hlist);
    hNode->_dbg_hlist = hList;
#endif
    hNode->_next = NULL;
    if (!hList->_head)
    {
        hList->_head = hNode;
    }
    else
    {
        hList->_tail->_next = hNode;
    }
    hList->_tail = hNode;
    hList->_count++;
}

AgListNodeHandle AgList_PopFront(AgListHandle hList)
{
    AgListNodeHandle tmp;
#if AG_DEBUG
    assert(hList->_head);
    hList->_head->_dbg_hlist = NULL;
#endif
    tmp = hList->_head;
    hList->_head = hList->_head->_next;
    hList->_count--;
    if (hList->_head == NULL)
        hList->_tail = NULL;
    return tmp;
}

void AgList_InsertNext_Internal(AgListHandle hList, AgListNodeHandle hNode, AgListNodeHandle hNodeToInsert)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(!hNodeToInsert->_dbg_hlist);
    hNodeToInsert->_dbg_hlist = hNode->_dbg_hlist;
#endif
    hNodeToInsert->_next = hNode->_next;
    if (hNode->_next == NULL)
        hList->_tail = hNodeToInsert;
    hNode->_next = hNodeToInsert;
    hList->_count++;
}

AgListNodeHandle AgList_PopNext_Internal(AgListHandle hList, AgListNodeHandle hNode)
{
    AgListNodeHandle tmp;
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(hNode->_next);
    hNode->_next->_dbg_hlist = NULL;
#endif
    tmp = hNode->_next;
    hNode->_next = hNode->_next->_next;
    if (hNode->_next == NULL)
        hList->_tail = hNode;
    hList->_count--;
    return tmp;
}

AgListNodeHandle AgList_GetNext_Internal(AgListNodeHandle hNode)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
#endif
    return hNode->_next;
}
