/*
 * AgSimpleList -- Fast generic singly-linked list implementation.
 *
 * NODE DATA STRUCTURES
 *  
 *      SIMPLELIST uses c-style inheritance to allow any structure to be a node
 *      type.  Simply begin your structure definition with "AgSimpleListNode link;".
 *      For example:
 *
 *          struct MyNodeStruct
 *          {
 *              AgSimpleListNode link;
 *              ...
 *          }
 *
 *      The client is repsonsible for allocating the node structures.  The
 *      node's "AgSimpleListNode link" member should be zeroed initially.  It can be
 *      zeroed by calling AgSimpleList_InitNode(&link) or by using calloc to allocate
 *      the node structure.
 *
 * LIST MEMBERSHIP OF NODES
 *
 *      Each AgSimpleList node can be a member of 0 or 1 lists.  If the node is a
 *      member of 0 lists, it is considered to be "orphaned".
 *
 *      A node cannot be inserted into multiple lists.  Howerver, you can
 *      effectively achieve this by making your Node data structure contain a
 *      pointer to the object you'd like to insert into multiple lists, and
 *      instantiating mulitple such Nodes.
 *
 * COMPARISON TO OTHER LINKED LIST DATA TYPES
 *
 *      Compared to the other linked-list data types, AgSimpleList has the fastest
 *      performance, but supports the fewest operations in O(1) time.
 *
 */
#ifndef AG_SIMPLELIST_INCLUDED
#define AG_SIMPLELIST_INCLUDED

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
 * AgSimpleListHandle -- Opaque data type representing a linked list.
 */
typedef struct AgSimpleList * AgSimpleListHandle;

/*
 * AgSimpleListNodeHandle -- Pointer to a node.  This can be safely cast to the
 *      actual datatype of the node.
 */
typedef struct AgSimpleListNode * AgSimpleListNodeHandle;

#include <ag_simplelist_internal.h>

/*
 * AgSimpleList_New -- Create a new, empty list.
 *
 *      Returns a AgSimpleListHandle representing the new list.
 */
AgSimpleListHandle AgSimpleList_New();

/*
 * AgSimpleList_HeadNode -- Get the head node for a list, or NULL if the list is
 *      empty.
 *
 *      <hList> specifies the list to get the head node for.
 *
 *      Returns a AgSimpleListNodeHandle that should be cast to the actual pointer
 *      data type of your list node.
 */
AgSimpleListNodeHandle AgSimpleList_HeadNode(AgSimpleListHandle hList);

/*
 * AgSimpleList_IsEmpty -- Returns TRUE if the list is empty.
 */
bool AgSimpleList_IsEmpty(AgSimpleListHandle hList);

/*
 *  AgSimpleList_AddToFront -- Appends a node to the front of the list.
 *
 *      <hList> specifies the list to add to.
 *
 *      <hNode> specifies an orphaned Node to insert.  <hNode> may be a pointer
 *          to any structure that begins with a AgSimpleListNode element.
 *
 *      If AG_DEBUG is set, this will assert if <hNode> is already a
 *      member of a list.
 *
 *      This is macro that automatically casts <hNode> to AgSimpleListNodeHandle.
 */
#define AgSimpleList_AddToFront(hList, hNode) \
    AgSimpleList_AddToFront_Internal(hList, (AgSimpleListNodeHandle)hNode)
void AgSimpleList_AddToFront_Internal(AgSimpleListHandle hList, AgSimpleListNodeHandle hNode);

/*
 *  AgSimpleList_PopFront -- Remove the first node in the list (orphaning it), and
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
AgSimpleListNodeHandle AgSimpleList_PopFront(AgSimpleListHandle hList);

/*
 *  AgSimpleList_InsertNext -- Insert a node after another node.
 *
 *      <hNode> specifies the node to insert after.  <hNode> must be a member
 *          of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgSimpleListNode element.
 *
 *      <hNodeToInsert> specifies the orphaned Node to be inserted.
 *          <hNodeToInsert> must be orphaned.  <hNodeToInsert> may be a pointer
 *          to any structure that begins with a AgSimpleListNode element.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list and
 *      <hNodeToInsert> is orphaned.
 *
 *      This is macro that automatically casts <hNode> and <hNodeToInsert> to
 *      AgSimpleListNodeHandle.
 */
#define AgSimpleList_InsertNext(hNode, hNodeToInsert) \
    AgSimpleList_InsertNext_Internal((AgSimpleListNodeHandle)hNode, (AgSimpleListNodeHandle)hNodeToInsert)
void AgSimpleList_InsertNext_Internal(AgSimpleListNodeHandle hNode, AgSimpleListNodeHandle hNodeToInsert);

/*
 *  AgSimpleList_GetNext -- Get the node that comes after another node.
 *
 *      <hNode> specifies the node to get the next node for.  <hNode> must be a
 *          member of a list (non-orphaned).  <hNode> may be a pointer to any
 *          structure that begins with a AgSimpleListNode element.
 *
 *      Returns a pointer to the next node, or NULL if <hNode> is the last node
 *      in its list.  The returned pointer should be cast to the appropriate
 *      datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list.
 *
 *      This is macro that automatically casts <hNode> to AgSimpleListNodeHandle.
 */
#define AgSimpleList_GetNext(hNode) \
    AgSimpleList_GetNext_Internal((AgSimpleListNodeHandle)hNode)
AgSimpleListNodeHandle AgSimpleList_GetNext_Internal(AgSimpleListNodeHandle hNode);

/*
 *  AgSimpleList_PopNext -- Remove the node that comes after another node (orphaning
 *      the removed node).
 *
 *      <hNode> specifies the node to remove after.  <hNode> must be a member
 *          of a list (non-orphaned), and must not be the last node in the
 *          list.  <hNode> may be a pointer to any structure that begins with a
 *          AgSimpleListNode element.
 *
 *      Returns a pointer to the removed node, which should be cast to the
 *      appropriate datatype.
 *
 *      If AG_DEBUG is set, asserts that <hNode> is in a list and is
 *      not the last node.
 *
 *      This is macro that automatically casts <hNode> to AgSimpleListNodeHandle.
 */
#define AgSimpleList_PopNext(hNode) \
    AgSimpleList_PopNext_Internal((AgSimpleListNodeHandle)hNode)
AgSimpleListNodeHandle AgSimpleList_PopNext_Internal(AgSimpleListNodeHandle hNode);

/*
 *  AG_SIMPLELIST_ITERATE -- Helper macro for looping over all nodes in a list.
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
 *          AgSimpleListNode link;
 *          double val;
 *      };
 *
 *      void PrintList(AgSimpleListHandle hList)
 *      {
 *          MyNodeStruct * pNode;
 *          AG_SIMPLELIST_ITERATE(hList, pNode, MyNodeStruct)
 *          {
 *              printf("%lf\n", pNode->val);
 *          }
 *      }
 */
#define AG_SIMPLELIST_ITERATE(hList, hNode, typ) \
    for ((hNode) = (typ *)(hList)->_head; (hNode) != NULL; (hNode) = (typ *)((AgSimpleListNode *)(hNode))->_next)

#endif
