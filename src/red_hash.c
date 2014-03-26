#include "red_hash.h"
#include <string.h>

typedef struct RedHashNodeHeader
{
    struct RedHashNodeHeader *next;
    void *value;
    unsigned keySize;
    char keyStart; /* First byte of key */
} RedHashNodeHeader;

typedef struct RedHash_t
{
    unsigned sizeLevel;
    unsigned numEntries;
    unsigned numBuckets;
    RedHashNodeHeader ** buckets;
} RedHash_t;

static const unsigned _RedHashValidBucketCounts[] =
{
    /* primes near powers of 2 */
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
    256049,
    512671,
    1281101,
    2562317,
    5194069,
    10991719  
};
#define _REDHASH_NUM_SIZE_LEVELS (sizeof(_RedHashValidBucketCounts) / sizeof(unsigned))

static unsigned _RedHash_Hash(const void *keyobj, size_t keylen, unsigned numBuckets) {
    /* Jenkins algoritm */
    char * key = (char *)keyobj;
    unsigned hash = 0;
    unsigned i;
    for (i = 0; i < keylen; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % numBuckets;
}

RedHash RedHash_Create(unsigned numItemsHint)
{
    RedHash hNew;
    int i;
    hNew = malloc(sizeof(RedHash));
    hNew->numEntries = 0;
    i = 0;
    do
    {
        hNew->numBuckets = _RedHashValidBucketCounts[i];
        i++;
    } while ((i < _REDHASH_NUM_SIZE_LEVELS) &&
             (hNew->numBuckets < numItemsHint));
    hNew->buckets = calloc(hNew->numBuckets, sizeof(RedHashNodeHeader *));
    return hNew;
}

static void _RedHash_AutoResize(RedHash hash)
{
    RedHash_t oldHash;
    RedHashNodeHeader *pNode;
    unsigned i;

    /* Do we need to do anything? */
    if (hash->numEntries < hash->numBuckets)
        return;

    /* Do nothing if we've reached the maximum number of buckets */
    if (hash->sizeLevel >= _REDHASH_NUM_SIZE_LEVELS-1)
        return;

    /* Increase the number of buckets */
    memcpy(&oldHash, hash, sizeof(RedHash));
    hash->sizeLevel++;
    hash->numBuckets = _RedHashValidBucketCounts[hash->sizeLevel];

    /* Create larger bucket array */
    hash->buckets = calloc(hash->numBuckets, sizeof(RedHashNodeHeader *));

    /* Move nodes to new array */
    for (i = 0; i < oldHash.numBuckets; i++)
    {
        pNode = oldHash.buckets[i];
        while (pNode) {
            /* remove from old bucket */
            oldHash.buckets[i] = pNode->next;

            /* rehash */
            unsigned newhashval = _HASH(&pNode->keyStart, hash->numBuckets);
            pNode->next = hash->buckets[newhashval];
            hash->buckets[newhashval] = pNode;

            pNode = oldHash.buckets[i];
        }
    }
}

void 
    RedHash_Insert(
            RedHash hash, 
            const void *key, 
            unsigned keySize, 
            void *value)
{
    unsigned hashval;
    RedHashNodeHeader *pNewNode;
    RedHashNodeHeader *tmp;

    assert(!RedHash_HasKey(hash, key, keySize));
    assert(keySize > 0);

    hashval = _RedHash_Hash(key, keySize, hash->numBuckets);
    tmp = hash->buckets[hashval];

    pNewNode = malloc(sizeof(RedHashNodeHeader) + keySize-1);
    pNewNode->next = tmp;
    pNewNode->value = value;
    pNewNode->keySize = keySize;
    memcpy(&pNewNode->keyStart, key, keySize);

    hash->buckets[hashval] = pNewNode;
    hash->numEntries++;

    _RedHash_AutoResize(hash);
}

void *
    RedHash_Get(
            const RedHash hash, 
            const void *key, 
            unsigned keySize)
{
    RedHashNodeHeader *pNode;
    unsigned hashval;
    hashval = _RedHash_Hash(key, keySize, hash->numBuckets);
    pNode = hash->buckets[hashval];
    while (pNode)
    {
        if (_RedHash_KeysMatch(
                    pNode->keySize, _RedHash_NODE_KEY(pNode), keySize, key))
            return pNode->value;
        pNode = pNode->next;
    };
    assert(!"RedHash_Get: key not found");
}

void *
    RedHash_Update(
            const RedHash hash, 
            const void *key, 
            unsigned keySize,
            void *value)
{
    RedHashNodeHeader *pNode;
    void *oldValue;
    unsigned hashval;
    hashval = _RedHash_Hash(key, keySize, hash->numBuckets);
    pNode = hash->buckets[hashval];
    while (pNode)
    {
        if (_RedHash_KeysMatch(
                    pNode->keySize, _RedHash_NODE_KEY(pNode), keySize, key))
        {
            oldValue = pNode->value;
            pNode->value = value;
            return oldValue;
        }
        pNode = pNode->next;
    };
    assert(!"RedHash_Update: key not found");
}


bool RedHash_HasKey(const RedHash hash, const void *key, unsigned keySize)
{
    RedHashNodeHeader *pNode;
    unsigned hashval;

    hashval = _RedHash_Hash(key, keySize, hash->numBuckets);
    pNode = hash->buckets[hashval];
    while (pNode)
    {
        if (_RedHash_KeysMatch(
                    pNode->keySize, _RedHash_NODE_KEY(pNode), keySize, key))
            return true;
        pNode = pNode->next;
    };
    return false;
}

unsigned RedHash_NumItems(const RedHash hash)
{
    return hash->numEntries;
}

bool RedHash_IsEmpty(const RedHash hash)
{
    return (hash->numEntries == 0);
}
