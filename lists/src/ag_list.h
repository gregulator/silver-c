/*
 * AgList -- Generic singly-linked list implementation.
 *
 * NODE DATA STRUCTURES
 *  
 *      AgList uses c-style inheritance to allow any structure to be a node
 *      type.  Simply begin your structure definition with "AgListNode link;".
 *      For example:
 *
 *          struct MyNodeStruct
 *          {
 *              AgListNode link;
 *              ...
 *          }
 *
 *      The client is repsonsible for allocating the node structures.  The
 *      node's "AgListNode link" member should be zeroed initially.  It can be
 *      zeroed by calling AgList_InitNode(&link) or by using calloc to allocate
 *      the node structure.
 *
 * LIST MEMBERSHIP OF NODES
 *
 *      Each AgList node can be a member of 0 or 1 lists.  If the node is a
 *      member of 0 lists, it is considered to be "orphaned".
 *
 *      A node cannot be inserted into multiple lists.  Howerver, you can
 *      effectively achieve this by making your Node data structure contain a
 *      pointer to the object you'd like to insert into multiple lists, and
 *      instantiating mulitple such Nodes.
 *
 * COMPARISON TO OTHER LINKED LIST DATA TYPES
 *
 *      Compared to AgSimpleList, AgList adds these operations:
 *          - AgList_NumItems  O(1)
 *          - AgList_TailNode  O(1)
 *          - AgList_Append    O(1)
 *
 *      Support for these operations comes at the expense of slightly higher
 *      memory and computational overhead compared to AgSimpleList.
 *
 */
#ifndef AG_LIST_INCLUDED
#define AG_LIST_INCLUDED

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
 * AgListHandle -- Opaque data type representing a linked list.
 */
typedef struct AgList * AgListHandle;

/*
 * AgListNodeHandle -- Pointer to a node.  This can be safely cast to the
 *      actual datatype of the node.
 */
typedef struct AgListNode * AgListNodeHandle;

#include <ag_list_internal.h>

/*
 * AgList_New -- Create a new, empty list.
 *
 *      Returns a AgListHandle representing the new list.
 */
AgListHandle AgList_New();

/*
 * AgList_NumItems -- Get the head node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the head node for.
 *
 *      Returns a AgListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgListNodeHandle AgList_HeadNode(AgListHandle hList);

/*
 * AgList_HeadNode -- Get the head node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the head node for.
 *
 *      Returns a AgListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgListNodeHandle AgList_HeadNode(AgListHandle hList);

/*
 * AgList_TailNode -- Get the tail node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the tail node for.
 *
 *      Returns a AgListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgListNodeHandle AgList_TailNode(AgListHandle hList);

/*
 * AgList_IsEmpty -- Returns TRUE if the list is empty.
 */
bool AgList_IsEmpty(AgListHandle hList);

/*
 *  AgList_AddToFront -- Appends a node to the front of the list.
 *
 *      <hList> specifies the list to add to.
 *
 *      <hNode> specifies an orphaned Node to insert.  <hNode> may be a pointer
 *          to any structure that begins with a AgListNode element.
 *
 *      If AG_DEBUG is set, this will assert if <hNode> is already a
 *      member of a list.
 *
 *      This is macro that automatically casts <hNode> to AgListNodeHandle.
 */
#define AgList_AddToFront(hList, hNode) \
    AgList_AddToFront_Internal(hList, (AgListNodeHandle)hNode)
void AgList_AddToFront_Internal(AgListHandle hList, AgListNodeHandle hNode);

/*
 *  AgList_Append -- Appends a node to the end of the list.
 *
 *      <hList> specifies the list to add to.
 *
 *      <hNode> specifies an orphaned Node to insert.  <hNode> may be a pointer
 *          to any structure that begins with a AgListNode element.
 *
 *      If AG_DEBUG is set, this will assert if <hNode> is already a
 *      member of a list.
 *
 *      This is macro that automatically casts <hNode> to AgListNodeHandle.
 */
#define AgList_Append(hList, hNode) \
    AgList_Append_Internal(hList, (AgListNodeHandle)hNode)
void AgList_Append_Internal(AgListHandle hList, AgListNodeHandle hNode);

/*
 *  AgList_PopFront -- Remove the first node in the list (orphaning it), and
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
AgListNodeHandle AgList_PopFront(AgListHandle hList);

/*
 *  AgList_InsertNext -- Insert a node after another node.
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to insert after.  <hNode> must be a member
 *          of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgListNode element.
 *
 *      <hNodeToInsert> specifies the orphaned Node to be inserted.
 *          <hNodeToInsert> must be orphaned.  <hNodeToInsert> may be a pointer
 *          to any structure that begins with a AgListNode element.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list and
 *      <hNodeToInsert> is orphaned.
 *
 *      This is macro that automatically casts <hNode> and <hNodeToInsert> to
 *      AgListNodeHandle.
 */
#define AgList_InsertNext(hList, hNode, hNodeToInsert) \
    AgList_InsertNext_Internal(hList, (AgListNodeHandle)hNode, (AgListNodeHandle)hNodeToInsert)
void AgList_InsertNext_Internal(AgListHandle hList, AgListNodeHandle hNode, AgListNodeHandle hNodeToInsert);

/*
 *  AgList_GetNext -- Get the node that comes after another node.
 *
 *      <hNode> specifies the node to get the next node for.  <hNode> must be a
 *          member of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgListNode element.
 *
 *      Returns a pointer to the next node, or NULL if <hNode> is the last node
 *      in its list.  The returned pointer should be cast to the appropriate
 *      datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list.
 *
 *      This is macro that automatically casts <hNode> to AgListNodeHandle.
 */
#define AgList_GetNext(hNode) \
    AgList_GetNext_Internal((AgListNodeHandle)hNode)
AgListNodeHandle AgList_GetNext_Internal(AgListNodeHandle hNode);

/*
 *  AgList_PopNext -- Remove the node that comes after another node (orphaning
 *      the removed node).
 *
 *      <hList> is the list to <hNode> belongs to.
 *
 *      <hNode> specifies the node to remove after.  <hNode> must be a member
 *          of a list (non-orphaned), and must not be the last node in the
 *          list.  <hNode> may be a pointer to any structure that begins with a
 *          AgListNode element.
 *
 *      Returns a pointer to the removed node, which should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in <hList> and is
 *      not the last node.
 *
 *      This is macro that automatically casts <hNode> to AgListNodeHandle.
 */
#define AgList_PopNext(hList, hNode) \
    AgList_PopNext_Internal(hList, (AgListNodeHandle)hNode)
AgListNodeHandle AgList_PopNext_Internal(AgListHandle hList, AgListNodeHandle hNode);

/*
 *  AG_LIST_ITERATE -- Helper macro for looping over all nodes in a list.
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
 *          AgListNode link;
 *          double val;
 *      };
 *
 *      void PrintList(AgListHandle hList)
 *      {
 *          MyNodeStruct * pNode;
 *          AG_LIST_ITERATE(hList, pNode, MyNodeStruct)
 *          {
 *              printf("%lf\n", pNode->val);
 *          }
 *      }
 */
#define AG_LIST_ITERATE(hList, hNode, typ) \
    for ((hNode) = (typ *)(hList)->_head; (hNode) != NULL; (hNode) = (typ *)((AgListNode *)(hNode))->_next)

#endif
