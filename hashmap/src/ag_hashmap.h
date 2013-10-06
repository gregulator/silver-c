/*
 * AgHashMap -- Hash table module.
 *
 *  OVERVIEW
 *
 *      The AgHashMap module provides a key-value memory store with constant-time
 *      lookup by key, where:
 *          - The key can be any block of memory.  The key is fully copied.
 *          - The value is a pointer.  You can also use intptr_t, uintptr_t, or
 *          smaller integer types, by casting to/from (void*), to store small
 *          integer values.  Keep in mind that the integer size for which it is
 *          safe to do this is platform dependent.
 *
 *  BASIC OPERATIONS
 *
 *      The client is responsible for allocating the value.  For example:
 *
 *          typedef struct
 *          {
 *              float x, y, z, w;
 *          } Quaternion;
 *
 *          ...
 *          pVal = malloc(sizeof(Quaternion));
 *
 *      Create a hashmap:
 *
 *          AgHashMapHandle hMap;
 *          hMap = AgHashMap_New(0);
 *
 *      Insert a key-value pair (string key):
 *
 *          AG_HASHMAP_INSERT(hMap, "My Quaternion", pVal);
 *
 *      Lookup by key (string key), defaulting to NULL:
 *
 *          pVal = AG_HASHMAP_GET_OR_NULL(hMap, "My Quaternion");
 *
 *      For non-string keys, use the AgHashMap_Insert/AgHashMap_Get/etc routines
 *      instead of the macros.
 *
 */
#ifndef AG_HASHMAP_INCLUDED
#define AG_HASHMAP_INCLUDED

#include <stdbool.h>

/*
 * AgHashMapHandle - Represents a hash table.
 */
typedef struct AgHashMap * AgHashMapHandle;

/*
 * AgHashMapEntry - Public structure used by AgHashMap_ListEntries routine.
 */
typedef struct
{
    void *key;
    unsigned keySize;
    void *value;
} AgHashMapEntry;

/*
 * AgHashMap_New - Create a new (emtpy) hash table and return handle to it.
 *
 *      <numItemsHint> is a hint to the implementation about how many entries
 *          the hash table will store, which can help the implementation choose
 *          an optimal number of buckets.  If unsure, just set to 0.
 */
AgHashMapHandle AgHashMap_New(unsigned numItemsHint);

/*
 * AG_HASHMAP_INSERT - Insert a key-value pair (null-terminated-string key).
 *
 *      This routine will assert if the key already exists in the hash table.
 *      If you would rather have it update the entry's value, use
 *      AG_HASHMAP_INSERT_OR_UPDATE instead.
 *
 *      <pMap> is the hash table to insert into.
 *
 *      <key> is a pointer to a null-terminated string which will be copied and
 *          used as the entry's key.
 *
 *      <value> is any pointer type, and will be used as the entry's value.
 *
 *      This macro references <key> twice, so avoid complex expressions for
 *      that parameter.
 */
#define AG_HASHMAP_INSERT(hMap, key, value) \
    AgHashMap_Insert((hMap), (key), strlen(key)+1, (value))

/*
 * AgHashMap_Insert - Insert a key-value pair (general key).
 *
 *      This routine will assert if the key already exists in the hash table.
 *      If you would rather have it update the entry's value, use
 *      AgHashMap_InsertOrUpdate instead.
 *
 *      <pMap> is the hash table to insert into.
 *
 *      <key> is a pointer to a block of memory which will be copied and used
 *          as the entry's key.
 *
 *      <keySize> is the size of the key in bytes.
 *
 *      <value> is any pointer type, and will be used as the entry's value.
 *          that parameter.
 */
void 
    AgHashMap_Insert(
            AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize, 
            void *value);

/*
 * AG_HASHMAP_GET - Get the value associated with a key (null-terminated string key).
 *
 *      This routine will assert if the key does not exist in the hash table.
 *      If you would rather have it return a default value, use
 *      AG_HASHMAP_GET_WITH_DEFAULT instead.
 *
 *      <pMap> is the hash table to lookup into.
 *
 *      <key> is a pointer to a null-terminated string key to be looked up.
 *
 *      This macro references <key> twice, so avoid complex expressions for
 *      that parameter.
 */
#define AG_HASHMAP_GET(hMap, key) AgHashMap_Get((hMap), (key), strlen(key)+1)

/*
 * AgHashMap_Get - Get the value associated with a key (general key).
 *
 *      <pMap> is the hash table to lookup into.
 *
 *      <key> is a pointer to a block of memory which will be used as the key
 *          for the lookup.
 *
 *      <keySize> is the size of the key in bytes.
 */
void *
    AgHashMap_Get(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize);

/*
 * AG_HASHMAP_GET_WITH_DEFAULT - Get the value associated with a key
 *      (null-terminated string key), or a default value if that key is not in
 *      the hash table.
 *
 *      <pMap> is the hash table to lookup into.
 *
 *      <key> is a pointer to a null-terminated string key to be looked up.
 *
 *      <defaultValue> is a pointer value to return if the key is not found.
 *          It is common to use NULL here (although NULL is also a valid
 *          key-value value, so, in general, it is not safe to assume that a
 *          returned NULL value (or any default value, for that matter)  means
 *          the key was not found).
 *
 *      This macro references <key> twice, so avoid complex expressions for
 *      that parameter.
 */
#define AG_HASHMAP_GET_WITH_DEFAULT(hMap, key, defaultValue) \
    AgHashMap_GetWithDefault((hMap), (key), strlen(key)+1, defaultValue)

/*
 * AgHashMap_GetWithDefault - Get the value associated with a key (general key),
 *      or a default value if that key is not in the hash table.
 *
 *      <pMap> is the hash table to lookup into.
 *
 *      <key> is a pointer to a block of memory which will be used as the key
 *          for the lookup.
 *
 *      <keySize> is the size of the key in bytes.
 *
 *      <defaultValue> is a pointer value to return if the key is not found.
 *          It is common to use NULL here (although NULL is also a valid
 *          key-value value, so, in general, it is not safe to assume that a
 *          returned NULL value (or any default value, for that matter)  means
 *          the key was not found).
 */
void *
    AgHashMap_GetWithDefault(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize,
            void *defaultValue);

/*
 * AG_HASHMAP_UPDATE - Update the value associated with a key (null-terminated
 *      string keys).
 *
 *      This routine will assert if the key does not exist in the hash table.
 *      If you would rather have it insert the key-value, use
 *      AG_HASHMAP_UPDATE_OR_INSERT instead.
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a null-terminated string key to update.
 *
 *      <value> is any pointer type, and will replace the entry's old value.
 *
 *      Returns the old value that was replaced (which is helpful if you need
 *      to free or dereference it).
 */
#define AG_HASHMAP_UPDATE(hMap, key, value) \
    AgHashMap_Update((hMap), (key), strlen(key)+1, value)

/*
 * AgHashMap_Update - Update the value associated with a key (general key).
 *
 *      This routine will assert if the key does not exist in the hash table.
 *      If you would rather have it insert the key-value, use
 *      AgHashMap_UpdateOrInsert instead.
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a block of memory which will be used as the key
 *          to update.
 *
 *      <keySize> is the size of the key in bytes.
 *
 *      <value> is any pointer type, and will replace the entry's old value.
 *
 *      Returns the old value that was replaced (which is helpful if you need
 *      to free or dereference it).
 */
void *
    AgHashMap_Update(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize,
            void *value);

/*
 * AG_HASHMAP_UPDATE_OR_INSERT - Update the value associated with a key, inserting
 *      the value if the key does not exist (null-terminated string keys).
 *
 *      <pMap> is the hash table to update.
 *
 *      <replacedValue> is a pointer to a pointer that will be set to the
 *          replaced value if an update occured.  If an insert occured, this
 *          parameter is ignored.  Use the boolean return value to determine
 *          which operation occured.  If <replacedValue> is NULL it is ignored.
 *
 *      <key> is a pointer to a null-terminated string key to update.
 *
 *      <value> is any pointer type, and will replace the entry's old value.
 *
 *      Returns a boolean, which is TRUE if an update occured or FALSE if an
 *      insert occured.
 */
#define AG_HASHMAP_UPDATE_OR_INSERT(hMap, replacedValue, key, value) \
    AgHashMap_UpdateOrInsert(\
            (hMap), \
            (replacedValue), \
            (key), \
            strlen(key)+1, \
            value)

/*
 * AgHashMap_UpdateOrInsert - Update the value associated with a key, inserting
 *      the value if the key does not exist (generalized key).
 *
 *      <pMap> is the hash table to update.
 *
 *      <replacedValue> is a pointer to a pointer that will be set to the
 *          replaced value if an update occured.  If an insert occured, this
 *          parameter is ignored.  Use the boolean return value to determine
 *          which operation occured.  If <replacedValue> is NULL it is ignored.
 *
 *      <key> is a pointer to a block of memory which will be used as the key
 *          to update.
 *
 *      <keySize> is the size of the key in bytes.
 *
 *      <value> is any pointer type, and will replace the entry's old value.
 *
 *      Returns a boolean, which is TRUE if an update occured or FALSE if an
 *      insert occured.
 */
bool
    AgHashMap_UpdateOrInsert(
            const AgHashMapHandle hMap,
            void ** replacedValue,
            const void *key, 
            unsigned keySize,
            void *value);

/*
 * AG_HASHMAP_REMOVE - Remove a key-value pair from hash table (null-terminated
 *      string keys).
 *
 *      This routine will assert if the key does not exist in the hash table.
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a null-terminated string key identifying the
 *          entry to remove.
 *
 *      Returns the value for the entry that was removed.
 */
#define AG_HASHMAP_REMOVE(hMap, key) \
    AgHashMap_Remove((hMap), (key), strlen(key)+1)

/*
 * AgHashMap_Remove - Remove a key-value pair from hash table (general key).
 *
 *      This routine will assert if the key does not exist in the hash table.
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a block of memory which is the key identifying
 *          which entry to remove.
 *
 *      <keySize> is the size of the key in bytes.
 *
 *      Returns the value for the entry that was removed.
 */
void *
    AgHashMap_Remove(
            AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize);

/*
 * AG_HASHMAP_HAS_KEY - Determine if hash table contains a key (null-terminated
 *      string keys).
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a null-terminated string key to check.
 *
 *      Returns TRUE if the key was found in <pMap>.
 */
#define AG_HASHMAP_HAS_KEY(hMap, key) \
    AgHashMap_HasKey((hMap), (key), strlen(key)+1)

/*
 * AgHashMap_HasKey - Determine if hash table contains a key (general key).
 *
 *      <pMap> is the hash table to update.
 *
 *      <key> is a pointer to a null-terminated string key to check.
 *
 *      Returns TRUE if the key was found in <pMap>, FALSE otherwise.
 */
bool 
    AgHashMap_HasKey(
            const AgHashMapHandle hHash, 
            const void *key, 
            unsigned keySize);

/*
 * AgHashMap_NumItems - Get the number of entries in a hash table.
 *
 *      <pMap> is the hash table to check.
 */
unsigned AgHashMap_NumItems(const AgHashMapHandle hHash);

/*
 * AgHashMap_IsEmpty - Determine if hash table is empty
 *
 *      <pMap> is the hash table to check.
 */
bool AgHashMap_IsEmpty(const AgHashMapHandle hHash);

/*
 * AgHashMap_Clear - Removes all key-value pairs from a hash table.
 *
 *      <pMap> is the hash table to clear.
 */
bool AgHashMap_Clear(const AgHashMapHandle hHash);

/*
 * Requires free 
unsigned 
    AgHashMap_ListEntries(
            const AgHashMapHandle hHash, 
            , 
            );
*/
#endif
