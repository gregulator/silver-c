#include "red_json.h"

#include "red_hash.h"
#include "red_string.h"
#include "../under_construction/zarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REF(hObj) ((hObj)->refcnt++, (hObj))

static char * _StrDup(const char *s)
{
    char * out = malloc(strlen(s) + 1);
    if (out)
        strcpy(out, s);
    return out;
}

typedef struct RedJsonValue_t
{
    int refcnt;
    RedJsonValueTypeEnum type;
    union
    {
        char *sz;
        double dbl;
        RedJsonObject hObj;
        RedJsonArray hArray;
        bool boolean;
    } val;
} RedJsonValue_t;

typedef struct RedJsonObject_t
{
    int refcnt;
    RedHash hash;
} RedJsonObject_t;

typedef struct RedJsonArray_t
{
    int refcnt;
    ZARRAY(RedJsonValue) items;
} RedJsonArray_t;

RedJsonValue RedJsonValue_FromString(char * sz)
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_STRING;
    hNew->val.sz = _StrDup(sz);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValue RedJsonValue_FromNumber(double val)
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_NUMBER;
    hNew->val.dbl = val;
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValue RedJsonValue_FromObject(RedJsonObject hObj)
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_OBJECT;
    hNew->val.hObj = REF(hObj);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValue RedJsonValue_FromArray(RedJsonArray hArray)
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_NUMBER;
    hNew->val.hArray = REF(hArray);
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValue RedJsonValue_FromBoolean(bool val)
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_BOOLEAN;
    hNew->val.boolean = val;
    hNew->refcnt = 0;
    return hNew;
}

RedJsonValue RedJsonValue_Null()
{
    RedJsonValue hNew;
    hNew = malloc(sizeof(RedJsonValue_t));
    hNew->type = RED_JSON_VALUE_TYPE_NULL;
    hNew->refcnt = 0;
    return hNew;
}

void _RedJsonValue_Free(RedJsonValue hVal)
{
    free(hVal->val.sz);
    free(hVal);
}

char * RedJsonValue_GetString(RedJsonValue hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_STRING);
    return _StrDup(hVal->val.sz);
}
double RedJsonValue_GetNumber(RedJsonValue hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_NUMBER);
    return hVal->val.dbl;
}
RedJsonObject RedJsonValue_GetObject(RedJsonValue hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_OBJECT);
    return REF(hVal->val.hObj);
}
RedJsonArray RedJsonValue_GetArray(RedJsonValue hVal)
{
    assert(hVal->type == RED_JSON_VALUE_TYPE_ARRAY);
    return REF(hVal->val.hArray);
}

bool RedJsonValue_IsString(RedJsonValue hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_STRING;
}
bool RedJsonValue_IsNumber(RedJsonValue hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_NUMBER;
}
bool RedJsonValue_IsObject(RedJsonValue hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_OBJECT;
}
bool RedJsonValue_IsArray(RedJsonValue hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_ARRAY;
}
bool RedJsonValue_IsNull(RedJsonValue hVal)
{
    return hVal->type == RED_JSON_VALUE_TYPE_NULL;
}

RedJsonObject RedJsonObject_New()
{
    RedJsonObject jsonObj;
    jsonObj = malloc(sizeof(RedJsonObject_t));
    jsonObj->hash = RedHash_New(0);
    jsonObj->refcnt = 1;
    return jsonObj;
}

void RedJsonObject_Set(RedJsonObject hObj, char * szKey, RedJsonValue hVal)
{
    hVal->refcnt++;
    RedHash_InsertS(hObj->hash, szKey, hVal);
}

void RedJsonObject_SetString(RedJsonObject hObj, char * szKey, char *szVal)
{
    RedJsonValue newVal = RedJsonValue_FromString(szVal);
    RedHash_InsertS(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetNumber(RedJsonObject hObj, char * szKey, double val)
{
    RedJsonValue newVal = RedJsonValue_FromNumber(val);
    RedHash_InsertS(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetObject(RedJsonObject hObj, char * szKey, RedJsonObject hObjVal)
{
    RedJsonValue newVal = RedJsonValue_FromObject(hObjVal);
    RedHash_InsertS(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetArray(RedJsonObject hObj, char * szKey, RedJsonArray hArray)
{
    RedJsonValue newVal = RedJsonValue_FromArray(hArray);
    RedHash_InsertS(hObj->hash, szKey, newVal);
}

void RedJsonObject_SetBoolean(RedJsonObject hObj, char * szKey, bool val)
{
    RedJsonValue newVal = RedJsonValue_FromBoolean(hObj);
    RedHash_InsertS(hObj->hash, szKey, newVal);
}

RedJsonValue RedJsonObject_Get(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetWithDefaultS(hObj->hash, szKey, NULL);
    return jsonVal;
}

RedJsonValueTypeEnum RedJsonObject_GetType(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetWithDefaultS(hObj->hash, szKey, NULL);
    return jsonVal ? jsonVal->type : RED_JSON_VALUE_TYPE_INVALID;
}

char * RedJsonObject_GetString(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetS(hObj->hash, szKey);
    return jsonVal->val.sz;
}
double RedJsonObject_GetNumber(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetS(hObj->hash, szKey);
    return jsonVal->val.dbl;
}
RedJsonObject RedJsonObject_GetObject(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetS(hObj->hash, szKey);
    return jsonVal->val.hObj;
}
RedJsonArray RedJsonObject_GetArray(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetS(hObj->hash, szKey);
    return jsonVal->val.hArray;
}
bool RedJsonObject_GetBoolean(RedJsonObject hObj, char * szKey)
{
    RedJsonValue jsonVal;
    jsonVal = RedHash_GetS(hObj->hash, szKey);
    return jsonVal->val.boolean;
}

bool RedJsonObject_IsValueString(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_STRING;
}
bool RedJsonObject_IsValueNumber(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_NUMBER;
}
bool RedJsonObject_IsValueObject(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_OBJECT;
}
bool RedJsonObject_IsValueArray(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_ARRAY;
}
bool RedJsonObject_IsValueBoolean(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_BOOLEAN;
}
bool RedJsonObject_IsValueNull(RedJsonObject hObj, char * szKey)
{
    return RedJsonObject_GetType(hObj, szKey) == RED_JSON_VALUE_TYPE_NULL;
}

void RedJsonObject_Unset(RedJsonObject hObj, char * szKey)
{
    // TODO
}
bool RedJsonObject_HasKey(RedJsonObject hObj, char * szKey)
{
    return RedHash_HasKeyS(hObj->hash, szKey);
}

RedJsonArray RedJsonArray_New()
{
    RedJsonArray hNew;
    hNew = malloc(sizeof(RedJsonArray_t));
    hNew->items = ZARRAY_NEW(RedJsonValue, 0);
    hNew->refcnt = 1;
    return hNew;
}

RedJsonArray RedJsonArray_NumItems(RedJsonArray hArray)
{
    /*return ZARRAY_NUM_ITEMS(hArray->items);*/
    return NULL;
}

void RedJsonArray_Append(RedJsonArray hArray, RedJsonValue hVal)
{
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendString(RedJsonArray hArray, char * szVal)
{
    RedJsonValue hVal;
    hVal = RedJsonValue_FromString(szVal);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendNumber(RedJsonArray hArray, double val)
{
    RedJsonValue hVal;
    hVal = RedJsonValue_FromNumber(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendObject(RedJsonArray hArray, RedJsonObject hObj)
{
    RedJsonValue hVal;
    hVal = RedJsonValue_FromObject(hObj);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendArray(RedJsonArray jsonArray, RedJsonArray val)
{
    RedJsonValue jsonVal;
    jsonVal = RedJsonValue_FromArray(val);
    ZARRAY_APPEND(jsonArray->items, jsonVal);
}
void RedJsonArray_AppendBoolean(RedJsonArray hArray, bool val)
{
    RedJsonValue hVal;
    hVal = RedJsonValue_FromBoolean(val);
    ZARRAY_APPEND(hArray->items, hVal);
}
void RedJsonArray_AppendNull(RedJsonArray hArray)
{
    RedJsonValue hVal;
    hVal = RedJsonValue_Null();
    ZARRAY_APPEND(hArray->items, hVal);
}
bool RedJsonArray_IsEntryString(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_STRING);
}
bool RedJsonArray_IsEntryNumber(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_NUMBER);
}
bool RedJsonArray_IsEntryObject(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_OBJECT);
}
bool RedJsonArray_IsEntryArray(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_ARRAY);
}
bool RedJsonArray_IsEntryBoolean(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_BOOLEAN);
}
bool RedJsonArray_IsEntryNull(RedJsonArray hArray, unsigned idx)
{
    return (ZARRAY_AT(hArray->items, idx)->type == RED_JSON_VALUE_TYPE_NULL);
}


void _Value_ToJson(RedStringList chain, RedJsonValue hVal)
{
    switch (hVal->type)
    {
        case RED_JSON_VALUE_TYPE_STRING:
        {
            // TODO: escape
            RedStringList_AppendPrintf(chain, "\"%s\"", hVal->val.sz);
            break;
        }
        case RED_JSON_VALUE_TYPE_NUMBER:
        {
            // TODO: formatting?
            RedStringList_AppendPrintf(chain, "%lf", hVal->val.dbl);
            break;
        }
        case RED_JSON_VALUE_TYPE_BOOLEAN:
        {
            RedStringList_AppendPrintf(chain, "%s", hVal->val.boolean ? "true" : "false");
            break;
        }
        case RED_JSON_VALUE_TYPE_OBJECT:
        {
            RedHashIterator_t iter;
            const char *key;
            size_t keySize;
            const void *value;
            RedStringList_AppendPrintf(chain, "{\n");
            RED_HASH_FOREACH(iter, hVal->val.hObj->hash, (const void **)&key, &keySize, &value)
            {
                RedJsonValue val = (RedJsonValue)value;
                RedStringList_AppendPrintf(chain, "\"%s\" : ", key);
                _Value_ToJson(chain, val);
                RedStringList_AppendPrintf(chain, ",\n");
            }
            RedStringList_AppendPrintf(chain, "}\n");
            break;
        }
        case RED_JSON_VALUE_TYPE_ARRAY:
        {
            int i, numItems;
            ZARRAY(RedJsonValue) items;
            RedStringList_AppendChars(chain, "[");
            items = (void *)hVal->val.hArray->items;
            numItems = ZARRAY_NUM_ITEMS(items);
            for (i = 0; i < numItems; i++)
            {
                RedJsonValue hItemVal = ZARRAY_AT(items, i);
                _Value_ToJson(chain, hItemVal);
                if (i < numItems - 1)
                    RedStringList_AppendChars(chain, ",");
            }
            RedStringList_AppendChars(chain, "]");
            break;
        }
        case RED_JSON_VALUE_TYPE_NULL:
        {
            RedStringList_AppendChars(chain, "null");
            break;
        }
        case RED_JSON_VALUE_TYPE_INVALID:
        {
            assert("!Unexpected JSON type!");
            break;
        }
    }
}

char * RedJsonValue_ToJsonString(RedJsonValue hVal)
{
    RedStringList chain = RedStringList_New();
    char * out;
    _Value_ToJson(chain, hVal);
    out = RedStringList_ToNewChars(chain);
    RedStringList_Free(chain);
    return out;
}
