/*  
 *  test_bloom.c -- Unit tests for "RedBloom" bloom filter module.
 *
 *  Author: Gregory Prsiament (greg@toruslabs.com)
 *
 *  ===========================================================================
 *  Creative Commons CC0 1.0 Universal - Public Domain 
 *
 *  To the extent possible under law, Gregory Prisament has waived all
 *  copyright and related or neighboring rights to RedTest. This work is
 *  published from: United States. 
 *
 *  For details please refer to either:
 *      - http://creativecommons.org/publicdomain/zero/1.0/legalcode
 *      - The LICENSE file in this directory, if present.
 *  ===========================================================================
 */
#include "red_json.h"
#include "red_test.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char *argv[])
{
    RedTest suite = RedTest_Begin(argv[0], NULL, NULL);

    {
        //RedJsonArray array;
        RedJsonObject obj;
        RedJsonValue val;
        char *out;

        //array = RedJsonArray_New();
        //RedJsonArray_AppendNumber(array, 1);
        //RedJsonArray_AppendString(array, "Two");
        //RedJsonArray_AppendNumber(array, 3.5);

        obj = RedJsonObject_New();
        RedJsonObject_SetString(obj, "Moo", "Cow");
        RedJsonObject_SetString(obj, "AArg", "Tree");
        RedJsonObject_SetString(obj, "Fat", "Cat");
        RedJsonObject_SetBoolean(obj, "Awesome", true);
        RedJsonObject_SetNumber(obj, "Pi", 3.14159);
        //RedJsonObject_SetArray(obj, "Numbers", array);

        val = RedJsonValue_FromObject(obj);

        out = RedJsonValue_ToJsonString(val);
        printf("%s\n", out);
    }

    return RedTest_End(suite);

}
