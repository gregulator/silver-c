#include "zjson.h"

#include "zhash.h"
#include "zarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GENERATE_ZHASH_FIXED_KEY_IMPLEMENTATION(_ZObjHash, char *, bool);

#define REF(hObj) ((hObj)->refcnt++, (hObj))

typedef struct ZJsonValue
{
    int refcnt;
    ZJsonValueTypeEnum type;
    union
    {
        char *sz;
        double dbl;
        ZJsonObjectHandle hObj;
        ZJsonArrayHandle hArray;
        bool bVal;
    };
} ZJsonValue;

typedef struct ZJsonObject
{
    int refcnt;
    _ZObjHash hash;
} ZJsonObject;

typedef struct
{
    int refcnt;
    ZARRAY(ZJsonValueHandle) items;
} ZJsonArray;

ZJsonValueHandle ZJsonValue_FromString(char * sz)
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_STRING;
    hNew->sz = strdup(sz);
    hNew->refcnt = 0;
    return hNew;
}

ZJsonValueHandle ZJsonValue_FromNumber(double val)
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_NUMBER;
    hNew->dbl = val;
    hNew->refcnt = 0;
    return hNew;
}

ZJsonValueHandle ZJsonValue_FromObject(ZJsonObjectHandle hObj)
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_OBJECT;
    hNew->hObj = REF(hObj);
    hNew->refcnt = 0;
    return hNew;
}

ZJsonValueHandle ZJsonValue_FromArray(ZJsonArrayHandle hArray)
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_NUMBER;
    hNew->hArray = REF(hArray);
    hNew->refcnt = 0;
    return hNew;
}

ZJsonValueHandle ZJsonValue_FromBoolean(bool val)
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_NUMBER;
    hNew->bVal = val;
    hNew->refcnt = 0;
    return hNew;
}

ZJsonValueHandle ZJsonValue_Null()
{
    ZJsonValueHandle hNew;
    hNew = malloc(sizeof(ZJsonValue));
    hNew->type = ZJSON_VALUE_TYPE_NULL;
    hNew->refcnt = 0;
    return hNew;
}

void _ZJsonValue_Free(ZJsonValueHandle hVal)
{
    free(hVal->sz);
    free(hVal);
}

char * ZJsonValue_GetString(ZJsonValueHandle hVal)
{
    assert(hVal->type == ZJSON_VALUE_TYPE_STRING);
    return strdup(hVal->sz);
}
double ZJsonValue_GetNumber(ZJsonValueHandle hVal)
{
    assert(hVal->type == ZJSON_VALUE_TYPE_NUMBER);
    return hVal->dbl;
}
ZJsonObjectHandle ZJsonValue_GetObject(ZJsonValueHandle hVal)
{
    assert(hVal->type == ZJSON_VALUE_TYPE_OBJECT);
    return REF(hVal->hObj);
}
ZJsonArrayHandle ZJsonValue_GetArray(ZJsonValueHandle hVal)
{
    assert(hVal->type == ZJSON_VALUE_TYPE_ARRAY);
    return REF(hVal->hArray);
}

bool ZJsonValue_IsString(ZJsonValueHandle hVal)
{
    return hVal->type == ZJSON_VALUE_TYPE_STRING;
}
bool ZJsonValue_IsNumber(ZJsonValueHandle hVal)
{
    return hVal->type == ZJSON_VALUE_TYPE_NUMBER;
}
bool ZJsonValue_IsObject(ZJsonValueHandle hVal)
{
    return hVal->type == ZJSON_VALUE_TYPE_OBJECT;
}
bool ZJsonValue_IsArray(ZJsonValueHandle hVal)
{
    return hVal->type == ZJSON_VALUE_TYPE_ARRAY;
}
bool ZJsonValue_IsNull(ZJsonValueHandle hVal)
{
    return hVal->type == ZJSON_VALUE_TYPE_NULL;
}

ZJsonObjectHandle ZJsonObject_New()
{
    ZJsonObjectHandle hNew;
    hNew = malloc(sizeof(ZJsonObject));
    hNew->hash = _ZObjHash_new();
    Ref_Init(hNew, _ZJsonValue_Free);
    return hNew;
}

void ZJsonObject_Set(ZJsonObjectHandle hObj, char * szKey, ZJsonValueHandle hVal)
{
    hVal->refcnt++;
    _ZObjHash_insert(hObj->hash, szKey, hVal);
}

void ZJsonObject_SetString(ZJsonObjectHandle hObj, char * szKey, char *szVal)
{
    ZJsonValueHandle newVal = ZJsonValue_FromString(szVal);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void ZJsonObject_SetNumber(ZJsonObjectHandle hObj, char * szKey, double val)
{
    ZJsonValueHandle newVal = ZJsonValue_FromNumber(val);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void ZJsonObject_SetObject(ZJsonObjectHandle hObj, char * szKey, ZJsonObjectHandle hObjVal)
{
    ZJsonValueHandle newVal = ZJsonValue_FromObject(hObjVal);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void ZJsonObject_SetArray(ZJsonObjectHandle hObj, char * szKey, ZJsonArrayHandle hArray)
{
    ZJsonValueHandle newVal = ZJsonValue_FromArray(hArray);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void ZJsonObject_SetBoolean(ZJsonObjectHandle hObj, char * szKey, bool val)
{
    ZJsonValueHandle newVal = ZJsonValue_FromBoolean(hObj);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

ZJsonValueHandle ZJsonObject_Get(ZJsonObjectHandle hObj, char * szKey)
{
    ZJsonValueHandle hVal;
    bool contains;
    contains = _ZObjHash_get(hObj->hash, szKey, &hVal);
    return contains ? hVal : NULL;
}

ZJsonValueTypeEnum ZJsonObject_GetType(ZJsonObjectHandle hObj, char * szKey)
{
    ZJsonValueHandle hVal;
    bool contains;
    contains = _ZObjHash_get(hObj, szKey, &hVal);
    return contains ? hVal->type : ZJSON_VALUE_TYPE_INVALID;
}

char * ZJsonObject_GetString(ZJsonObjectHandle hObj, char * szKey)
{
}
double ZJsonObject_GetNumber(ZJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
ZJsonObjectHandle ZJsonObject_GetObject(ZJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
ZJsonArrayHandle ZJsonObject_GetArray(ZJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
bool ZJsonObject_GetBoolean(ZJsonObjectHandle hObj, char * szKey);

bool ZJsonObject_IsValueString(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_STRING;
}
bool ZJsonObject_IsValueNumber(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_NUMBER;
}
bool ZJsonObject_IsValueObject(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_OBJECT;
}
bool ZJsonObject_IsValueArray(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_ARRAY;
}
bool ZJsonObject_IsValueBoolean(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_BOOLEAN;
}
bool ZJsonObject_IsValueNull(ZJsonObjectHandle hObj, char * szKey)
{
    return ZJsonObject_GetType(hObj, szKey) == ZJSON_VALUE_TYPE_NULL;
}

void ZJsonObject_Unset(ZJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
bool ZJsonObject_HasKey(ZJsonObjectHandle hObj, char * szKey)
{
    return _ZObjHash_has(hObj, szKey);
}

ZJsonArrayHandle ZJsonArray_New()
{
    ZJsonArrayHandle hNew;
    hNew = malloc(sizeof(ZJsonArrayHandle));
    hNew->items = ZARRAY_NEW(ZJsonValue, 0);
    hNew->sz = strdup(sz);
    Ref_Init(hNew, _ZJsonValue_Free);
    return hNew;
}

ZJsonArrayHandle ZJsonArray_NumItems(ZJsonArrayHandle hArray)
{
    return ZARRAY_NUM_ITEMS(hArray->items);
}

void ZJsonArray_Append(ZJsonArrayHandle hArray, ZJsonValueHandle hVal)
{
    ZARRAY_APPEND(hArray->items, hVal)
}
void ZJsonArray_AppendString(ZJsonArrayHandle hArray, char * szVal)
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_FromString(szVal);
    ZARRAY_APPEND(hArray->items, hVal);
}
void ZJsonArray_AppendNumber(ZJsonArrayHandle hArray, double val);
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_FromNumber(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void ZJsonArray_AppendObject(ZJsonArrayHandle hArray, ZJsonObjectHandle hObj);
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_FromObject(hObj);
    ZARRAY_APPEND(hArray->items, hVal);
}
void ZJsonArray_AppendArray(ZJsonArrayHandle hArray, ZJsonArrayHandle hArray);
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_FromArray(hArray);
    ZARRAY_APPEND(hArray->items, hVal);
}
void ZJsonArray_AppendBoolean(ZJsonArrayHandle hArray, bool val);
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_FromBoolean(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void ZJsonArray_AppendNull(ZJsonArrayHandle hArray);
{
    ZJsonValueHandle hVal;
    hVal = ZJsonValue_Null();
    ZARRAY_APPEND(hArray->items, hVal);
}
bool ZJsonArray_IsEntryString(ZJsonArrayHandle hArray, unsigned idx)
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_STRING);
}
bool ZJsonArray_IsEntryNumber(ZJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_NUMBER);
}
bool ZJsonArray_IsEntryObject(ZJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_OBJECT);
}
bool ZJsonArray_IsEntryArray(ZJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_ARRAY);
}
bool ZJsonArray_IsEntryBoolean(ZJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_BOOLEAN);
}
bool ZJsonArray_IsEntryNull(ZJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == ZJSON_VALUE_TYPE_NULL);
}


_Value_ToJson(ZStringChain chain, ZJsonValueHandle hVal)
{
    switch (hVal->type)
    {
        case ZJSON_VALUE_TYPE_STRING:
        {
            // TODO: escape
            ZStringChain_AppendPrintf(chain, "\"%s\"", hVal->sz)
            break;
        }
        case ZJSON_VALUE_TYPE_NUMBER:
        {
            // TODO: formatting?
            ZStringChain_AppendPrintf(chain, "%f", hVal->number)
            break;
        }
        case ZJSON_VALUE_TYPE_OBJECT:
        {
            // TODO
            break;
        }
        case ZJSON_VALUE_TYPE_ARRAY:
        {
            ZStringChain_AppendChars(chain, "[");
            ZARRAY(ZJsonValueHandle) items = hVal->hArray->items;
            int numItems = ZARRAY_NUM_ITEMS(items);
            for (i = 0; i < numItems; i++)
            {
                ZJsonValueHandle hItemVal = items[i];
                _Value_ToJson(chain, ZJsonValueHandle hVal);
                if (i < numItems - 1)
                    ZStringChain_Append(",");
            }
            ZStringChain_AppendChars(chain, "]");
            break;
        }
        case ZJSON_VALUE_TYPE_NULL:
        {
            ZStringChain_AppendChars(chain, "null")
            break;
        }
    }
}

char * ZJsonValue_ToJsonString(ZJsonValueHandle hVal)
{
    ZStringListHandle chain = ZStringList_New();
    char * out;
    _Value_ToJson(chain, hVal);
    out = ZStringList_ToNewChars();
    ZStrinChain_Unref(chain);
    return out;
}
