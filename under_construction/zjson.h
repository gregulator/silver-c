#ifndef ZJSON_INCLUDED
#define ZJSON_INCLUDED

#include <stdbool.h>

typedef enum
{
    ZJSON_VALUE_TYPE_INVALID,
    ZJSON_VALUE_TYPE_STRING,
    ZJSON_VALUE_TYPE_NUMBER,
    ZJSON_VALUE_TYPE_OBJECT,
    ZJSON_VALUE_TYPE_ARRAY,
    ZJSON_VALUE_TYPE_BOOLEAN,
    ZJSON_VALUE_TYPE_NULL,
} ZJsonValueTypeEnum;

typedef struct ZJsonObject * ZJsonObjectHandle;

typedef struct ZJsonArray * ZJsonArrayHandle;

typedef struct ZJsonValue * ZJsonValueHandle;

ZJsonValueHandle ZJsonValue_FromString(char * sz); /* String is copied */
ZJsonValueHandle ZJsonValue_FromNumber(double val);
ZJsonValueHandle ZJsonValue_FromObject(ZJsonObjectHandle hObj);
ZJsonValueHandle ZJsonValue_FromArray(ZJsonArrayHandle hArray);
ZJsonValueHandle ZJsonValue_FromBoolean(bool val);
ZJsonValueHandle ZJsonValue_Null();

char * ZJsonValue_GetString(ZJsonValueHandle hVal); /* String is copied */
double ZJsonValue_GetNumber(ZJsonValueHandle hVal);
ZJsonObjectHandle ZJsonValue_GetObject(ZJsonValueHandle hVal);
ZJsonArrayHandle ZJsonValue_GetArray(ZJsonValueHandle hVal);

bool ZJsonValue_IsString(ZJsonValueHandle hVal);
bool ZJsonValue_IsNumber(ZJsonValueHandle hVal);
bool ZJsonValue_IsObject(ZJsonValueHandle hVal);
bool ZJsonValue_IsArray(ZJsonValueHandle hVal);
bool ZJsonValue_IsNull(ZJsonValueHandle hVal);

ZJsonObjectHandle ZJsonObject_New();

void ZJsonObject_Set(ZJsonObjectHandle hObj, char * szKey, ZJsonValueHandle hVal);
void ZJsonObject_SetString(ZJsonObjectHandle hObj, char * szKey, char *szVal);
void ZJsonObject_SetNumber(ZJsonObjectHandle hObj, char * szKey, double val);
void ZJsonObject_SetObject(ZJsonObjectHandle hObj, char * szKey, ZJsonObjectHandle hVal);
void ZJsonObject_SetArray(ZJsonObjectHandle hObj, char * szKey, ZJsonArrayHandle hArray);
void ZJsonObject_SetBoolean(ZJsonObjectHandle hObj, char * szKey, bool val);

ZJsonValueHandle ZJsonObject_Get(ZJsonObjectHandle hObj, char * szKey);
ZJsonValueTypeEnum ZJsonObject_GetType(ZJsonObjectHandle hObj, char * szKey);
char * ZJsonObject_GetString(ZJsonObjectHandle hObj, char * szKey);
double ZJsonObject_GetNumber(ZJsonObjectHandle hObj, char * szKey);
ZJsonObjectHandle ZJsonObject_GetObject(ZJsonObjectHandle hObj, char * szKey);
ZJsonArrayHandle ZJsonObject_GetArray(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_GetBoolean(ZJsonObjectHandle hObj, char * szKey);

bool ZJsonObject_IsValueString(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_IsValueNumber(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_IsValueObject(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_IsValueArray(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_IsValueBoolean(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_IsValueNull(ZJsonObjectHandle hObj, char * szKey);

void ZJsonObject_Unset(ZJsonObjectHandle hObj, char * szKey);
bool ZJsonObject_HasKey(ZJsonObjectHandle hObj, char * szKey);

ZJsonArrayHandle ZJsonArray_New();

ZJsonArrayHandle ZJsonArray_NumItems(ZJsonArrayHandle hArray);

void ZJsonArray_Append(ZJsonArrayHandle hArray, ZJsonValueHandle hVal);
void ZJsonArray_AppendString(ZJsonArrayHandle hArray, char * szVal);
void ZJsonArray_AppendNumber(ZJsonArrayHandle hArray, double val);
void ZJsonArray_AppendObject(ZJsonArrayHandle hArray, ZJsonObjectHandle hVal);
void ZJsonArray_AppendArray(ZJsonArrayHandle hArray, ZJsonArrayHandle hVal);
void ZJsonArray_AppendBoolean(ZJsonArrayHandle hArray, bool val);
void ZJsonArray_AppendNull(ZJsonArrayHandle hArray);

void ZJsonArray_SetEntry(ZJsonArrayHandle hArray, unsigned idx, ZJsonValueHandle hVal);
void ZJsonArray_SetEntryString(ZJsonArrayHandle hArray, unsigned idx, char * szVal);
void ZJsonArray_SetEntryNumber(ZJsonArrayHandle hArray, unsigned idx, double val);
void ZJsonArray_SetEntryObject(ZJsonArrayHandle hArray, unsigned idx, ZJsonObjectHandle hVal);
void ZJsonArray_SetEntryArray(ZJsonArrayHandle hArray, unsigned idx, ZJsonArrayHandle hVal);
void ZJsonArray_SetEntryBoolean(ZJsonArrayHandle hArray, unsigned idx, bool val);
void ZJsonArray_SetEntryNull(ZJsonArrayHandle hArray, unsigned idx);

ZJsonValueHandle ZJsonArray_GetEntry(ZJsonArrayHandle hArray);
char * ZJsonArray_GetEntryString(ZJsonArrayHandle hArray, unsigned idx);
double ZJsonArray_GetEntryNumber(ZJsonArrayHandle hArray, unsigned idx);
ZJsonObjectHandle ZJsonArray_GetEntryObject(ZJsonArrayHandle hArray, unsigned idx);
ZJsonArrayHandle ZJsonArray_GetEntryArray(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_GetEntryBoolean(ZJsonArrayHandle hArray, unsigned idx);

bool ZJsonArray_IsEntryString(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_IsEntryNumber(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_IsEntryObject(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_IsEntryArray(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_IsEntryBoolean(ZJsonArrayHandle hArray, unsigned idx);
bool ZJsonArray_IsEntryNull(ZJsonArrayHandle hArray, unsigned idx);

char * ZJsonValue_ToJsonString(ZJsonValueHandle hVal);

#endif
