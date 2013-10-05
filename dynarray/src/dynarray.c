#include <xe_dynarray.h>
#include <stdlib.h>
#include <assert.h>

typedef struct 
{
    unsigned numItems;
    unsigned actualNumItems;
    unsigned floorHint;
    unsigned elemSize;
} DynArrayHeader;

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


#define HEADER_SIZE (sizeof(DynArrayHeader))

#define GET_ARRAY(pArrayHeader) \
    ((DynArrayHandle *)&(((char *)pArrayHeader)[+HEADER_SIZE]))

#define GET_HEADER(hDynArray) \
    ((DynArrayHeader *)&(((char *)hDynArray)[-HEADER_SIZE]))

DynArrayHandle DynArray_Create(unsigned elemSize, unsigned startNumItems, unsigned floorHint)
{
    DynArrayHeader *full;
    unsigned actualNumItems;

    actualNumItems = _NextPowerOfTwo(startNumItems);
    full = malloc(elemSize*actualNumItems + sizeof(DynArrayHeader));
    full->numItems = startNumItems;
    full->actualNumItems = actualNumItems;
    full->elemSize = elemSize;
    return GET_ARRAY(full);
}

void DynArray_Free(DynArrayHandle hDynArray)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);
    free(pHeader);
}

unsigned DynArray_NumItems(DynArrayHandle hDynArray)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);
    return pHeader->numItems;
}

DynArrayHandle DynArray_GrowByOne(DynArrayHandle hDynArray)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems++;
    if (pHeader->numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems *= 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(DynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

DynArrayHandle DynArray_ShrinkByOne(DynArrayHandle hDynArray)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems--;
    if (pHeader->numItems < pHeader->actualNumItems/4)
    {
        pHeader->actualNumItems /= 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->numItems + sizeof(DynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

DynArrayHandle DynArray_Grow(DynArrayHandle hDynArray, unsigned numItemsToAdd)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems += numItemsToAdd;
    if (pHeader->numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems);
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(DynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

DynArrayHandle DynArray_Shrink(DynArrayHandle hDynArray, unsigned numItemsToRemove)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);

    assert(pHeader->numItems >= numItemsToRemove);

    pHeader->numItems -= numItemsToRemove;
    if (pHeader->numItems < pHeader->actualNumItems / 4)
    {
        /* TODO: handle floorHint */
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems) * 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(DynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}

DynArrayHandle DynArray_Resize(DynArrayHandle hDynArray, unsigned numItems)
{
    DynArrayHeader *pHeader = GET_HEADER(hDynArray);

    pHeader->numItems = numItems;
    if (numItems > pHeader->actualNumItems)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems);
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(DynArrayHeader));
    }
    else if (numItems < pHeader->actualNumItems / 4)
    {
        pHeader->actualNumItems = _NextPowerOfTwo(pHeader->numItems) * 2;
        pHeader = realloc(pHeader, pHeader->elemSize*pHeader->actualNumItems + sizeof(DynArrayHeader));
    }

    return GET_ARRAY(pHeader);
}
