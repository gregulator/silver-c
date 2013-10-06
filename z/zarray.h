#ifndef ZARRAY_INCLUDED
#define ZARRAY_INCLUDED

#include <stdlib.h>

typedef struct 
{
    unsigned numItems;
    unsigned actualNumItems;
    unsigned elemSize;
} _ZArrayHeader;

#define ZArray void *
static unsigned _ZArray_NextPowerOfTwo(unsigned x)
{
    /* From http://acius2.blogspot.com/2007/11/calculating-next-power-of-2.html */
    x--;
    x = (x >> 1) | x;
    x = (x >> 2) | x;
    x = (x >> 4) | x;
    x = (x >> 8) | x;
    x = (x >> 16) | x;
    return x + 1;
}

#define _ZARRAY_HEADER_SIZE (sizeof(_ZArrayHeader))

#define _ZARRAY_ARRAY(pHeader) \
    ((void *)&(((char *)pHeader)[+_ZARRAY_HEADER_SIZE]))

#define _ZARRAY_HEADER(array) \
    ((_ZArrayHeader *)&(((char *)array)[-_ZARRAY_HEADER_SIZE]))

#define _ZARRAY_REALLOC(array) \
    (array) = realloc(_ZARRAY_HEADER(array), \
         _ZARRAY_HEADER(array)->elemSize*_ZARRAY_HEADER(array)->actualNumItems \
            + sizeof(_ZArrayHeader)); \
    (array) = _ZARRAY_ARRAY(array); \

static ZArray ZARRAY_ALLOC(unsigned elemSize, unsigned startNumItems)
{
    _ZArrayHeader *full;
    unsigned actualNumItems;

    actualNumItems = _ZArray_NextPowerOfTwo(startNumItems);
    full = malloc(elemSize*actualNumItems + _ZARRAY_HEADER_SIZE);
    full->numItems = startNumItems;
    full->actualNumItems = actualNumItems;
    full->elemSize = elemSize;
    return _ZARRAY_ARRAY(full);
}

#define ZARRAY_FREE(array) \
    free(_ZARRAY_HEADER(array))

#define ZARRAY_NUM_ITEMS(array) \
    (_ZARRAY_HEADER(array)->numItems)

#define ZARRAY_TAIL(array) \
    ((array)[ZARRAY_NUM_ITEMS(array) - 1])

#define ZARRAY_GROW_BY_ONE(array) \
    do { \
        _ZARRAY_HEADER(array)->numItems++; \
        if (_ZARRAY_HEADER(array)->numItems > _ZARRAY_HEADER(array)->actualNumItems) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems *= 2; \
            _ZARRAY_REALLOC(array); \
        } \
    } while (0)

#define ZARRAY_SHRINK_BY_ONE(array) \
    do { \
        _ZARRAY_HEADER(array)->numItems--; \
        if (_ZARRAY_HEADER(array)->numItems < _ZARRAY_HEADER(array)->actualNumItems/4) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems /= 2; \
            _ZARRAY_REALLOC(array); \
        } \
    } while (0)
 
#define ZARRAY_GROW(array, numItemsToAdd) \
    do { \
        _ZARRAY_HEADER(array)->numItems += (numItemsToAdd); \
        if (_ZARRAY_HEADER(array)->numItems > _ZARRAY_HEADER(array)->actualNumItems) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems = _NextPowerOfTwo(_ZARRAY_HEADER(array)->numItems); \
            _ZARRAY_REALLOC(array); \
        } \
    } while (0)

#define ZARRAY_SHRINK(array, numItemsToRemove) \
    do { \
        _ZARRAY_HEADER(array)->numItems -= (numItemsToRemove); \
        if (_ZARRAY_HEADER(array)->numItems < _ZARRAY_HEADER(array)->actualNumItems / 4) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems = _NextPowerOfTwo(_ZARRAY_HEADER(array)->numItems) * 2; \
            _ZARRAY_REALLOC(array); \
        } \
    } while (0)

#define ZARRAY_RESIZE(array, numItems) \
    do { \
        _ZARRAY_HEADER(array)->numItems += (numItemsToAdd); \
        if (_ZARRAY_HEADER(array)->numItems > _ZARRAY_HEADER(array)->actualNumItems) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems = _NextPowerOfTwo(_ZARRAY_HEADER(array)->numItems); \
            _ZARRAY_REALLOC(array); \
        } \
        else if (_ZARRAY_HEADER(array)->numItems < _ZARRAY_HEADER(array)->actualNumItems / 4) \
        { \
            _ZARRAY_HEADER(array)->actualNumItems = _NextPowerOfTwo(_ZARRAY_HEADER(array)->numItems) * 2; \
            _ZARRAY_REALLOC(array); \
        } \
    } while (0)

#define ZARRAY_APPEND(array, value) \
    do { \
        ZARRAY_GROW_BY_ONE(array); \
        ZARRAY_TAIL(array) = (value); \
    } while (0)

#define ZARRAY_POP(array, dest) \
    do { \
        (dest) = ZARRAY_TAIL(array); \
        ZARRAY_SHRINK_BY_ONE(array); \
    } while (0)

#endif
