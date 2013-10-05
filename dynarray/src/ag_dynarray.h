/*
 * AgDynArray -- Generic, fast, easy-to-use dynamic array module.
 *
 *  OVERVIEW
 *
 *      The AgDynArray module provides a mechanism for creating dynamic arrays of
 *      any data type.  The dynamic arrays can be indexed just like
 *      conventional arrays.
 *
 *  CREATING, INDEXING, AND FREEING A DYNAMIC ARRAY
 *
 *      To create a dynamic array, call AgDynArray_Create, and cast the returned
 *      pointer to the desired data type.  For example:
 *
 *          int *pArray = (int *)AgDynArray_Create(sizeof(int), 5, 0);
 *
 *      This creates a dynamic array of signed integers that initially has 5
 *      (unintialized) elements.  The last parameter is a hint to the
 *      implementation specifying the minimum amount of memory to keep
 *      allocated (if unsure, just use 0).
 *
 *      A dynamic array can be created from any data type:
 *
 *          Sphere *pSpheres = (Sphere *)AgDynArray_Create(sizeof(Sphere), 1, 0);
 *
 *      The pArray can be indexed just like a normal array:
 *
 *          pArray[0] = 10;
 *          foo = pSpheres[4].radius;
 *
 *      Call AgDynArray_Free to free the dynamic array:
 *
 *          AgDynArray_Free(pArray)
 *
 *  RESIZING OPERATIONS
 *
 *      Dynamic arrays can be resized at runtime.
 *
 *      The dynamic array's current size is obtained by calling:
 *
 *          AgDynArray_NumItems(pArray)
 *
 *      To grow the array, call:
 *
 *          AG_DYNARRAY_GROW_BY_ONE(pArray);
 *      or
 *          AG_DYNARRAY_GROW(pArray, n);    // add n elements
 *      or
 *          AG_DYNARRAY_APPEND(pArray, v);  // append value v (growing by 1)
 *
 *      These routines are macros because they may change the value of pArray
 *      if the underlying realloc call moves the memory block.  Because the
 *      value of pArray, and the underlying memory allocation, can change, you
 *      must exercise extreme caution if using secondary pointers to dynamic
 *      array elements (the general advice here is to just not do it!).
 *
 *      The AG_DYNARRAY_GROW_BY_ONE and AG_DYNARRAY_GROW routines increase the size
 *      of the array, but do not initialize the new elements.
 *
 *      To shrink the array, call:
 *
 *          AG_DYNARRAY_SHRINK_BY_ONE(pArray);
 *      or
 *          AG_DYNARRAY_SHRINK(pArray, n);  // remove n elements
 *      or
 *          AG_DYNARRAY_POP(pArray, &v);    // pop last element (shrinking by 1)
 *          
 *      You can also resize the array to a specific size using:
 *
 *          AG_DYNARRAY_RESIZE(pArray, n);   // set array size to n
 *
 */
#ifndef AG_AG_DYNARRAY_INCLUDED
#define AG_AG_DYNARRAY_INCLUDED

 /* 
  * AgDynArrayHandle - Represents a generic dynamic array.  The AgDynArrayHandle
  *     should be cast to the specific pointer type of your array.
  */
typedef void * AgDynArrayHandle;

/*
 * AgDynArray_Create - Create a new dynamic array, returning a pointer to the
 *      underlying conventional array.
 *
 *      <elemSize> is the size (in bytes) of each array element.
 *
 *      <startNumItems> is the initial number of (unitialized) elements that
 *          the array contains.  It is valid for <startNumItems> to be 0.
 *
 *      <floorHint> is a hint to the implenentation (that may be ignored),
 *          specifying the minimum number of elements to keep allocated
 *          internally, even if the array shrinks below this size.
 *
 *      Returns a AgDynArrayHandle that can be cast to the desired pointer data
 *      type, and then indexed like a conventional array.
 */
AgDynArrayHandle
    AgDynArray_Create(
        unsigned elemSize, 
        unsigned startNumItems,
        unsigned floorHint);

/*
 * AgDynArray_Free - Free a dynamic array.
 *
 *      <hDynArray> specifies the dynamic array to free.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting is required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *          If NULL, this routine does nothing.
 */
void AgDynArray_Free(AgDynArrayHandle hDynArray);

/*
 * AgDynArray_NumItems - Get the number of elements in a dynamic array.
 *
 *      <hDynArray> specifies the dynamic array to check.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      Returns the number of elements in the array.
 */
unsigned AgDynArray_NumItems(AgDynArrayHandle hDynArray);

/*
 * AgDynArray_ElemSize - Get the size of each array element.
 *
 *      <hDynArray> specifies the dynamic array to check.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      Returns the size of an individual array element, as passed to
 *          AgDynArray_Create in the <elemSize> parameter.
 */
unsigned AgDynArray_ElemSize(AgDynArrayHandle hDynArray);

/*
 * AG_DYNARRAY_GROW_BY_ONE - Increase the size of the array by one (1) and update
 *      the array pointer.
 *
 *      The new element is appended to the end unitialized.
 *
 *      This routine is slightly more efficient than calling
 *      AG_DYNARRAY_GROW(hDynArray, 1)
 *
 *      <hDynArray> specifies the dynamic array to grow.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      The funcion version (AgDynArray_GrowByOne) returns a new AgDynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
AgDynArrayHandle AgDynArray_GrowByOne(AgDynArrayHandle hDynArray);
#define AG_DYNARRAY_GROW_BY_ONE(hDynArray) \
    do { \
        (hDynArray) = AgDynArray_GrowByOne(hDynArray); \
    } while (0)

/*
 * AG_DYNARRAY_SHRINK_BY_ONE - Decrease the size of the array by one (1) and
 *      update the array pointer.
 *
 *      One element is removed from the end, and its value lost.
 *
 *      This routine is slightly more efficient than calling
 *      AG_DYNARRAY_SHRINK(hDynArray, 1).
 *
 *      This will assert if <hDynArray> has 0 elements when called.
 *
 *      <hDynArray> specifies the dynamic array to shrink.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      The funcion version (AgDynArray_ShrinkByOne) returns a new AgDynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
AgDynArrayHandle AgDynArray_ShrinkByOne(AgDynArrayHandle hDynArray);
#define AG_DYNARRAY_SHRINK_BY_ONE(hDynArray) \
    do { \
        (hDynArray) = AgDynArray_ShrinkByOne(hDynArray); \
    } while (0)

/*
 * AG_DYNARRAY_GROW - Increase the size of a dynamic array and update the array
 *      pointer.
 *
 *      The new elements are appended to the end unitialized.
 *
 *      <hDynArray> specifies the dynamic array to grow.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).
 *
 *      <numItemsToAdd> specifies the number of new elements to append to the
 *          end.  It is valid for <numItemsToAdd> to be 0 (in which case the
 *          array is unmodified, but its address may change).
 *
 *      The funcion version (AgDynArray_GrowByOne) returns a new AgDynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
AgDynArrayHandle AgDynArray_Grow(AgDynArrayHandle hDynArray, unsigned numItemsToAdd);
#define AG_DYNARRAY_GROW(hDynArray, numItemsToAdd) \
    do { \
        (hDynArray) = AgDynArray_Grow(hDynArray, numItemsToAdd); \
    } while (0)

/*
 * AG_DYNARRAY_SHRINK - Decrease the size of a dynamic array and update the array
 *      pointer.
 *
 *      Elements are removed from the end, and their values lost.
 *
 *      This will assert if shrinking by the specified amount would result in a
 *      dynamic array with fewer than 0 elements.
 *
 *      <hDynArray> specifies the dynamic array to shrink.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      <numItemsToRemove> specifies the number of elements to remove from the
 *          end.  It is valid for <numItemsToAdd> to be 0 (in which case the
 *          array is unmodified, but its address may change).
 *
 *      The funcion version (AgDynArray_ShrinkByOne) returns a new AgDynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
AgDynArrayHandle AgDynArray_Shrink(AgDynArrayHandle hDynArray, unsigned numItemsToRemove);
#define AG_DYNARRAY_SHRINK(hDynArray, numItemsToRemove) \
    do { \
        (hDynArray) = AgDynArray_Shrink(hDynArray, numItemsToRemove); \
    } while (0)

/*
 * AG_DYNARRAY_RESIZE - Resize a dynamic array and update the array pointer.
 *
 *      If the array grows, then new elements are appended to the end
 *      unitialized.
 *
 *      If the array shrinks, then elements are removed from the end, and their
 *      values lost.
 *
 *      <hDynArray> specifies the dynamic array to resize.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      <numItems> specifies the desired number of elements in the array.
 *
 *      The funcion version (AgDynArray_Resize) returns a new AgDynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
AgDynArrayHandle AgDynArray_Resize(AgDynArrayHandle hDynArray, unsigned numItems);
#define AG_DYNARRAY_RESIZE(hDynArray, numItems) \
    do { \
        (hDynArray) = AgDynArray_Shrink(hDynArray, numItems); \
    } while (0)

/*
 * AG_DYNARRAY_TAIL - Macro for accessing the last element of a dynamic array.
 *
 *      This macro indexes (dereferences) the last element of the array.
 *
 *      It expands to a modifiable lvalue that can used in assignments.  For
 *      example:
 *
 *          AG_DYNARRAY_TAIL(pIntAgDynArray) = 43;
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 */
#define AG_DYNARRAY_TAIL(hDynArray) \
    ((hDynArray)[AgDynArray_NumItems(hDynArray) - 1])

/*
 * AG_DYNARRAY_APPEND - Increase the size of the dynarray by one (1) and set the
 *      last element.
 *
 *      It expands to a modifiable lvalue, and can used like so:
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      <value> is the value (of the appropriate type) to set the last element
 *          to.
 *
 *      This macro automatically reasigns <hDynArray> to the new array address.
 *      The macro references <hDynArray> three times, so avoid complex
 *      expressions for that parameter.
 *
 *      For this macro to work, the array elements must have a data type that
 *      is assignable (for example, fixed arrays will not work, but structures
 *      will).
 */
#define AG_DYNARRAY_APPEND(hDynArray, value) \
    do { \
        AG_DYNARRAY_GROW_BY_ONE(hDynArray); \
        AG_DYNARRAY_TAIL(hDynArray) = (value); \
    } while (0)

/*
 * AG_DYNARRAY_POP - Decrease the size of the dynarray by one (1) and return the
 *      element that was removed.
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by AgDynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the AgDynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          AgDynArrayHandle is typedef'd to (void *)).  
 *
 *      <value> is the value (of the appropriate type) to set the last element
 *          to.
 *
 *      This macro automatically reasigns <hDynArray> to the new array address.
 *      The macro references <hDynArray> three times, so avoid complex
 *      expressions for that parameter.
 *
 *      For this macro to work, the array elements must have a data type that
 *      is assignable (for example, fixed arrays will not work, but structures
 *      will).
 */
#define AG_DYNARRAY_POP(a, pv) \
    do { \
        *(pv) = AG_DYNARRAY_TAIL(a); \
        AG_DYNARRAY_SHRINK_BY_ONE(a); \
    } while (0)

#endif
