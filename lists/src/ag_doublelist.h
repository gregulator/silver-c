/*
 * AgDoubleList -- Generic doubly-linked list implementation.
 *
 * NODE DATA STRUCTURES
 *  
 *      AgDoubleList uses c-style inheritance to allow any structure to be a node
 *      type.  Simply begin your structure definition with "AgDoubleListNode link;".
 *      For example:
 *
 *          struct MyNodeStruct
 *          {
 *              AgDoubleListNode link;
 *              ...
 *          }
 *
 *      The client is repsonsible for allocating the node structures.  The
 *      node's "AgDoubleListNode link" member should be zeroed initially.  It can be
 *      zeroed by calling AgDoubleList_InitNode(&link) or by using calloc to allocate
 *      the node structure.
 *
 * LIST MEMBERSHIP OF NODES
 *
 *      Each AgDoubleList node can be a member of 0 or 1 lists.  If the node is a
 *      member of 0 lists, it is considered to be "orphaned".
 *
 *      A node cannot be inserted into multiple lists.  Howerver, you can
 *      effectively achieve this by making your Node data structure contain a
 *      pointer to the object you'd like to insert into multiple lists, and
 *      instantiating mulitple such Nodes.
 *
 * COMPARISON TO OTHER LINKED LIST DATA TYPES
 *
 *      Compared to AgList, AgDoubleList adds these operations:
 *          - AgDoubleList_PopNode      O(1)
 *          - AgDoubleList_GetPrev      O(1)
 *          - AgDoubleList_InsertPrev   O(1)
 *          - AgDoubleList_PopPrev      O(1)
 *
 *      Support for these operations comes at the expense of slightly higher
 *      memory and computational overhead compared to AgSimpleList.
 *
 */
#ifndef AG_DOUBLELIST_INCLUDED
#define AG_DOUBLELIST_INCLUDED

/*
 * Debugging.
 *
 * Build with AG_DEBUG=1 to enable debug checks.
 */
#ifndef AG_DEBUG
#define AG_DEBUG 0
#endif

#include <stdbool.h>

/*
 * AgDoubleListHandle -- Opaque data type representing a linked list.
 */
typedef struct AgDoubleList * AgDoubleListHandle;

/*
 * AgDoubleListNodeHandle -- Pointer to a node.  This can be safely cast to the
 *      actual datatype of the node.
 */
typedef struct AgDoubleListNode * AgDoubleListNodeHandle;

#include <ag_doublelist_internal.h>

/*
 * AgDoubleList_New -- Create a new, empty list.
 *
 *      Returns a AgDoubleListHandle representing the new list.
 */
AgDoubleListHandle AgDoubleList_New();

/*
 * AgDoubleList_NumItems -- Get the head node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the head node for.
 *
 *      Returns a AgDoubleListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgDoubleListNodeHandle AgDoubleList_HeadNode(AgDoubleListHandle hList);

/*
 * AgDoubleList_HeadNode -- Get the head node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the head node for.
 *
 *      Returns a AgDoubleListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgDoubleListNodeHandle AgDoubleList_HeadNode(AgDoubleListHandle hList);

/*
 * AgDoubleList_TailNode -- Get the tail node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the tail node for.
 *
 *      Returns a AgDoubleListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgDoubleListNodeHandle AgDoubleList_TailNode(AgDoubleListHandle hList);

/*
 * AgDoubleList_IsEmpty -- Returns TRUE if the list is empty.
 */
bool AgDoubleList_IsEmpty(AgDoubleListHandle hList);

/*
 *  AgDoubleList_AddToFront -- Appends a node to the front of the list.
 *
 *      <hList> specifies the list to add to.
 *
 *      <hNode> specifies an orphaned Node to insert.  <hNode> may be a pointer
 *          to any structure that begins with a AgDoubleListNode element.
 *
 *      If AG_DEBUG is set, this will assert if <hNode> is already a
 *      member of a list.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_AddToFront(hList, hNode) \
    AgDoubleList_AddToFront_Internal(hList, (AgDoubleListNodeHandle)hNode)
void AgDoubleList_AddToFront_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode);

/*
 *  AgDoubleList_Append -- Appends a node to the end of the list.
 *
 *      <hList> specifies the list to add to.
 *
 *      <hNode> specifies an orphaned Node to insert.  <hNode> may be a pointer
 *          to any structure that begins with a AgDoubleListNode element.
 *
 *      If AG_DEBUG is set, this will assert if <hNode> is already a
 *      member of a list.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_Append(hList, hNode) \
    AgDoubleList_Append_Internal(hList, (AgDoubleListNodeHandle)hNode)
void AgDoubleList_Append_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode);

/*
 *  AgDoubleList_PopFront -- Remove the first node in the list (orphaning it), and
 *      return its handle.  
 *      
 *      <hList> specifies the list to remove from.
 *
 *      Returns a pointer to the removed node, which should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts if the the list is empty.
 *      Otherwise, crashes.
 */
AgDoubleListNodeHandle AgDoubleList_PopFront(AgDoubleListHandle hList);

/*
 *  AgDoubleList_InsertNext -- Insert a node after another node.
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to insert after.  <hNode> must be a member
 *          of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgDoubleListNode element.
 *
 *      <hNodeToInsert> specifies the orphaned Node to be inserted.
 *          <hNodeToInsert> must be orphaned.  <hNodeToInsert> may be a pointer
 *          to any structure that begins with a AgDoubleListNode element.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list and
 *      <hNodeToInsert> is orphaned.
 *
 *      This is macro that automatically casts <hNode> and <hNodeToInsert> to
 *      AgDoubleListNodeHandle.
 */
#define AgDoubleList_InsertNext(hList, hNode, hNodeToInsert) \
    AgDoubleList_InsertNext_Internal(hList, (AgDoubleListNodeHandle)hNode, (AgDoubleListNodeHandle)hNodeToInsert)
void AgDoubleList_InsertNext_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode, AgDoubleListNodeHandle hNodeToInsert);

/*
 *  AgDoubleList_GetNext -- Get the node that comes after another node.
 *
 *      <hNode> specifies the node to get the next node for.  <hNode> must be a
 *          member of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgDoubleListNode element.
 *
 *      Returns a pointer to the next node, or NULL if <hNode> is the last node
 *      in its list.  The returned pointer should be cast to the appropriate
 *      datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_GetNext(hNode) \
    AgDoubleList_GetNext_Internal((AgDoubleListNodeHandle)hNode)
AgDoubleListNodeHandle AgDoubleList_GetNext_Internal(AgDoubleListNodeHandle hNode);

/*
 *  AgDoubleList_PopNext -- Remove the node that comes after another node (orphaning
 *      the removed node).
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to remove after.  <hNode> must be a member
 *          of a list (non-orphaned), and must not be the last node in the
 *          list.  <hNode> may be a pointer to any structure that begins with a
 *          AgDoubleListNode element.
 *
 *      Returns a pointer to the removed node, which should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in <hList> and is
 *      not the last node.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_PopNext(hList, hNode) \
    AgDoubleList_PopNext_Internal(hList, (AgDoubleListNodeHandle)hNode)
AgDoubleListNodeHandle AgDoubleList_PopNext_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode);

/*
 *  AgDoubleList_InsertPrev -- Insert a node before another node.
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to insert before.  <hNode> must be a member
 *          of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgDoubleListNode element.
 *
 *      <hNodeToInsert> specifies the orphaned Node to be inserted.
 *          <hNodeToInsert> must be orphaned.  <hNodeToInsert> may be a pointer
 *          to any structure that begins with a AgDoubleListNode element.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list and
 *      <hNodeToInsert> is orphaned.
 *
 *      This is macro that automatically casts <hNode> and <hNodeToInsert> to
 *      AgDoubleListNodeHandle.
 */
#define AgDoubleList_InsertPrev(hList, hNode, hNodeToInsert) \
    AgDoubleList_InsertPrev_Internal(hList, (AgDoubleListNodeHandle)hNode, (AgDoubleListNodeHandle)hNodeToInsert)
void AgDoubleList_InsertPrev_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode, AgDoubleListNodeHandle hNodeToInsert);

/*
 *  AgDoubleList_GetPrev -- Get the node that comes before another node.
 *
 *      <hNode> specifies the node to get the previous node for.  <hNode> must
 *          be a member of a list (non-orphaned).  <hNode> may be a pointer to
 *          any structure that begins with a AgDoubleListNode element.
 *
 *      Returns a pointer to the previous node, or NULL if <hNode> is the first
 *      node in its list.  The returned pointer should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_GetPrev(hNode) \
    AgDoubleList_GetNext_Internal((AgDoubleListNodeHandle)hNode)
AgDoubleListNodeHandle AgDoubleList_GetPrev_Internal(AgDoubleListNodeHandle hNode);

/*
 *  AgDoubleList_PopPrev -- Remove the node that comes before another node (orphaning
 *      the removed node).
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to remove before.  <hNode> must be a member
 *          of a list (non-orphaned), and must not be the first node in the
 *          list.  <hNode> may be a pointer to any structure that begins with a
 *          AgDoubleListNode element.
 *
 *      Returns a pointer to the removed node, which should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in <hList> and is
 *      not the first node.
 *
 *      This is macro that automatically casts <hNode> to AgDoubleListNodeHandle.
 */
#define AgDoubleList_PopNext(hList, hNode) \
    AgDoubleList_PopNext_Internal(hList, (AgDoubleListNodeHandle)hNode)
AgDoubleListNodeHandle AgDoubleList_PopNext_Internal(AgDoubleListHandle hList, AgDoubleListNodeHandle hNode);

/*
 *  AG_DOUBLELIST_ITERATE -- Helper macro for looping over all nodes in a list.
 *
 *      <hList> is the list to iterate over.
 *
 *      <hNode> is a pointer to the appropriate (actual) node data type, which
 *          will be used for the iteration (it be set to point to each node in
 *          succession).
 *
 *      <typ> is the actual data type of <hNode>.
 *
 *      For example:
 *
 *      struct MyNodeStruct
 *      {
 *          AgDoubleListNode link;
 *          double val;
 *      };
 *
 *      void PrintList(AgDoubleListHandle hList)
 *      {
 *          MyNodeStruct * pNode;
 *          AG_DOUBLELIST_ITERATE(hList, pNode, MyNodeStruct)
 *          {
 *              printf("%lf\n", pNode->val);
 *          }
 *      }
 */
#define AG_DOUBLELIST_ITERATE(hList, hNode, typ) \
    for ((hNode) = (typ *)(hList)->_head; (hNode) != NULL; (hNode) = (typ *)((AgDoubleListNode *)(hNode))->_next)

#endif
