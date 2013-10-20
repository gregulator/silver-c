ZARRAY Reference
---------------------------

ZARRAY provides a high-performance implementation of dynamic arrays using C
macros.

The following macros are provided:

    `ZARRAY_NEW` - Create new dynamic array.
    `ZARRAY_FREE` - Free dynamic array.

    `ZARRAY_NUM_ITEMS` - Count number of elements in dynamic array.

    `ZARRAY_AT` - Access array element by index.
    `ZARRAY_TAIL` - Access last element of array.

    `ZARRAY_APPEND` - Add element to end of array.
    `ZARRAY_POP` - Remove last element of array, and return its value.

    `ZARRAY_RESIZE` - Grow or shrink dynamic array to target size.
    `ZARRAY_GROW` - Grow dynamic array.
    `ZARRAY_GROW_BY_ONE` - Grow by 1 element.
    `ZARRAY_SHRINK` - Shrink dynamic array.
    `ZARRAY_SHRINK_BY_ONE` - Shrink by 1 element.

ZARRAY Details
---------------------------

<hr></hr>
<h3>ZARRAY_APPEND macro.</h3>

Add value to end of a dynamic array, growing the dynamic array by 1 element.

    ZARRAY_APPEND(<zarray>, <value>)

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to append to.
  - `<value>` is the value to append.  This must be same dataype as passed to `ZARRAY_NEW`.

*Return Value*

  - *none*

*Evaluates To (Approximately):*

    do {
        ZARRAY_GROW_BY_ONE(zarray);
        ZARRAY_TAIL(zarray) = (value);
    } while (0)

*Usage Notes*

  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 0);
        ZARRAY_APPEND(myArray, 14.25f);
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.


<hr></hr>
<h3>ZARRAY_AT macro.</h3>

Access dynamic array element by index.

    ZARRAY_AT(<zarray>, <index>)

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to pop from.
  - `<index>` is an non-negative integer specifying the array element to access
  (0 is the first element).

*Return Value*

  - Evaluates to a modifiable l-value that is the element at position `<index>`
  in the dynamic array.

*Evaluates To (Approximately):*

    ((zarray)->item[(index)])

This implementation takes advantage of the fact that when shrinking, the actual
storage allocated remains above 2*numItems (so item[numItems+1] is always still
allocated, underneath).

*Usage Notes*

  - Bounds checking is only performed if debbuging is enabled.
  - If debugging is enabled, this macro evaluates `<zarray>` multiple times.
  Avoid passing an argument that is computationally expensive or has side
  effects.

*Example*

    {
        int i;
        ZARRAY(int) myArray = ZARRAY_NEW(float, 10);
        for (i = 0; i < 10; i++)
            ZARRAY_AT(myArray, i) = i*i;
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Asserts that `<index>` is within bounds, between `0` and
  `ZARRAY_NUM_ITEMS(<zarray>) - 1` inclusive.
  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.


<hr></hr>
<h3>ZARRAY_FREE macro.</h3>

Free a dynamic array.

    ZARRAY_FREE(<zarray>)
    
*Parameters*

  - `<zarray>` is a pointer to the dynamic array object you wish to free, or NULL.

*Return Value*

  - *none*

*Usage Notes*

  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.
  - Does nothing if `<zarray>` is NULL.

*Example*

    {
        ZARRAY(char) myArray = ZARRAY_NEW(char, 1024);
        ZARRAY_FREE(myArray);
    }
 
*Debug Checks*

  - If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then does a
  special check asserting that `<zarray>` is, in fact, a dynamic array object
  created with `ZARRAY_NEW`.


<hr></hr>
<h3>ZARRAY_GROW macro.</h3>

Increase the size of a dynamic array.

    ZARRAY_GROW(<zarray>, <numItemsToAdd>)

The newly appended elements are unitialized.

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to grow.
  - `<numItemsToAdd>` is an unsigned integer specifying the number of elements
  to add.

*Return Value*

  - *none*

*Usage Notes*

  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 5);
        ZARRAY_GROW(myArray, 3);
        assert(ZARRAY_NUM_ITEMS(myArray) == 8);
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.
  - Asserts that `<numItemsToAdd>` is non-negative.


<hr></hr>
<h3>ZARRAY_GROW_BY_ONE macro.</h3>

Increase the size of a dynamic array by one (1).

    ZARRAY_GROW_BY_ONE(<zarray>)

The newly appended element is unitialized.

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to grow.

*Return Value*

  - *none*

*Usage Notes*

  - This macro is slightly more efficient than calling 
  `ZARRAY_GROW(zarray, 1)`.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 0);
        ZARRAY_GROW_BY_ONE(myArray);
        assert(ZARRAY_NUM_ITEMS(myArray) == 1);
    }

*Debug Checks*

  - If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then does a
  special check asserting that `<zarray>` is, in fact, a dynamic array object
  created with `ZARRAY_NEW`.



<hr></hr>
<h3>ZARRAY_NEW macro.</h3>

Create a new dynamic array.

    void * ZARRAY_NEW(<datatype>, <startNumItems>)

*Parameters:*

  - `<datatype>` is the desired datatype of the array elements.  For example
  `unsigned int` or `struct Sphere`. 
  - `<startNumItems>` is an unsigned integer specifying the initial array size.
  Use `0` to create an empty array.

*Returns:*

  - Pointer to newly allocated dynamic array object, or `NULL` if memory
  allocation failed.
  - Should be assigned to a `ZARRAY(<datatype>)` variable.

*Usage Notes*

  - Array elements are uninitialized and contain undefined data.

*Example*

    ZARRAY(long double) myArray = ZARRAY_NEW(long double, 10);
    
*C++ specific:*

Unlike C, C++ requires you to explicitely cast `void *` to the appropriate
pointer type.  Furthermore, C++ does not allow anonymous structures in casts.
To get around these issues, the `C++` implementation uses templates.  If
`__cplusplus` is defined the macro instead uses a template return type:

    _ZArrayStruct<datatype> ZARRAY_NEW(<datatype>, <startNumItems>)



<hr></hr>
<h3>ZARRAY_NUM_ITEMS macro.</h3>

Get number of array elements in a dynamic array.

    unsigned ZARRAY_NUM_ITEMS(<zarray>)

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object that you are interested
  in.

*Return Value*

  - Unsigned integer number of "usable" array elements in the dynamic array.

*Usage Notes*

  - The underlying implementation internally allocates memory for up to twice
  the returned number of elements, in order to reduce the number of calls to
  `realloc`.  However, the caller must not rely on this behavior or ever access
  those elements.
  - This macro evaluates `<zarray>` only once.

*Evaluates To:*

    (((unsigned)(zarray)->numItems))

  - The `(unsigned)` cast is there to prevent this from being used as an l-value.
  - If debugging is enable (ie: `Z4C_NO_DEBUG` is undefined) the evaluation is
  more complex.

*Example*

    {
        ZARRAY(FILE *) myFileArray = ZARRAY_NEW(FILE *, 30);
        unsigned numItems = ZARRAY_NUM_ITEMS(myFileArray);
        assert(numItems == 30);
    }

*Debug Checks*

  - If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then does a
  special check asserting that `<zarray>` is, in fact, a dynamic array object
  created with `ZARRAY_NEW`.


<hr></hr>
<h3>ZARRAY_POP macro.</h3>

Remove value from end of a dynamic array, and return the removed value.

    ZARRAY_POP(<zarray>)

This decreases the size of the dynamic array by one (1) element.

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to pop from.
  - `<variable>` is the variable (l-value) that gets assigned the popped value.
  This must be same dataype as passed to `ZARRAY_NEW`.

*Return Value*

  - Returns the value that was stored in the last element of the array.  The
  datatype of this value matches the datatype passed to `ZARRAY_NEW`.

*Evaluates To (Approximately):*

    (   
        ZARRAY_SHRINK_BY_ONE(zarray),
        (zarray)->item[(zarray)->numItems]
    )

This implementation takes advantage of the fact that when shrinking, the actual
storage allocated remains above 2*numItems (so item[numItems+1] is always still
allocated internally).

*Usage Notes*

  - The dynamic array must have at least 1 element, or your program will do
  Very Bad things.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        float value;
        ZARRAY(float) myArray = ZARRAY_NEW(float, 1);
        ZARRAY_AT(myArray, 0) = 73.2f;
        value = ZARRAY_POP(myArray);
        assert(value == 73.2f)
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Asserts that `<zarray>` contains at least one (1) element.
  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.



<hr></hr>
<h3>ZARRAY_RESIZE macro.</h3>

Resize a dynamic array.

    ZARRAY_RESIZE(<zarray>, <newSize>)

If `<newSize>` is larger than `ZARRAY_NUM_ITEMS(<zarray>)` then the dynamic
array will grow to `<newSize>`.  Newly added elements will be uninitialized.

If `<newSize>` is smaller than `ZARRAY_NUM_ITEMS(<zarray>)` then the dynamic
array will shrink to `<newSize>`.  Elements are removed from the end, and their
values lost.

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to resize.
  - `<newSize>` is an unsigned integer specifying the desired number of
  elements in the dynamic array.

*Return Value*

  - *none*

*Usage Notes*

  - This macro is slightly less efficient than calling `ZARRAY_GROW` or
  `ZARRAY_SHRINK`.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 0);
        ZARRAY_RESIZE(myArray, 100);
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Asserts that `<newSize>` is non-negative.
  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.




<hr></hr>
<h3>ZARRAY_SHRINK macro.</h3>

Decrease the size of a dynamic array.

    ZARRAY_SHRINK(<zarray>, <numItemsToRemove>)

Elements are removed from the end, and their values lost.

*Parameters*

  - `<zarray>` is a pointer to the dynamic array object to shrink.
  - `<numItemsToRemove>` is an unsigned integer specifying the number of elements
  to remove.

*Return Value*

  - *none*

*Usage Notes*

  - The dynamic array must have at least `<numItemsToRemove>` elements, or your
  program will do Very Bad things.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 5);
        ZARRAY_GROW(myArray, 3);
        assert(ZARRAY_NUM_ITEMS(myArray) == 8);
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:

  - Asserts that the dynamic array contains at least `<numItemsToRemove>` elements.
  - Asserts that `<numItemsToRemove>` is non-negative.
  - Does a special check asserting that `<zarray>` is, in fact, a dynamic array
  object created with `ZARRAY_NEW`.



<hr></hr>
<h3>ZARRAY_SHRINK_BY_ONE macro.</h3>

Decrease the size of a dynamic array by one (1).

    ZARRAY_SHRINK_BY_ONE(<zarray>)

One (1) element is removed from the end, and its value lost.

*Parameters*

  - `<zarray>` is a pointer to a non-empty dynamic array object to shrink.

*Return Value*

  - *none*

*Usage Notes*

  - The dynamic array must have at least 1 element, or your program will do
  Very Bad things.
  - This macro is slightly more efficient than calling 
  `ZARRAY_SHRINK(zarray, 1)`.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 1);
        ZARRAY_SHRINK_BY_ONE(myArray);
        assert(ZARRAY_NUM_ITEMS(myArray) == 0);
    }

*Debug Checks*

If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then this macro:
    
    - Does a special check asserting that `<zarray>` is, in fact, a dynamic
    array object created with `ZARRAY_NEW`.
    - Asserts that the dynamic array contains at least one element.





<hr></hr>
<h3>ZARRAY_TAIL macro.</h3>

Access the last element of a dynamic array.

    ZARRAY_TAIL(<zarray>)

*Parameters*

  - `<zarray>` is a pointer to a non-empty dynamic array object whose last
  element will be indexed.  

*Return Value*

  - Evaluates to a modifiable l-value that is the last element of the dynamic
  array.

*Usage Notes*

  - The array must contain at least 1 element or your program will corrupt
  memory and may crash.
  - This macro expands to a modifiable l-value that can be used in assignments.
  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument
  that is computationally expensive or has side effects.

*Evaluates To (Approximately):*

    (zarray)[(zarray)->numItems - 1]

*Example*

    {
        ZARRAY(float) myArray = ZARRAY_NEW(float, 1);
        ZARRAY_TAIL(myArray) = 52.1;
        float value = ZARRAY_TAIL(myArray);
    }

*Debug Checks*

  - If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then does a
  special check asserting that `<zarray>` is, in fact, a dynamic array object
  created with `ZARRAY_NEW`.


