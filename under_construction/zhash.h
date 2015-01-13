/*
 *  ZHASH -- Hash table
 *
 *  Author: Gregory Prsiament (greg@toruslabs.com)
 *
 *  ===========================================================================
 *  Creative Commons CC0 1.0 Universal - Public Domain 
 *
 *  To the extent possible under law, Gregory Prisament has waived all
 *  copyright and related or neighboring rights to ZARRAY. This work is
 *  published from: United States. 
 *
 *  For details please refer to either:
 *      - http://creativecommons.org/publicdomain/zero/1.0/legalcode
 *      - The LICENSE file in this directory, if present.
 *  ===========================================================================
 */
#ifndef ZHASH_INCLUDED
#define ZHASH_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GENERATE_ZHASH_FIXED_KEY_IMPLEMENTATION(name, key_type, value_type) \
    static const unsigned _##name##_ValidBucketCounts[] =  \
    { \
        23, \
        509, \
        1021, \
        2053, \
        4093, \
        8191, \
        16301, \
        32771, \
        65521, \
        131071, \
        /* TODO: extend this list */ \
    }; \
    typedef struct _##name##_Node { \
        key_type key; \
        value_type value; \
        struct _##name##_Node *next; \
    } _##name##_Node; \
 \
    typedef struct { \
        unsigned sizeLevel; \
        unsigned numItems; \
        unsigned numBuckets; \
        struct _##name##_Node **buckets; /* array of linked lists */ \
    } _##name##_Struct; \
    typedef _##name##_Struct * name; \
 \
    static inline name name##_new() { \
        name newObj; \
        newObj = malloc(sizeof(_##name##_Struct)); \
        newObj->numItems = 0; \
        newObj->sizeLevel = 0; \
        newObj->numBuckets = _##name##_ValidBucketCounts[0]; \
        newObj->buckets = calloc(newObj->numBuckets, sizeof(_##name##_Node *)); \
        return newObj; \
    } \
    static inline unsigned _HASH(key_type *keyobj, unsigned numBuckets) { \
        /* Jenkins algoritm */ \
        char * key = (char *)keyobj; \
        unsigned hash = 0; \
        unsigned i; \
        for (i = 0; i < sizeof(keyobj); i++) { \
            hash += key[i]; \
            hash += (hash << 10); \
            hash ^= (hash >> 6); \
        } \
        hash += (hash << 3); \
        hash ^= (hash >> 11); \
        hash += (hash << 15); \
        return hash % numBuckets; \
    } \
 \
    static void _##name##_AutoResize(name ht) \
    { \
        _##name##_Struct oldHash; \
        _##name##_Node *pNode; \
        unsigned i; \
 \
        /* Do we need to do anything? */ \
        if (ht->numItems < ht->numBuckets) \
            return; \
 \
        /* Do nothing if we've reached the maximum number of buckets */ \
        if (ht->sizeLevel >= sizeof(_##name##_ValidBucketCounts)/sizeof(_##name##_ValidBucketCounts[0])-1) \
            return; \
 \
        /* Increase the number of buckets */ \
        memcpy(&oldHash, ht, sizeof(_##name##_Struct)); \
        ht->sizeLevel++; \
        ht->numBuckets = _##name##_ValidBucketCounts[ht->sizeLevel]; \
 \
        /* Create larger bucket array */ \
        ht->buckets = calloc(ht->numBuckets, sizeof(_##name##_Node *)); \
 \
        /* Move nodes to new array */ \
        for (i = 0; i < oldHash.numBuckets; i++) \
        { \
            pNode = oldHash.buckets[i]; \
            while (pNode) { \
                /* remove from old bucket */\
                oldHash.buckets[i] = pNode->next; \
 \
                /* rehash */\
                unsigned newhashval = _HASH(&pNode->key, ht->numBuckets); \
                pNode->next = ht->buckets[newhashval]; \
                ht->buckets[newhashval] = pNode; \
 \
                pNode = oldHash.buckets[i]; \
            } \
        } \
    } \
    static inline bool name##_insert(name ht, key_type key, value_type value) { \
        unsigned hashVal = _HASH(&key, ht->numBuckets); \
        _##name##_Node * pNode, *newNode; \
 \
        pNode = ht->buckets[hashVal]; \
        newNode = malloc(sizeof(_##name##_Node)); \
        newNode->next = pNode; \
        newNode->key = key; \
        newNode->value = value; \
 \
        while (pNode) { \
            if (!memcmp(&pNode->key, &key, sizeof(key))) \
                return false; \
            pNode = pNode->next; \
        }; \
 \
        ht->buckets[hashVal] = newNode; \
        ht->numItems++; \
 \
        _##name##_AutoResize(ht); \
    }\
    static inline bool name##_get(name ht, key_type key, value_type *out) { \
        unsigned hashval = _HASH(&key, ht->numBuckets) ; \
        _##name##_Node * pNode = ht->buckets[hashval]; \
        while (pNode) { \
            if (!memcmp(&pNode->key, &key, sizeof(key))) { \
                memcpy(out, &pNode->value, sizeof(pNode->value)); \
                return true; \
            } \
            pNode = pNode->next; \
        } \
        return false; \
    } \
    static inline bool name##_has_key(name ht, key_type key) { \
        unsigned hashval = _HASH(&key, ht->numBuckets) ; \
        _##name##_Node * pNode = ht->buckets[hashval]; \
        while (pNode) { \
            if (!memcmp(&pNode->key, &key, sizeof(key))) \
                return true; \
            pNode = pNode->next; \
        } \
        return false; \
    } \
    static inline unsigned name##_num_items(name ht) { \
        return ht->numItems; \
    } \
    static inline bool name##_is_empty(name ht) { \
        return (ht->numItems == 0); \
    }

#ifdef __cplusplus
}
#endif

#endif
