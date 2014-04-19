#include "red_json.h"

#include "zhash.h"
#include "zarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GENERATE_ZHASH_FIXED_KEY_IMPLEMENTATION(_ZObjHash, char *, bool);

#define REF(hObj) ((hObj)->refcnt++, (hObj))

typedef struct RedJsonValue
{
    int refcnt;
    RedJsonValueTypeEnum type;
    union
    {
        char *sz;
        double dbl;
        RedJsonObjectHandle hObj;
        RedJsonArrayHandle hArray;
        bool bVal;
    };
} RedJsonValue;

typedef struct RedJsonObject
{
    int refcnt;
    _ZObjHash hash;
} RedJsonObject;

typedef struct
{
    int refcnt;
    ZARRAY(RedJsonValueHandle) items;
} RedJsonArray;

RedJsonValueHandle RedJsonValue_FromString(char * sz)
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_STRING;
    hNew->sz = strdup(sz);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValueHandle RedJsonValue_FromNumber(double val)
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_NUMBER;
    hNew->dbl = val;
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValueHandle RedJsonValue_FromObject(RedJsonObjectHandle hObj)
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_OBJECT;
    hNew->hObj = REF(hObj);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValueHandle RedJsonValue_FromArray(RedJsonArrayHandle hArray)
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_NUMBER;
    hNew->hArray = REF(hArray);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValueHandle RedJsonValue_FromBoolean(bool val)
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_NUMBER;
    hNew->bVal = val;
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValueHandle RedJsonValue_Null()
{
    RedJsonValueHandle hNew;
    hNew = malloc(sizeof(RedJsonValue));
    hNew->type = RED_JSON_VALUE_TYPE_NULL;
    hNew->refcnt = 0;
    return hNew;
}

void _RedJsonValue_Free(RedJsonValueHandle hVal)
{
    free(hVal->sz);
    free(hVal);
}

char * RedJsonValue_GetString(RedJsonValueHandle hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_STRING);
    return strdup(hVal->sz);
}
double RedJsonValue_GetNumber(RedJsonValueHandle hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_NUMBER);
    return hVal->dbl;
}
RedJsonObjectHandle RedJsonValue_GetObject(RedJsonValueHandle hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_OBJECT);
    return REF(hVal->hObj);
}
RedJsonArrayHandle RedJsonValue_GetArray(RedJsonValueHandle hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_ARRAY);
    return REF(hVal->hArray);
}

bool RedJsonValue_IsString(RedJsonValueHandle hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_STRING;
}
bool RedJsonValue_IsNumber(RedJsonValueHandle hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_NUMBER;
}
bool RedJsonValue_IsObject(RedJsonValueHandle hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_OBJECT;
}
bool RedJsonValue_IsArray(RedJsonValueHandle hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_ARRAY;
}
bool RedJsonValue_IsNull(RedJsonValueHandle hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_NULL;
}

RedJsonObjectHandle RedJsonObject_New()
{
    RedJsonObjectHandle hNew;
    hNew = malloc(sizeof(RedJsonObject));
    hNew->hash = _ZObjHash_new();
    Ref_Init(hNew, _RedJsonValue_Free);
    return hNew;
}

void RedJsonObject_Set(RedJsonObjectHandle hObj, char * szKey, RedJsonValueHandle hVal)
{
    hVal->refcnt++;
    _ZObjHash_insert(hObj->hash, szKey, hVal);
}

void RedJsonObject_SetString(RedJsonObjectHandle hObj, char * szKey, char *szVal)
{
    RedJsonValueHandle newVal = RedJsonValue_FromString(szVal);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetNumber(RedJsonObjectHandle hObj, char * szKey, double val)
{
    RedJsonValueHandle newVal = RedJsonValue_FromNumber(val);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetObject(RedJsonObjectHandle hObj, char * szKey, RedJsonObjectHandle hObjVal)
{
    RedJsonValueHandle newVal = RedJsonValue_FromObject(hObjVal);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetArray(RedJsonObjectHandle hObj, char * szKey, RedJsonArrayHandle hArray)
{
    RedJsonValueHandle newVal = RedJsonValue_FromArray(hArray);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetBoolean(RedJsonObjectHandle hObj, char * szKey, bool val)
{
    RedJsonValueHandle newVal = RedJsonValue_FromBoolean(hObj);
    _ZObjHash_insert(hObj->hash, szKey, newVal);
}

RedJsonValueHandle RedJsonObject_Get(RedJsonObjectHandle hObj, char * szKey)
{
    RedJsonValueHandle hVal;
    bool contains;
    contains = _ZObjHash_get(hObj->hash, szKey, &hVal);
    return contains ? hVal : NULL;
}

RedJsonValueTypeEnum RedJsonObject_GetType(RedJsonObjectHandle hObj, char * szKey)
{
    RedJsonValueHandle hVal;
    bool contains;
    contains = _ZObjHash_get(hObj, szKey, &hVal);
    return contains ? hVal->type : RED_JSON_VALUE_TYPE_INVALID;
}

char * RedJsonObject_GetString(RedJsonObjectHandle hObj, char * szKey)
{
}
double RedJsonObject_GetNumber(RedJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
RedJsonObjectHandle RedJsonObject_GetObject(RedJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
RedJsonArrayHandle RedJsonObject_GetArray(RedJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
bool RedJsonObject_GetBoolean(RedJsonObjectHandle hObj, char * szKey);

bool RedJsonObject_IsValueString(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_STRING;
}
bool RedJsonObject_IsValueNumber(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_NUMBER;
}
bool RedJsonObject_IsValueObject(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_OBJECT;
}
bool RedJsonObject_IsValueArray(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_ARRAY;
}
bool RedJsonObject_IsValueBoolean(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_BOOLEAN;
}
bool RedJsonObject_IsValueNull(RedJsonObjectHandle hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_NULL;
}

void RedJsonObject_Unset(RedJsonObjectHandle hObj, char * szKey)
{
    // TODO
}
bool RedJsonObject_HasKey(RedJsonObjectHandle hObj, char * szKey)
{
    return _ZObjHash_has(hObj, szKey);
}

RedJsonArrayHandle RedJsonArray_New()
{
    RedJsonArrayHandle hNew;
    hNew = malloc(sizeof(RedJsonArrayHandle));
    hNew->items = ZARRAY_NEW(RedJsonValue, 0);
    hNew->sz = strdup(sz);
    Ref_Init(hNew, _RedJsonValue_Free);
    return hNew;
}

RedJsonArrayHandle RedJsonArray_NumItems(RedJsonArrayHandle hArray)
{
    return ZARRAY_NUM_ITEMS(hArray->items);
}

void RedJsonArray_Append(RedJsonArrayHandle hArray, RedJsonValueHandle hVal)
{
    ZARRAY_APPEND(hArray->items, hVal)
}
void RedJsonArray_AppendString(RedJsonArrayHandle hArray, char * szVal)
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_FromString(szVal);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendNumber(RedJsonArrayHandle hArray, double val);
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_FromNumber(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendObject(RedJsonArrayHandle hArray, RedJsonObjectHandle hObj);
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_FromObject(hObj);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendArray(RedJsonArrayHandle hArray, RedJsonArrayHandle hArray);
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_FromArray(hArray);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendBoolean(RedJsonArrayHandle hArray, bool val);
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_FromBoolean(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendNull(RedJsonArrayHandle hArray);
{
    RedJsonValueHandle hVal;
    hVal = RedJsonValue_Null();
    ZARRAY_APPEND(hArray->items, hVal);
}
bool RedJsonArray_IsEntryString(RedJsonArrayHandle hArray, unsigned idx)
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_STRING);
}
bool RedJsonArray_IsEntryNumber(RedJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_NUMBER);
}
bool RedJsonArray_IsEntryObject(RedJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_OBJECT);
}
bool RedJsonArray_IsEntryArray(RedJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_ARRAY);
}
bool RedJsonArray_IsEntryBoolean(RedJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_BOOLEAN);
}
bool RedJsonArray_IsEntryNull(RedJsonArrayHandle hArray, unsigned idx);
{
    return (hArray->items[idx].type == RED_JSON_VALUE_TYPE_NULL);
}


_Value_ToJson(ZStringChain chain, RedJsonValueHandle hVal)
{
    switch (hVal->type)
    {
        case RED_JSON_VALUE_TYPE_STRING:
        {
            // TODO: escape
            ZStringChain_AppendPrintf(chain, "\"%s\"", hVal->sz)
            break;
        }
        case RED_JSON_VALUE_TYPE_NUMBER:
        {
            // TODO: formatting?
            ZStringChain_AppendPrintf(chain, "%f", hVal->number)
            break;
        }
        case RED_JSON_VALUE_TYPE_OBJECT:
        {
            // TODO
            break;
        }
        case RED_JSON_VALUE_TYPE_ARRAY:
        {
            ZStringChain_AppendChars(chain, "[");
            ZARRAY(RedJsonValueHandle) items = hVal->hArray->items;
            int numItems = ZARRAY_NUM_ITEMS(items);
            for (i = 0; i < numItems; i++)
            {
                RedJsonValueHandle hItemVal = items[i];
                _Value_ToJson(chain, RedJsonValueHandle hVal);
                if (i < numItems - 1)
                    ZStringChain_Append(",");
            }
            ZStringChain_AppendChars(chain, "]");
            break;
        }
        case RED_JSON_VALUE_TYPE_NULL:
        {
            ZStringChain_AppendChars(chain, "null")
            break;
        }
    }
}

char * RedJsonValue_ToJsonString(RedJsonValueHandle hVal)
{
    ZStringListHandle chain = ZStringList_New();
    char * out;
    _Value_ToJson(chain, hVal);
    out = ZStringList_ToNewChars();
    ZStrinChain_Unref(chain);
    return out;
}
