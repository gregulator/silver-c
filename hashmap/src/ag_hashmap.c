#include <ag_hashmap.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

struct AgHashMapNodeHeader
{
    struct AgHashMapNodeHeader *next;
    void *value;
    unsigned keySize;
    char keyStart; /* First byte of key */
};

typedef struct AgHashMapNodeHeader AgHashMapNodeHeader;

typedef struct AgHashMap
{
    unsigned numEntries;
    unsigned numBuckets;
    unsigned sizeLevel;
    AgHashMapNodeHeader ** buckets;
} AgHashMap;

#define _AG_HASH_MULTIPLIER (65599)

#define _NODE_KEY(header) (&((header)->keyStart))

static const unsigned _hashMapValidBucketCounts[] =
{
    23,
    509,
    1021,
    2053,
    4093,
    8191,
    16301,
    32771,
    65521,
    131071,
    /* TODO: extend this list */
};
#define _AG_HASHMAP_NUM_SIZE_LEVELS (sizeof(_hashMapValidBucketCounts) / sizeof(unsigned))

static uint32_t _Hash(const void *key, unsigned keySize, unsigned numBuckets)
{
  uint32_t i;
  uint32_t hash = 0;
  for (i = 0; i < keySize; i++) {
    hash = hash * _AG_HASH_MULTIPLIER + (uint32_t)(((char *)key)[i]);
  }
  return hash % numBuckets;
}

static bool 
    _KeysMatch(
        unsigned keySize0, 
        void *key0, 
        unsigned keySize1, 
        void *key1)
{
    if (keySize0 != keySize1)
        return false;
    return (memcmp(key0, key1, keySize0) == 0);
}

static void _AutoResize(AgHashMapHandle hMap)
{
    AgHashMap oldMap;
    AgHashMapNodeHeader *pNode;
    unsigned newHashVal;
    int i;

    /* Do we need to do anything? */
    if (hMap->numEntries < hMap->numBuckets)
        return;

    /* Do nothing if we've reached the maximum number of buckets */
    if (hMap->sizeLevel >= _AG_HASHMAP_NUM_SIZE_LEVELS-1)
        return;

    /* Increase the number of buckets */
    memcpy(&oldMap, hMap, sizeof(AgHashMap));
    hMap->sizeLevel++;
    hMap->numBuckets = _hashMapValidBucketCounts[hMap->sizeLevel];

    /* Create larger bucket array */
    hMap->buckets = calloc(hMap->numBuckets, sizeof(AgHashMapNodeHeader *));

    /* Move nodes to new array */
    for (i = 0; i < oldMap.numBuckets; i++)
    {
        pNode = oldMap.buckets[i];
        //newHashVal = _Hash(pNode->key, keySize, hMap->numBuckets);
        // TODO: finish ... //
        assert(!"Not implemented");
    }
}

AgHashMapHandle AgHashMap_Create(unsigned numItemsHint)
{
    AgHashMapHandle hNew;
    int i;
    hNew = malloc(sizeof(AgHashMap));
    hNew->numEntries = 0;
    hNew->sizeLevel = 0;
    i = 0;
    do
    {
        hNew->numBuckets = _hashMapValidBucketCounts[i];
        i++;
    } while ((i < _AG_HASHMAP_NUM_SIZE_LEVELS) &&
             (hNew->numBuckets < numItemsHint));
    hNew->buckets = calloc(hNew->numBuckets, sizeof(AgHashMapNodeHeader *));
    return hNew;
}

void 
    AgHashMap_Insert(
            AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize, 
            void *value)
{
    unsigned hashval;
    AgHashMapNodeHeader *hNewNode;
    AgHashMapNodeHeader *tmp;

    assert(!AgHashMap_HasKey(hMap, key, keySize));
    assert(keySize > 0);

    hashval = _Hash(key, keySize, hMap->numBuckets);
    tmp = hMap->buckets[hashval];

    hNewNode = malloc(sizeof(AgHashMapNodeHeader) + keySize-1);
    hNewNode->next = tmp;
    hNewNode->value = value;
    hNewNode->keySize = keySize;
    memcpy(&hNewNode->keyStart, key, keySize);

    hMap->buckets[hashval] = hNewNode;
    hMap->numEntries++;

    _AutoResize(hMap);
}

void *
    AgHashMap_Get(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize)
{
    uint32_t hashval = _Hash(key, keySize, hMap->numBuckets);
    AgHashMapNodeHeader *pNode = hMap->buckets[hashval];
    while (pNode)
    {
        if (_KeysMatch(pNode->keySize, _NODE_KEY(pNode), keySize, key))
            return pNode->value;
        pNode = pNode->next;
    };
    assert(!"AgHashMap_Get: key not found");
}

void *
    AgHashMap_Update(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize,
            void *value)
{
    uint32_t hashval = _Hash(key, keySize, hMap->numBuckets);
    AgHashMapNodeHeader *pNode = hMap->buckets[hashval];
    while (pNode)
    {
        if (_KeysMatch( pNode->keySize, _NODE_KEY(pNode), keySize, key))
        {
            pNode->value = value;
        }
        pNode = pNode->next;
    };
    assert(!"AgHashMap_Update: key not found");
}


bool 
    AgHashMap_HasKey(
            const AgHashMapHandle hMap, 
            const void *key, 
            unsigned keySize)
{
    uint32_t hashval = _Hash(key, keySize, hMap->numBuckets);
    AgHashMapNodeHeader *pNode = hMap->buckets[hashval];
    while (pNode)
    {
        if (_KeysMatch(pNode->keySize, _NODE_KEY(pNode), keySize, key))
            return true;
        pNode = pNode->next;
    };
    return false;
}

unsigned AgHashMap_NumItems(const AgHashMapHandle hMap)
{
    return hMap->numEntries;
}

bool AgHashMap_IsEmpty(const AgHashMapHandle hMap)
{
    return (hMap->numEntries == 0);
}
