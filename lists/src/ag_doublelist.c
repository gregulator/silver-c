#include <ag_doublelist.h>
#include <stdlib.h>
#include <assert.h>


AgDoubleListHandle AgDoubleList_New()
{
    AgDoubleListHandle hNew;
    hNew = calloc(1, sizeof *hNew);
    return hNew;
}

AgDoubleListNodeHandle AgDoubleList_HeadNode(AgDoubleListHandle hList)
{
    return hList->_head;
}

AgDoubleListNodeHandle AgDoubleList_TailNode(AgDoubleListHandle hList)
{
    return hList->_tail;
}

bool AgDoubleList_IsEmpty(AgDoubleListHandle hList)
{
    return hList->_head == NULL;
}

unsigned AgDoubleList_NumItems(AgDoubleListHandle hList)
{
    return hList->_count;
}

void AgDoubleList_AddToFront_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(!hNode->_dbg_hlist);
    hNode->_dbg_hlist = hList;
#endif
    hNode->_next = hList->_head;
    hNode->_prev = NULL;
    if (!hList->_tail)
    {
        hList->_tail = hNode;
    }
    else
    {
        hList->_head->_prev = hNode;
    }
    hList->_head = hNode;
    hList->_count++;
}

void AgDoubleList_Append_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(!hNode->_dbg_hlist);
    hNode->_dbg_hlist = hList;
#endif
    hNode->_prev = hList->_tail;
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

AgDoubleListNodeHandle AgDoubleList_PopFront(AgDoubleListHandle hList)
{
    AgDoubleListNodeHandle tmp;
#if AG_DEBUG
    assert(hList->_head);
    hList->_head->_dbg_hlist = NULL;
#endif
    tmp = hList->_head;
    hList->_head = hList->_head->_next;
    hList->_count--;
    if (hList->_head == NULL)
    {
        hList->_tail = NULL;
    }
    else
    {
        hList->_head->_prev = NULL;
    }
    return tmp;
}

void AgDoubleList_InsertNext_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode, AgDoubleListNodeHandle hNodeToInsert)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(!hNodeToInsert->_dbg_hlist);
    hNodeToInsert->_dbg_hlist = hNode->_dbg_hlist;
#endif
    hNodeToInsert->_next = hNode->_next;
    hNodeToInsert->_prev = hNode;
    if (hNode->_next == NULL)
    {
        hList->_tail = hNodeToInsert;
    }
    else
    {
        hNode->_next->_prev = hNodeToInsert;
    }
    hNode->_next = hNodeToInsert;
    hList->_count++;
}

void AgDoubleList_InsertPrev_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode, AgDoubleListNodeHandle hNodeToInsert)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(!hNodeToInsert->_dbg_hlist);
    hNodeToInsert->_dbg_hlist = hNode->_dbg_hlist;
#endif
    hNodeToInsert->_next = hNode;
    hNodeToInsert->_prev = hNode->_prev;
    if (hNode->_prev == NULL)
    {
        hList->_head = hNodeToInsert;
    }
    else
    {
        hNode->_prev->_next = hNodeToInsert;
    }
    hNode->_prev = hNodeToInsert;
    hList->_count++;
}

AgDoubleListNodeHandle AgDoubleList_PopNext_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode)
{
    AgDoubleListNodeHandle tmp;
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(hNode->_next);
    hNode->_next->_dbg_hlist = NULL;
#endif
    tmp = hNode->_next;
    hNode->_next = hNode->_next->_next;
    if (hNode->_next == NULL)
    {
        hList->_tail = hNode;
    }
    else
    {
        hNode->_next->_prev = hNode;
    }
    hList->_count--;
    return tmp;
}


AgDoubleListNodeHandle AgDoubleList_PopPrev_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode)
{
    AgDoubleListNodeHandle tmp;
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
    assert(hNode->_dbg_hlist == hList);
    assert(hNode->_prev);
    hNode->_prev->_dbg_hlist = NULL;
#endif
    tmp = hNode->_prev;
    hNode->_prev = hNode->_prev->_prev;
    if (hNode->_prev == NULL)
    {
        hList->_head = hNode;
    }
    else
    {
        hNode->_prev->_next = hNode;
    }
    hList->_count--;
    return tmp;
}

AgDoubleListNodeHandle AgDoubleList_GetNext_Internal(AgDoubleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
#endif
    return hNode->_next;
}

AgDoubleListNodeHandle AgDoubleList_GetPrev_Internal(AgDoubleListNodeHandle hNode)
{
#if AG_DEBUG
    assert(hNode->_dbg_hlist);
#endif
    return hNode->_prev;
}
