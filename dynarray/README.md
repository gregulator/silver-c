AgDynArray - The awesomest dynamic array implementation
---------------------------------------------------------

AgDynArray implements dynamically-sized arrays in C that have the speed and
ease-of-use of conventional C arrays.

See full documention in the header file [here](src/ag_dynarray.h).

**OVERVIEW**

The AgDynArray module provides a mechanism for creating dynamic arrays of
any data type.  The dynamic arrays can be indexed just like
conventional arrays.

**CREATING, INDEXING, AND FREEING A DYNAMIC ARRAY**

To create a dynamic array, call `AgDynArray_Create`, and cast the returned
pointer to the desired data type.  For example:

    int *pArray = (int *)AgDynArray_Create(sizeof(int), 5, 0);

This creates a dynamic array of signed integers that initially has 5
(unintialized) elements.  The last parameter is a hint to the implementation
specifying the minimum amount of memory to keep allocated (if unsure, just use
0).

A dynamic array can be created from any data type:
    
    typedef struct 
    {
        float x, y, z;
        float radius;
    } Sphere;

    ...

    Sphere *pSpheres = (Sphere *)AgDynArray_Create(sizeof(Sphere), 1, 0);

The `pArray` can be indexed just like a normal array:

    pArray[0] = 10;
    foo = pSpheres[4].radius;

Call `AgDynArray_Free` to free the dynamic array:

    AgDynArray_Free(pArray);

**RESIZING OPERATIONS**

Dynamic arrays can be resized at runtime.

The dynamic array's current size is obtained by calling:

    AgDynArray_NumItems(pArray)

This returns the number of elements in the array, not the number of bytes storage.

To grow the array, call:

    AG_DYNARRAY_GROW_BY_ONE(pArray);

    or

    AG_DYNARRAY_GROW(pArray, n);    // add n elements

    or

    AG_DYNARRAY_APPEND(pArray, v);  // append value v (growing by 1 element)

**IMPORTANT: These routines are macros because they may change the value of
pArray if the underlying realloc call moves the memory block.  Because the
value of pArray, and the underlying memory allocation, can change, you must
exercise extreme caution if using secondary pointers to dynamic array elements
(the general advice here is to just not do it!).**

    Bottom line: Never take the address of a dynamic array element, unless you
    can guarantee you'll be finished using the address before performing any
    resize operations.

The `AG_DYNARRAY_GROW_BY_ONE` and `AG_DYNARRAY_GROW` routines increase the size
of the array, but do not initialize the new elements.

To shrink the array, call:

    AG_DYNARRAY_SHRINK_BY_ONE(pArray);

    or

    AG_DYNARRAY_SHRINK(pArray, n);  // remove n elements

    or

    AG_DYNARRAY_POP(pArray, &v);    // pop last element (shrinking by 1)

You can also resize the array to a specific size using:

    AG_DYNARRAY_RESIZE(pArray, n);   // set array size to n elements
