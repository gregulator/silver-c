#include <ag_dynarray.h>
#include <stdlib.h>
#include <assert.h>

typedef struct 
{
    unsigned numItems;
    unsigned actualNumItems;
    unsigned floorHint;
    unsigned elemSize;
} AgDynArrayHeader;

unsigned _NextPowerOfTwo(unsigned x)
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

unsigned _PrevPowerOfTwo(unsigned x)
{
    x--;
    x = (x >> 1) | x;
    x = (x >> 2) | x;
    x = (x >> 4) | x;
    x = (x >> 8) | x;
    x = (x >> 16) | x;
    return (x + 1) / 2;
}


#define HEADER_SIZE (sizeof(AgDynArrayHeader))

#define GET_ARRAY(pArrayHeader) \
    ((AgDynArrayHandle *)&(((char *)pArrayHeader)[+HEADER_SIZE]))

#define GET_HEADER(hDynArray) \
    ((AgDynArrayHeader *)&(((char *)hDynArray)[-HEADER_SIZE]))

AgDynArrayHandle AgDynArray_Create(unsigned elemSize, unsigned startNumItems, unsigned floorHint)
{
    AgDynArrayHeader *full;
    unsigned actualNumItems;

    actualNumItems = _NextPowerOfTwo(startNumItems);
    full = malloc(elemSize*actualNumItems + sizeof(AgDynArrayHeader));
    full->numItems = startNumItems;
    full->actualNumItems = actualNumItems;
    full->elemSize = elemSize;
    return GET_ARRAY(full);
}

void AgDynArray_Free(AgDynArrayHandle hDynArray)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);
    free(pHeader);
}

unsigned AgDynArray_NumItems(AgDynArrayHandle hDynArray)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);
    return pHeader->numItems;
}

AgDynArrayHandle AgDynArray_GrowByOne(AgDynArrayHandle hDynArray)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems++;
    if (pHeader->numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems *= 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(AgDynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

AgDynArrayHandle AgDynArray_ShrinkByOne(AgDynArrayHandle hDynArray)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems--;
    if (pHeader->numItems < pHeader->actualNumItems/4)
    {
        pHeader->actualNumItems /= 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->numItems + sizeof(AgDynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

AgDynArrayHandle AgDynArray_Grow(AgDynArrayHandle hDynArray, unsigned numItemsToAdd)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems += numItemsToAdd;
    if (pHeader->numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems);
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(AgDynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

AgDynArrayHandle AgDynArray_Shrink(AgDynArrayHandle hDynArray, unsigned numItemsToRemove)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);

    assert(pHeader->numItems >= numItemsToRemove);

    pHeader->numItems -= numItemsToRemove;
    if (pHeader->numItems < pHeader->actualNumItems / 4)
    {
        /* TODO: handle floorHint */
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems) * 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(AgDynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

AgDynArrayHandle AgDynArray_Resize(AgDynArrayHandle hDynArray, unsigned numItems)
{
    AgDynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems = numItems;
    if (numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems);
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(AgDynArrayHeader));
    }
    else if (numItems < pHeader->actualNumItems / 4)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems) * 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(AgDynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}
