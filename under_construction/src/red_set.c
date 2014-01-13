#include "red_set.h"

/*
 * TODO: What if iterator allocation fails?
 */

struct RedSet_t
{
    RedHash hash;
};

RedSet RedSet_New()
{
    RedSet set;
    set = malloc(sizeof(struct RedSet_t));
    if (!set)
        return NULL;

    set->hash = RedHash_New();
    if (!set->hash)
    {
        free(set);
        return NULL;
    }

    return set;
}

unsigned RedSet_NumItems(RedSet set)
{
    return RedHash_NumItems(set->hash);
}

bool RedSet_Add(RedSet set, const void *pItem, size_t itemSize)
{
    RedHashResultEnum result;
    result = RedHash_Insert(set->hash, pItem, itemSize, NULL, 0);
    if (result == RED_HASH_SUCCESS || result == RED_HASH_ERROR_DUPLICATE_KEY)
        return true;
    return false;
}

bool RedSet_AddS(RedSet set, const char *pItem)
{
    return RedSet_Add(set, pItem, strlen(pItem));
}

bool RedSet_Remove(RedSet set, const void *pItem, size_t itemSize)
{
    RedHashResultEnum result;
    result = RedHash_Remove(set->hash, pItem, itemSize, NULL, 0);
    return (result == RED_HASH_SUCCESS);
}

bool RedSet_RemoveS(RedSet set, const char *pItem)
{
    return RedSet_Remove(set, pItem, strlen(pItem));
}

bool RedSet_HasItem(RedSet set, const void *pItem, size_t itemSize)
{
    return RedHash_HasKey(set, pItem, itemSize);
}

bool RedSet_HasItemS(RedSet set, const char *pItem)
{
    return RedSet_HasItem(set, pItem, strlen(pItem));
}

bool RedSet_HasItemS(RedSet set, const char *pItem)
{
    return RedSet_HasItem(set, pItem, strlen(pItem));
}

RedSet RedSet_NewUnion(RedSet sourceA, RedSet sourceB)
{
    RedSet target, source;
    RedSetIter_t iter;
    bool result;

    target = RedSet_New();
    if (!target)
        return NULL;

    for (i = 0; i < 2; i++)
    {
        source = i ? sourceA : sourceB;
        RED_SET_FOREACH(source, iter, &item, &itemSize)
        {
            result = RedSet_Insert(target, item, itemSize);
            if (!result)
            {
                RedSet_Free(target);
                return NULL;
            }
        }
    }

    return target;
}

RedSet RedSet_NewIntersection(RedSet sourceA, RedSet sourceB)
{
    RedSet target, source;
    RedSetIter_t iter;
    bool result;

    target = RedSet_New();
    if (!target)
        return NULL;

    RED_SET_FOREACH(source, iter, &item, &itemSize)
    {
        if (RedSet_HasItem(sourceB, item, itemSize))
        {
            result = RedSet_Insert(target, item, itemSize);
            if (!result)
            {
                RedSet_Free(target);
                return NULL;
            }
        }
    }

    return target;
}

RedSet RedSet_NewDifference(RedSet sourceA, RedSet sourceB)
{
    RedSet target, source;
    RedHashIter iter;
    bool result;

    target = RedSet_New();
    if (!target)
        return NULL;

    RED_SET_FOREACH(sourceA, iter, &item, &itemSize)
    {
        if (!RedSet_HasItem(sourceB, item, itemSize))
        {
            result = RedSet_Insert(target, item, itemSize);
            if (!result)
            {
                RedSet_Free(target);
                return NULL;
            }
        }
    }

    return target;
}

RedSet RedSet_NewSymmetricDifference(RedSet sourceA, RedSet sourceB)
{
    RedSet target, source;
    RedHashIter iter;
    bool result;

    target = RedSet_New();
    if (!target)
        return NULL;

    for (i = 0; i < 2; i++)
    {
        source = i ? sourceA : sourceB;
        otherSource = i ? sourceB : sourceA;
        RED_SET_FOREACH(sourceA, iter, &item, &itemSize)
        {
            if (!RedSet_HasItem(otherSource, item, itemSize))
            {
                result = RedSet_Insert(target, item, itemSize);
                if (!result)
                {
                    RedSet_Free(target);
                    return NULL;
                }
            }
        }
    }

    return target;
}

RedSet RedSet_Free(RedSet set)
{
    if (set)
    {
        RedSet_Clear(set);
        RedHash_Free(set->hash);
        free(set);
    }

}

bool RedSet_IsEqual(RedSet setA, RedSet setB)
{
    RedHashIter_t iter;
    const void *key;
    size_t keySize;

    RED_SET_FOREACH(setA, iter, &key, &keySize)
        if (!RedSet_HasItem(setB, key, keySize))
            return false;

    RED_SET_FOREACH(setB, iter, &key, &keySize)
        if (RedSet_HasItem(setA, key, keySize))
            return false;

    return true;
}

bool RedSet_IsSubset(RedSet setA, RedSet setB)
{
    RedSetIter_t iter;
    const void *item;
    size_t itemSize;

    RED_SET_FOREACH(setA, iter, &item, &itemSize)
        if (!RedSet_HasItem(setB, item, itemSize))
            return false;
    }
    return true;
}

bool RedSet_IsSuperset(RedSet setA, RedSet setB)
{
    RedSetIter_t iter;
    const void *key;
    size_t keySize;

    RED_SET_FOREACH(setB, iter, &item, &itemSize)
        if (!RedSet_HasItem(setA, item, itemSize))
            return false;
    }
    return true;
}

void RedSetIter_Init(RedSetIter_t *pIter, RedSet set)
{
    RedHashIter_Init(&pIter->_hashIter, set->hash);
}

bool RedSetIter_Advance(RedSetIter_t *pIter, const void **pOutItem, size_t *pOutSize)
{
    return RedHashIter_Advance(&pIter->_hashIter, pOutItem, pOutSize, NULL, NULL);
}
