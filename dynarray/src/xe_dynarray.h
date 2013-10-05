/*
 * DynArray -- Generic, fast, easy-to-use dynamic array module.
 *
 *  OVERVIEW
 *
 *      The DynArray module provides a mechanism for creating dynamic arrays of
 *      any data type.  The dynamic arrays can be indexed just like
 *      conventional arrays.
 *
 *  CREATING, INDEXING, AND FREEING A DYNAMIC ARRAY
 *
 *      To create a dynamic array, call DynArray_Create, and cast the returned
 *      pointer to the desired data type.  For example:
 *
 *          int *pArray = (int *)DynArray_Create(sizeof(int), 5, 0);
 *
 *      This creates a dynamic array of signed integers that initially has 5
 *      (unintialized) elements.  The last parameter is a hint to the
 *      implementation specifying the minimum amount of memory to keep
 *      allocated (if unsure, just use 0).
 *
 *      A dynamic array can be created from any data type:
 *
 *          Sphere *pSpheres = (Sphere *)DynArray_Create(sizeof(Sphere), 1, 0);
 *
 *      The pArray can be indexed just like a normal array:
 *
 *          pArray[0] = 10;
 *          foo = pSpheres[4].radius;
 *
 *      Call DynArray_Free to free the dynamic array:
 *
 *          DynArray_Free(pArray)
 *
 *  RESIZING OPERATIONS
 *
 *      Dynamic arrays can be resized at runtime.
 *
 *      The dynamic array's current size is obtained by calling:
 *
 *          DynArray_NumItems(pArray)
 *
 *      To grow the array, call:
 *
 *          DYNARRAY_GROW_BY_ONE(pArray);
 *      or
 *          DYNARRAY_GROW(pArray, n);    // add n elements
 *      or
 *          DYNARRAY_APPEND(pArray, v);  // append value v (growing by 1)
 *
 *      These routines are macros because they may change the value of pArray
 *      if the underlying realloc call moves the memory block.  Because the
 *      value of pArray, and the underlying memory allocation, can change, you
 *      must exercise extreme caution if using secondary pointers to dynamic
 *      array elements (the general advice here is to just not do it!).
 *
 *      The DYNARRAY_GROW_BY_ONE and DYNARRAY_GROW routines increase the size
 *      of the array, but do not initialize the new elements.
 *
 *      To shrink the array, call:
 *
 *          DYNARRAY_SHRINK_BY_ONE(pArray);
 *      or
 *          DYNARRAY_SHRINK(pArray, n);  // remove n elements
 *      or
 *          DYNARRAY_POP(pArray, &v);    // pop last element (shrinking by 1)
 *          
 *      You can also resize the array to a specific size using:
 *
 *          DYNARRAY_RESIZE(pArray, n);   // set array size to n
 *
 */
#ifndef XE_DYNARRAY_INCLUDED
#define XE_DYNARRAY_INCLUDED

 /* 
  * DynArrayHandle - Represents a generic dynamic array.  The DynArrayHandle
  *     should be cast to the specific pointer type of your array.
  */
typedef void * DynArrayHandle;

/*
 * DynArray_Create - Create a new dynamic array, returning a pointer to the
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
 *      Returns a DynArrayHandle that can be cast to the desired pointer data
 *      type, and then indexed like a conventional array.
 */
DynArrayHandle
    DynArray_Create(
        unsigned elemSize, 
        unsigned startNumItems,
        unsigned floorHint);

/*
 * DynArray_Free - Free a dynamic array.
 *
 *      <hDynArray> specifies the dynamic array to free.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting is required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *          If NULL, this routine does nothing.
 */
void DynArray_Free(DynArrayHandle hDynArray);

/*
 * DynArray_NumItems - Get the number of elements in a dynamic array.
 *
 *      <hDynArray> specifies the dynamic array to check.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      Returns the number of elements in the array.
 */
unsigned DynArray_NumItems(DynArrayHandle hDynArray);

/*
 * DynArray_ElemSize - Get the size of each array element.
 *
 *      <hDynArray> specifies the dynamic array to check.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      Returns the size of an individual array element, as passed to
 *          DynArray_Create in the <elemSize> parameter.
 */
unsigned DynArray_ElemSize(DynArrayHandle hDynArray);

/*
 * DYNARRAY_GROW_BY_ONE - Increase the size of the array by one (1) and update
 *      the array pointer.
 *
 *      The new element is appended to the end unitialized.
 *
 *      This routine is slightly more efficient than calling
 *      DYNARRAY_GROW(hDynArray, 1)
 *
 *      <hDynArray> specifies the dynamic array to grow.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      The funcion version (DynArray_GrowByOne) returns a new DynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
DynArrayHandle DynArray_GrowByOne(DynArrayHandle hDynArray);
#define DYNARRAY_GROW_BY_ONE(hDynArray) \
    do { \
        (hDynArray) = DynArray_GrowByOne(hDynArray); \
    } while (0)

/*
 * DYNARRAY_SHRINK_BY_ONE - Decrease the size of the array by one (1) and
 *      update the array pointer.
 *
 *      One element is removed from the end, and its value lost.
 *
 *      This routine is slightly more efficient than calling
 *      DYNARRAY_SHRINK(hDynArray, 1).
 *
 *      This will assert if <hDynArray> has 0 elements when called.
 *
 *      <hDynArray> specifies the dynamic array to shrink.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      The funcion version (DynArray_ShrinkByOne) returns a new DynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
DynArrayHandle DynArray_ShrinkByOne(DynArrayHandle hDynArray);
#define DYNARRAY_SHRINK_BY_ONE(hDynArray) \
    do { \
        (hDynArray) = DynArray_ShrinkByOne(hDynArray); \
    } while (0)

/*
 * DYNARRAY_GROW - Increase the size of a dynamic array and update the array
 *      pointer.
 *
 *      The new elements are appended to the end unitialized.
 *
 *      <hDynArray> specifies the dynamic array to grow.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).
 *
 *      <numItemsToAdd> specifies the number of new elements to append to the
 *          end.  It is valid for <numItemsToAdd> to be 0 (in which case the
 *          array is unmodified, but its address may change).
 *
 *      The funcion version (DynArray_GrowByOne) returns a new DynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
DynArrayHandle DynArray_Grow(DynArrayHandle hDynArray, unsigned numItemsToAdd);
#define DYNARRAY_GROW(hDynArray, numItemsToAdd) \
    do { \
        (hDynArray) = DynArray_Grow(hDynArray, numItemsToAdd); \
    } while (0)

/*
 * DYNARRAY_SHRINK - Decrease the size of a dynamic array and update the array
 *      pointer.
 *
 *      Elements are removed from the end, and their values lost.
 *
 *      This will assert if shrinking by the specified amount would result in a
 *      dynamic array with fewer than 0 elements.
 *
 *      <hDynArray> specifies the dynamic array to shrink.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      <numItemsToRemove> specifies the number of elements to remove from the
 *          end.  It is valid for <numItemsToAdd> to be 0 (in which case the
 *          array is unmodified, but its address may change).
 *
 *      The funcion version (DynArray_ShrinkByOne) returns a new DynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
DynArrayHandle DynArray_Shrink(DynArrayHandle hDynArray, unsigned numItemsToRemove);
#define DYNARRAY_SHRINK(hDynArray, numItemsToRemove) \
    do { \
        (hDynArray) = DynArray_Shrink(hDynArray, numItemsToRemove); \
    } while (0)

/*
 * DYNARRAY_RESIZE - Resize a dynamic array and update the array pointer.
 *
 *      If the array grows, then new elements are appended to the end
 *      unitialized.
 *
 *      If the array shrinks, then elements are removed from the end, and their
 *      values lost.
 *
 *      <hDynArray> specifies the dynamic array to resize.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 *      <numItems> specifies the desired number of elements in the array.
 *
 *      The funcion version (DynArray_Resize) returns a new DynArrayHandle
 *      that should be used in place of <hDynArray> going forward.
 *
 *      The macro version (recommended) automatically reasigns <hDynArray> to
 *      the new array address (and does not return anything).  The macro
 *      references <hDynArray> twice, so avoid complex expressions for that
 *      parameter.
 */
DynArrayHandle DynArray_Resize(DynArrayHandle hDynArray, unsigned numItems);
#define DYNARRAY_RESIZE(hDynArray, numItems) \
    do { \
        (hDynArray) = DynArray_Shrink(hDynArray, numItems); \
    } while (0)

/*
 * DYNARRAY_TAIL - Macro for accessing the last element of a dynamic array.
 *
 *      This macro indexes (dereferences) the last element of the array.
 *
 *      It expands to a modifiable lvalue that can used in assignments.  For
 *      example:
 *
 *          DYNARRAY_TAIL(pIntDynArray) = 43;
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
 *
 */
#define DYNARRAY_TAIL(hDynArray) \
    ((hDynArray)[DynArray_NumItems(hDynArray) - 1])

/*
 * DYNARRAY_APPEND - Increase the size of the dynarray by one (1) and set the
 *      last element.
 *
 *      It expands to a modifiable lvalue, and can used like so:
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
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
#define DYNARRAY_APPEND(hDynArray, value) \
    do { \
        DYNARRAY_GROW_BY_ONE(hDynArray); \
        DYNARRAY_TAIL(hDynArray) = (value); \
    } while (0)

/*
 * DYNARRAY_POP - Decrease the size of the dynarray by one (1) and return the
 *      element that was removed.
 *
 *      <hDynArray> specifies the dynamic array to index.  This should be a
 *          pointer returned by DynArray_Create or the most recent resizing
 *          operation (whichever happened most recently).  Typically, the
 *          caller will have already cast the DynArrayHandle to a concrete
 *          pointer type (such as (int *) or (Sphere *)) and use that concrete
 *          pointer for this parameter (no casting required because
 *          DynArrayHandle is typedef'd to (void *)).  
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
#define DYNARRAY_POP(a, pv) \
    do { \
        *(pv) = DYNARRAY_TAIL(a); \
        DYNARRAY_SHRINK_BY_ONE(a); \
    } while (0)

#endif
