/*
 * string.c -- Copyright 2009-2013 Greg Prisament
 *
 * Implements string interface.
 */

#include "zstring.h"
#include "zarray.h"
//#include "dsa/dynarray.h"
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* TODO: is strlen correct with unicode characters? */
#define _MIN(x, y) ((x) < (y) ? (x) : (y))
#define _MAX(x, y) ((x) > (y) ? (x) : (y))

typedef struct ZString
{
    // data consists of length chars followed by null-ter_MINator.
    char *data;
    unsigned length; /* in bytes? characters? */
} ZString; 

typedef struct ZStringList
{
    ZARRAY(ZStringHandle) array;
} ZStringList;

ZStringHandle ZString_New(const char *src)
{
    ZStringHandle hNew = malloc(sizeof(ZString));
    hNew->length = strlen(src);
    hNew->data = malloc(hNew->length + 1);
    strcpy(hNew->data, src);
    hNew->data[hNew->length] = 0;
    return hNew;
}

ZStringHandle ZString_NewLength(const char *src, unsigned length)
{
    ZStringHandle hNew = malloc(sizeof(ZString));
    hNew->length = _MIN(length, strlen(src));
    hNew->data = malloc(hNew->length + 1);
    strncpy(hNew->data, src, length);
    hNew->data[hNew->length] = 0;
    return hNew;
}

ZStringHandle ZString_NewPrintf(const char *fmt, unsigned size, ...)
{
    char * tmpResult;
    va_list args;
    ZStringHandle hNew;

    tmpResult = malloc(size+1);

    va_start(args, size);
    vsnprintf(tmpResult, (size_t)size, fmt, args);
    va_end(args);

    hNew = ZString_New(tmpResult);

    free(tmpResult);

    return hNew;
}

void ZString_Free(ZStringHandle *phZString)
{
    free((*phZString)->data);
    free(*phZString);
    *phZString = NULL;
}

void ZString_Set(ZStringHandle hOut, const char *in)
{
    free(hOut->data);
    hOut->length = strlen(in);
    hOut->data = malloc(hOut->length + 1);
    strcpy(hOut->data, in);
    hOut->data[hOut->length] = 0;
}

void ZString_Clear(ZStringHandle hOut)
{
    ZString_Set(hOut, 0);
}

unsigned ZString_Length(const ZStringHandle hZString)
{
    return hZString->length;
}

/* TODO: unicode support */
unsigned ZString_Bytes(const ZStringHandle hZString)
{
    return hZString->length + 1;
}

const char * ZString_GetChars(const ZStringHandle hZString)
{
    return hZString->data;
}

void ZString_Copy(ZStringHandle hResult, const ZStringHandle hSrc)
{
    if (hSrc == hResult)
        return;

    free(hResult->data);
    hResult->length = hSrc->length;
    hResult->data = malloc(hSrc->length+1);
    strncpy(hResult->data, hSrc->data, hResult->length);
    hResult->data[hResult->length] = 0;
}

void ZString_CaseConvert(ZStringHandle hZString, ZStringCase newCase)
{
    unsigned i;
    switch (newCase)
    {
        case STRING_CASE_UPPER:
        {
            for (i = 0; i < hZString->length; i++)
                hZString->data[i] = toupper(hZString->data[i]);
            break;
        }
        case STRING_CASE_LOWER:
        {
            for (i = 0; i < hZString->length; i++)
                hZString->data[i] = tolower(hZString->data[i]);
            break;
        }
        default:
        {
            assert(0);
        }
    }
}

int ZString_Compare(
        const ZStringHandle hZStringA, 
        const ZStringHandle hZStringB,
        unsigned flags )
{
    // not supported yet
    assert(!(flags & STRING_COMPARE_IGNORE_WHITESPACE_FLAG));
    
    if (flags & STRING_COMPARE_IGNORE_CASE_FLAG)
    {
        return strcasecmp(hZStringA->data, hZStringB->data);
    }
    return strcmp(hZStringA->data, hZStringB->data);
}

int ZString_CompareChars(
        const char * strA, 
        const char * strB,
        unsigned flags )
{
    // not supported yet
    assert(!(flags & STRING_COMPARE_IGNORE_WHITESPACE_FLAG));
    
    if (flags & STRING_COMPARE_IGNORE_CASE_FLAG)
    {
        return strcasecmp(strA, strB);
    }
    return strcmp(strA, strB);
}

bool ZString_ContainsChars(ConstZStringHandle hHaystack, const char *needle)
{
    if (strstr(hHaystack->data, needle))
    {
        return true;
    }
    return false;
}

bool ZString_StartsWith(ConstZStringHandle hZString, const char *needle)
{
    if (!memcmp(hZString->data, needle, strlen(needle)))
    {
        return true;
    }
    return false;
}

int ZString_Search(ConstZStringHandle hHaystack, const char c)
{
    char *p;
    char needle[2] = {c, 0};
    p = strstr(hHaystack->data, needle);
    if (!p)
    {
        return -1;
    }
    return p - hHaystack->data;
}

float ZString_RemoveFloat(ZStringHandle hZString)
{
    float out;
    char *endptr;
    
    out = strtof(hZString->data, &endptr);
    ZString_SubString(hZString, hZString, endptr - hZString->data, -1);

    return out;
}

char ZString_FirstNonWhitespaceChar(ConstZStringHandle hZString)
{
    char *s = (char *)hZString->data;

    while (*s && isspace(*s))
        s++;

    return *s;

}

void ZString_Trim(ZStringHandle hZString)
{
    char *s = (char *)hZString->data;
    char *e = (char *)&hZString->data[hZString->length - 1];

    while (*s && isspace(*s))
        s++;

    while (*e && isspace(*e))
        e--;

    ZString_SubString(hZString, hZString, s - hZString->data, e - hZString->data);
}

void ZString_SubString(
        ZStringHandle hResult, 
        const ZStringHandle hSrc, 
        int start, 
        int end)
{
    char *newData;

    if (start < 0)
        start = (int)hSrc->length + start;

    if (end < 0)
        end = (int)hSrc->length + end;

    start = _MIN(_MAX(start, 0), hSrc->length - 1);
    end = _MIN(_MAX(end, 0), hSrc->length - 1);

    if (end < start)
    {
        newData = malloc(1);
        newData[0] = 0;
        free(hResult->data);
        hResult->data = newData;
        hResult->length = 0;
    }
    else
    {
        newData = malloc(end - start + 2);
        memcpy(newData, &hSrc->data[start], end - start + 1);
        newData[end - start + 1] = 0;
        free(hResult->data);
        hResult->data = newData;
        hResult->length = end - start + 1;
    }
    return;
}

unsigned ZString_Tounsigned(ConstZStringHandle hZString)
{
    return (unsigned)atoi(hZString->data);
}

void ZString_Append(ZStringHandle hOriginal, const ZStringHandle hAppend)
{
    unsigned sumLength;
    sumLength = hOriginal->length + hAppend->length;
    hOriginal->data = realloc(hOriginal->data, sumLength + 1);
    memcpy(&hOriginal->data[hOriginal->length], hAppend->data, hAppend->length);
    hOriginal->data[sumLength] = 0;
    hOriginal->length = sumLength;
}

void ZString_AppendChars(ZStringHandle hOriginal, const char *pAppend)
{
    unsigned sumLength;
    sumLength = hOriginal->length + strlen(pAppend);
    hOriginal->data = realloc(hOriginal->data, sumLength + 1);
    memcpy(&hOriginal->data[hOriginal->length], pAppend, strlen(pAppend));
    hOriginal->data[sumLength] = 0;
    hOriginal->length = sumLength;
}

void ZString_AppendPrintf(ZStringHandle hOriginal, const char *fmt, unsigned size, ...)
{
    char * tmpResult;
    va_list args;

    tmpResult = malloc(size+1);

    va_start(args, size);
    vsnprintf(tmpResult, (size_t)size, fmt, args);
    va_end(args);

    ZString_AppendChars(hOriginal, tmpResult);

    free(tmpResult);
}

void ZString_RemoveToChar(ZStringHandle hZString, char c)
{
    char *s = (char *)hZString->data;

    while (*s && (*s != c))
        s++;

    s++;
    ZString_SubString(hZString, hZString, s - hZString->data, -1);
}

static uint64_t ror_13_bits(uint64_t in)
{
    uint64_t bit = in & 0x1fff;
    uint64_t out;
    in >>= 13;
    out = in | (bit << (64 - 13));
    return out;
    
}

void ZString_Hash(ZStringHandle hResult, ConstZStringHandle hSrc)
{
    uint64_t sum = 0x1234567801234567ULL;
    unsigned i, j;
    char result[13];

    for (j = 0; j < 50; j++)
    {
        for (i = 0; i < hSrc->length; i++)
        {
            sum += (uint64_t)((uint8_t)hSrc->data[i] ^ (uint8_t)i);
            sum = ror_13_bits(sum);
        }
    }

    for (i = 0; i < 13; i++)
    {
        result[i] = (char)(sum & 0x1f);
        if (result[i] < 26)
            result[i] += 'a';
        else 
            result[i] = result[i] + '0' - 26;
        sum >>= 5;
    }

    hResult->data = realloc(hResult->data, 14);
    memcpy(hResult->data, result, 13);
    hResult->data[13] = 0;
    hResult->length = 13;
}

void ZString_Rot13(ZStringHandle hResult, ZStringHandle hOriginal)
{
    unsigned i;
    char *newData;
    newData = malloc(hOriginal->length);
    for (i = 0; i < hOriginal->length; i++)
    {
        if (isupper(hOriginal->data[i]))
        {
            newData[i] = (((hOriginal->data[i] - 'A') + 13) % 26) + 'A';
        }
        if (islower(hOriginal->data[i]))
        {
            newData[i] = (((hOriginal->data[i] - 'a') + 13) % 26) + 'a';
        }
        else
        {
            newData[i] = hOriginal->data[i];
        }
    }
    free(hResult->data);
    hResult->length = hOriginal->length;
    hResult->data = malloc(hOriginal->length+1);
    strncpy(hResult->data, newData, hResult->length);
    hResult->data[hResult->length] = 0;
    free(newData);
}

ZStringListHandle ZString_Split(ZStringHandle hZString, char delimiter)
{
    char *start = hZString->data;
    char *end = start;
    ZStringHandle hNewZString;
    ZStringListHandle hNew;

    hNew = malloc(sizeof(ZStringList));

    hNew->array = ZARRAY_NEW(ZStringHandle, 0);

    while (*end)
    {
        if (*end == delimiter)
        {
            hNewZString = ZString_NewLength(start, end - start);
            ZARRAY_APPEND(hNew->array, hNewZString);
            start = end + 1;
        }
        end++;
    }
    hNewZString = ZString_NewLength(start, end - start);
    ZARRAY_APPEND(hNew->array, hNewZString);

    return hNew;
}

unsigned ZStringList_NumZStrings(ZStringListHandle hList)
{
    return ZARRAY_NUM_ITEMS(hList->array);
}

ZStringHandle ZStringList_GetString(ZStringListHandle hList, unsigned idx)
{
    return ZARRAY_AT(hList->array, idx);
}

void ZStringList_Join(ZStringHandle hString, ZStringListHandle hList, const char *joiner)
{
    /* TODO: optimize */
    int numItems = ZARRAY_NUM_ITEMS(hList->array);
    int i;
    ZString_Clear(hString);
    for (i = 0; i < numItems; i++)
    {
        ZString_Append(hString, ZARRAY_AT(hList->array, i));
        if (joiner && (i < numItems - 1))
            ZString_AppendChars(hString, joiner);
    }
}


#if 0
void ZStringList_Free(ZStringListHandle *phList)
{
    unsigned numZStrings = DynArrayNumItems((*phList)->hArray);
    ZStringHandle hZString;
    unsigned i;

    for (i = 0; i < numZStrings; i++)
    {
        hZString = DynArrayGet((*phList)->hArray, i);
        ZString_Free(&hZString);
    }
    DynArrayFree(&(*phList)->hArray);
    free(*phList);
    *phList = NULL;
}
#endif
