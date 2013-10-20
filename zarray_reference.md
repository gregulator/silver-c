ZARRAY Reference
---------------------------

<hr></hr>
**`ZARRAY_NEW` macro.**
-----------------------

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
**`ZARRAY_FREE` macro.**
-----------------------

Free a dynamic array.

    void ZARRAY_FREE(<zarray>)
    
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
**`ZARRAY_NUM_ITEMS` macro.**
-----------------------

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
**`ZARRAY_TAIL` macro.**
-----------------------

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
