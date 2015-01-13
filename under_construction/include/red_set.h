/*
 * RedSet - Set (Collection) data structure.  A set is an unordered collection
 * of unique elements.
 *
 * The elements of a set are arbitrary chunks of data.  They can be any size
 * and type (unfortunately, this means there is no type safety checks).  An
 * element is considered to be in a set iff both the element's data size and
 * all data bytes match a previously added element's exactly.
 */
#ifndef RED_SET_INCLUDED
#define RED_SET_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RedSet_t * RedSet;

/* TODO: move to internal header */
typedef struct RedSetIter_t
{
    RedHashIter_t _hashIter;
};

typedef struct RedSetIter_t RedSetIter_t;
/* TODO: constify stuff */

/*
 * RedSet_New - Create a new (empty) set.
 *
 *      Returns a newly allocated RedSet object handle, or NULL if memory
 *      allocation failed.
 */
RedSet RedSet_New();

/*
 * RedSet_NewUnion - Create a new set that is the union of two existing sets.
 *      
 *      <sourceA> and <sourceB> are the two sets to take the union of.  Both
 *          must be non-NULL.
 *
 *      Returns a newly allocated RedSet object handle, or NULL if memory
 *      allocation failed.  On success, the returned set will contain all
 *      elements from both <sourceA> and <sourceB> (shallow copied).
 */
RedSet RedSet_NewUnion(RedSet sourceA, RedSet sourceB);

/*
 * RedSet_NewUnion - Create a new set that is the interection of two existing
 *      sets.
 *      
 *      <sourceA> and <sourceB> are the two sets to take the intersection of.
 *          Both must be non-NULL.
 *
 *      Returns a newly allocated RedSet object handle, or NULL if memory
 *      allocation failed.  On success, the returned set will contain all
 *      elements that are found in both <sourceA> and <sourceB> (shallow
 *      copied).
 */
RedSet RedSet_NewIntersection(RedSet sourceA, RedSet sourceB);

/*
 * RedSet_NewDifference - Create a new set that is the difference of two
 *      existing sets.
 *      
 *      <sourceA> and <sourceB> are the two sets to take the difference of.
 *          Both must be non-NULL.
 *
 *      Returns a newly allocated RedSet object handle, or NULL if memory
 *      allocation failed.  On success, the returned set will contain all
 *      elements that are found in <sourceA> but not <sourceB> (shallow
 *      copied).
 */
RedSet RedSet_NewDifference(RedSet sourceA, RedSet sourceB);

/*
 * RedSet_NewSymmetricDifference - Create a new set that is the symmetric
 *      difference of two existing sets.  The symmetric difference is defined
 *      as all elements in one set, but not the other.
 *      
 *      <sourceA> and <sourceB> are the two sets to take the symmetric
 *          difference of.  Both must be non-NULL.
 *
 *      Returns a newly allocated RedSet object handle, or NULL if memory
 *      allocation failed.  On success, the returned set will contain all
 *      elements that are found in exactly one of <sourceA> or <sourceB>
 *      (shallow copied).
 */
RedSet RedSet_NewSymmetricDifference(RedSet sourceA, RedSet sourceB);

/*
 * RedSet_Free - Free memory used by <set>.
 *
 *      This includes clearing the set, and freeing all overhead data.
 *
 *      Does nothing if <set> is NULL.
 */
void RedSet_Free();

/*
 * RedSet_NumItems - Count number of elements in a set.
 *
 *      <set> is the set to check.
 *
 *      Returns the number of elements in <set>.
 */
unsigned RedSet_NumItems(RedSet set);

/*
 * RedSet_IsEmpty - Determine if a set is empty.
 *
 *      <set> is the set to check.
 *
 *      Returns true if there are 0 elements in <set>, false otherwise.
 */
bool RedSet_IsEmpty(RedSet set);

/*
 * RedSet_Clear - Remove all elements from a set.
 *
 *      <set> is the set clear.
 *
 *      All elements are removed from <set>, leaving an empty set.
 */
void RedSet_Clear(RedSet set);

/*
 * RedSet_Add - Add an element to a set.  The element may be any size and
 *      datatype, and will be copied internally.  Does nothing if element is
 *      already in the set.
 *
 *      <set> is the set to add to.
 *
 *      <pItem> is a pointer to the bytes representing the element.
 *
 *      <itemSize> is the size, in bytes, of the element, specifying the number
 *          of bytes that will be copied.
 *
 *      Returns true if the element was added, or was already part of the set.
 *      Returns false if memory allocation failed.
 *
 *      This must not be called while you are iterating over the set using a
 *      RedSetIterator_t object.
 */
bool RedSet_Add(RedSet set, const void *pItem, size_t itemSize);

/*
 * RedSet_AddS - Add a string element to a set.
 *
 *      This is equivalent to:
 *      -----------------------------------------------------------------------
 *      RedSet_Insert(set, szItem, strlen(szItem));
 *      -----------------------------------------------------------------------
 *
 *      <set> is the set to add to.
 *
 *      <szItem> is a NULL-terminated string to add.
 *
 *      Returns true if the element was added, or was already part of the set.
 *      Returns false if memory allocation failed.
 *
 *      This must not be called while you are iterating over the set using a
 *      RedSetIterator_t object.
 */
bool RedSet_AddS(RedSet set, const char *szItem);

/*
 * RedSet_Remove -- Remove an element from a set.
 *
 *      <set> is the set to remove from.
 *
 *      <pItem> is a pointer to the bytes representing the element to remove.
 *
 *      <itemSize> is the size, in bytes, of the element.
 *
 *      Returns true if the element was succesfully removed.
 *      Returns false if the element was not in the set.
 *
 *      This must not be called while you are iterating over the set using a
 *      RedSetIterator_t object.
 */
bool RedSet_Remove(RedSet set, const void *pItem, size_t itemSize);

/*
 * RedSet_RemoveS -- Remove a string element from a set.
 *
 *      This is equivalent to:
 *      ----------------------------------------------------------------------
 *      RedSetRemove(set, szItem, strlen(szItem) + 1)
 *      ----------------------------------------------------------------------
 *
 *      <set> is the set to remove from.
 *
 *      <szItem> is the string element to remove.
 *
 *      Returns true if the element was succesfully removed.
 *      Returns false if the element was not in the set.
 *
 *      This must not be called while you are iterating over the set using a
 *      RedSetIterator_t object.
 */
bool RedSet_RemoveS(RedSet set, const char *szItem);

/*
 * RedSet_HasItem -- Check if set contains an element.
 *
 *      <set> is the set to check.
 *
 *      <pItem> is a pointer to the bytes representing the element to look for.
 *
 *      <itemSize> is the size, in bytes, of the element.
 *
 *      Returns true if the element is in the set.  Both the value of
 *      <itemSize> as well as the value of the first <itemSize> bytes starting
 *      from <pItem> must match an element in the set exactly.
 *
 *      Returns false if the element is not in the set.
 */
bool RedSet_HasItem(RedSet set, const void *pItem, size_t itemSize);

/*
 * RedSet_HasItemS -- Check if set contains a string element.
 *
 *      This is equivalent to:
 *      ----------------------------------------------------------------------
 *      RedSetHashItem(set, szItem, strlen(szItem) + 1)
 *      ----------------------------------------------------------------------
 *
 *      <set> is the set to check.
 *
 *      <szItem> is a NULL-terminated string element to look for.
 *
 *      Returns true if the element is in the set.
 *      Returns false if the element is not in the set.
 */
bool RedSet_HasItemS(RedSet set, const char *szItem);

/*
 * RedSet_IsEqual -- Check if two sets are equal.
 *
 *      Two sets are equal iff they have the same number of elements and the
 *      elements are exactly the same.
 *
 *      <setA> and <setB> are the sets to compare.
 *
 *      Returns true if <setA> and <setB> are equal.
 *      Returns false otherwise.
 */
bool RedSet_IsEqual(RedSet setA, RedSet setB);

/*
 * RedSet_IsSubset -- Check if <setA> is a subset of <setB>.
 *
 *      <setA> and <setB> are the sets to compare.
 *
 *      Returns true if <setA> and <setB> are equal or if <setA> is a subset of
 *      <setB>.
 *      Returns false otherwise.
 */
bool RedSet_IsSubset(RedSet setA, RedSet setB);

/*
 * RedSet_IsSubset -- Check if <setA> is a superset of <setB>.
 *
 *      <setA> and <setB> are the sets to compare.
 *
 *      Returns true if <setA> and <setB> are equal or if <setA> is a superset
 *      of <setB>.
 *      Returns false otherwise.
 */
bool RedSet_IsSuperset(RedSet setA, RedSet setB);

/*
 * RedSetIterator_Init -- Initialize a set iterator object.
 *
 *      <pIter> is a pointer to the iterator object to initialize.  This
 *      typically points to an automatic RedSetIter_t variable (on the stack).
 *
 *      <set> is the set to iterate over.
 */
void RedSetIterator_Init(RedSetIter_t *pIter, RedSet set);

/*
 * RedSetIterator_Advance -- Advance a set iterator object.
 *
 *      <pIter> is a pointer to the iterator object to advance.  This typically
 *          points to an automatic RedSetIter_t variable (on the stack).
 *
 *      <ppOutItem> address of (void *) variable that will be set to point to
 *          the next element's data.
 *
 *      <pOutItem> address of (size_t) variable that will be set to the next
 *          element's size.
 *
 *      Returns false if the end of the iteration has been reached.
 */
bool RedSetIterator_Advance(RedSetIterator_t *pIter, const void **ppOutItem, size_t *pOutSize);

/*
 * RED_SET_FOREACH -- Macro for iterating over all elements in a set.
 *
 *      <iter> is a RedSetIterator_t object that will hold the iteration state.
 *
 *      <set> is the set to iterate over.
 *
 *      <ppItem> is address of (void *) variable that will be set to point to the
 *          current element's data, each iteration of the loop.
 *
 *      <pItemSize> is address of (size_t) variable that will be set to the
 *          current element's size current element's data, each iteration of the loop.
 */
#define RED_SET_FOREACH(iter, set, ppItem, pItemSize) \
    for (RedSetIter_Init(&(iter), (set)); RedSetIter_Advance(&(iter), (ppItem), (pItemSize)))

#ifdef __cplusplus
}
#endif

#endif
