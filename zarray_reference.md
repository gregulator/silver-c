ZARRAY Reference
---------------------------

**Debugging**

<hr></hr>
**`ZARRAY_NEW` macro.**
-----------------------

Create a new dynamic array.

    void * ZARRAY_NEW(<datatype>, <startNumItems>)

*Parameters:*

  - `<datatype>` is the desired datatype of the array elements.  For example `unsigned int` or `struct Sphere`. 
  - `<startNumItems>` is an unsigned integer specifying the initial array size.  Use `0` to create an empty array.

*Returns:*

  - Pointer to newly allocated dynamic array object, or `NULL` if memory allocation failed.
  - Should be assigned to a `ZARRAY(<datatype>)` variable.

*Usage Notes*

  - Array elements are uninitialized and contain undefined data.

*Example*

    ZARRAY(long double) myArray = ZARRAY_NEW(long double, 10);
    
*C++ specific:*

Unlike C, C++ requires you to explicitely cast `void *` to the appropriate pointer type.  Furthermore, C++ does not allow anonymous structures in casts.  To get around these issues, the `C++` implementation uses templates.  If `__cplusplus` is
defined the macro instead uses a template return type:

    _ZArrayStruct<datatype> ZARRAY_NEW(<datatype>, <startNumItems>)

<hr></hr>
**`ZARRAY_FREE` macro.**
-----------------------

Free a dynamic array.

    void ZARRAY_FREE(<zarray>)
    
*Parameters*

  - `<zarray>` is a pointer to the dynamic array object you wish to free.

*Return Value*

  - *none*

*Usage Notes*

  - This macro evaluates `<zarray>` multiple times.  Avoid passing an argument that is computationally expensive or has side effects.

*Example*

    ZARRAY(char) myArray = ZARRAY_NEW(char, 1024);
    ZARRAY_FREE(myArray);
 
*Debug Checks*

  - If debugging is enable (ie:`Z4C_NO_DEBUG` is undefined), then does a special check asserting that `<zarray>` is, in fact, a dynamic array object created with `ZARRAY_NEW`.
