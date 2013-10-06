#ifndef ZARRAY_INCLUDED
#define ZARRAY_INCLUDED

#include <stdlib.h>

#ifndef Z4C_NO_DEBUG
#include <assert.h>
#include <stdio.h>
#define _ZARRAY_DEBUG 1
#define _ZARRAY_DEBUG_ASSERT(cond) assert(cond)
#define _ZARRAY_DEBUG_VERIFY(cond, msg)  \
    do {  \
        if (!(cond)) {  \
            fprintf(stderr, msg "\n"); \
            fflush(0); \
            assert(!msg); \
        } \
    } while (0)

#define _ZARRAY_DEBUG_MAGIC_MATCHES(zarray) \
    (_ZARRAY_HEADER(zarray)->_magicid[0] == 'z' \
        && _ZARRAY_HEADER(zarray)->_magicid[1] == 'a' \
        && _ZARRAY_HEADER(zarray)->_magicid[2] == 'r' \
        && _ZARRAY_HEADER(zarray)->_magicid[3] == 'y')

#define _ZARRAY_DEBUG_CLEAR_MAGIC(zarray) \
    (_ZARRAY_HEADER(zarray)->_magicid[0] = 0, \
     _ZARRAY_HEADER(zarray)->_magicid[1] = 0, \
     _ZARRAY_HEADER(zarray)->_magicid[2] = 0, \
     _ZARRAY_HEADER(zarray)->_magicid[3] = 0)

#define _ZARRAY_DEBUG_IS_ZARRAY(zarray)  \
    ((zarray) && _ZARRAY_DEBUG_MAGIC_MATCHES(zarray))

#define _ZARRAY_DEBUG_VERIFY_IS_ZARRAY(macroname, zarray)  \
    do { \
        _ZARRAY_DEBUG_VERIFY(zarray, \
                "ERROR in " macroname ": <zarray> must be non-NULL."); \
        _ZARRAY_DEBUG_VERIFY(_ZARRAY_DEBUG_MAGIC_MATCHES(zarray), \
                "ERROR in " macroname ": <zarray> does not appear to point to a ZArray object. " \
                "Please double-check that you allocated it with ZARRAY_ALLOC"); \
    } while (0)
#else
#define _ZARRAY_DEBUG 0
#define _ZARRAY_DEBUG_ASSERT(cond) (void)0
#define _ZARRAY_DEBUG_VERIFY(cond, msg) (void) 0
#define _ZARRAY_DEBUG_IS_ZARRAY(zarray)  (void) 0
#define _ZARRAY_DEBUG_VERIFY_IS_ZARRAY(macroname, zarray)  (void) 0
#define _ZARRAY_DEBUG_CLEAR_MAGIC(zarray) (void) 0
#endif

typedef struct 
{
#if _ZARRAY_DEBUG
    char _magicid[4];
#endif
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

#define _ZARRAY_HEADER(zarray) \
    ((_ZArrayHeader *)&(((char *)zarray)[-_ZARRAY_HEADER_SIZE]))

#define _ZARRAY_REALLOC(zarray) \
    (zarray) = realloc(_ZARRAY_HEADER(zarray), \
         _ZARRAY_HEADER(zarray)->elemSize*_ZARRAY_HEADER(zarray)->actualNumItems \
            + sizeof(_ZArrayHeader)); \
    (zarray) = _ZARRAY_ARRAY(zarray); \

static ZArray ZARRAY_ALLOC(unsigned elemSize, unsigned startNumItems)
{
    _ZArrayHeader *full;
    unsigned actualNumItems;

    actualNumItems = _ZArray_NextPowerOfTwo(startNumItems+1);
    full = malloc(elemSize*actualNumItems + _ZARRAY_HEADER_SIZE);
    full->numItems = startNumItems;
    full->actualNumItems = actualNumItems;
    full->elemSize = elemSize;
    full->elemSize = elemSize;
#if _ZARRAY_DEBUG
    full->_magicid[0] = 'z';
    full->_magicid[1] = 'a';
    full->_magicid[2] = 'r';
    full->_magicid[3] = 'y';
#endif
    return _ZARRAY_ARRAY(full);
}

#define ZARRAY_FREE(zarray) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_FREE", zarray);  \
        _ZARRAY_DEBUG_CLEAR_MAGIC(zarray); \
        free(_ZARRAY_HEADER(zarray)); \
    } while (0);

#if _ZARRAY_DEBUG
static unsigned ZARRAY_NUM_ITEMS(ZArray zarray)
{
    _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_NUM_ITEMS", zarray);
    return _ZARRAY_HEADER(zarray)->numItems;
}
#else
#define ZARRAY_NUM_ITEMS(zarray) \
        (_ZARRAY_HEADER(zarray)->numItems)
#endif

#define ZARRAY_TAIL(zarray) \
    ((zarray)[ZARRAY_NUM_ITEMS(zarray) - 1])

#define ZARRAY_GROW_BY_ONE(zarray) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_GROW_BY_ONE", zarray);  \
        _ZARRAY_HEADER(zarray)->numItems++; \
        if (_ZARRAY_HEADER(zarray)->numItems > _ZARRAY_HEADER(zarray)->actualNumItems) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems *= 2; \
            _ZARRAY_REALLOC(zarray); \
        } \
    } while (0)

#define ZARRAY_SHRINK_BY_ONE(zarray) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_SHRINK_BY_ONE", zarray);  \
        _ZARRAY_DEBUG_VERIFY(_ZARRAY_HEADER(zarray)->numItems, \
                "ERROR in ZARRAY_SHRINK_BY_ONE: Cannot shrink <zarray> below 0 elements."); \
        _ZARRAY_HEADER(zarray)->numItems--; \
        if (_ZARRAY_HEADER(zarray)->numItems < _ZARRAY_HEADER(zarray)->actualNumItems/4) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems /= 2; \
            _ZARRAY_REALLOC(zarray); \
        } \
    } while (0)
 
#define ZARRAY_GROW(zarray, numItemsToAdd) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_GROW", zarray);  \
        _ZARRAY_DEBUG_VERIFY(((numItemsToAdd) >= 0), \
                "ERROR in ZARRAY_GROW: <numItemsToAdd> must be positive."); \
        _ZARRAY_HEADER(zarray)->numItems += (numItemsToAdd); \
        if (_ZARRAY_HEADER(zarray)->numItems > _ZARRAY_HEADER(zarray)->actualNumItems) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems = _ZArray_NextPowerOfTwo(_ZARRAY_HEADER(zarray)->numItems + 1); \
            _ZARRAY_REALLOC(zarray); \
        } \
    } while (0)

#define ZARRAY_SHRINK(zarray, numItemsToRemove) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_SHRINK", zarray);  \
        _ZARRAY_DEBUG_VERIFY(_ZARRAY_HEADER(zarray)->numItems > numItemsToRemove, \
                "ERROR in ZARRAY_SHRINK: Cannot shrink <zarray> below 0 elements."); \
        _ZARRAY_HEADER(zarray)->numItems -= (numItemsToRemove); \
        if (_ZARRAY_HEADER(zarray)->numItems < _ZARRAY_HEADER(zarray)->actualNumItems / 4) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems = _ZArray_NextPowerOfTwo(_ZARRAY_HEADER(zarray)->numItems + 1) * 2; \
            _ZARRAY_REALLOC(zarray); \
        } \
    } while (0)

#define ZARRAY_RESIZE(zarray, newSize) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_RESIZE", zarray);  \
        _ZARRAY_DEBUG_VERIFY(((newSize) >= 0), \
                "ERROR in ZARRAY_RESIZE: <numItems> must be positive."); \
        _ZARRAY_HEADER(zarray)->numItems = (newSize); \
        if (_ZARRAY_HEADER(zarray)->numItems > _ZARRAY_HEADER(zarray)->actualNumItems) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems = _ZArray_NextPowerOfTwo(_ZARRAY_HEADER(zarray)->numItems + 1); \
            _ZARRAY_REALLOC(zarray); \
        } \
        else if (_ZARRAY_HEADER(zarray)->numItems < _ZARRAY_HEADER(zarray)->actualNumItems / 4) \
        { \
            _ZARRAY_HEADER(zarray)->actualNumItems = _ZArray_NextPowerOfTwo(_ZARRAY_HEADER(zarray)->numItems + 1) * 2; \
            _ZARRAY_REALLOC(zarray); \
        } \
    } while (0)

#define ZARRAY_APPEND(zarray, value) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_APPEND", zarray);  \
        ZARRAY_GROW_BY_ONE(zarray); \
        ZARRAY_TAIL(zarray) = (value); \
    } while (0)

#define ZARRAY_POP(zarray, dest) \
    do { \
        _ZARRAY_DEBUG_VERIFY_IS_ZARRAY("ZARRAY_POP", zarray);  \
        _ZARRAY_DEBUG_VERIFY(_ZARRAY_HEADER(zarray)->numItems > 0, \
                "ERROR in ZARRAY_POP: <zarray> is empty"); \
        (dest) = ZARRAY_TAIL(zarray); \
        ZARRAY_SHRINK_BY_ONE(zarray); \
    } while (0)

#endif
