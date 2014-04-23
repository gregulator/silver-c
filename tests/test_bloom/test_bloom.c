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
#include "red_bloom.h"
#include "red_test.h"

#include <math.h>
#include <assert.h>

/*
 * _GenStringValue -- Helper routine to generate a string value, based on i.
 *
 *  i   |   value
 *  ---------------
 *  0   |  "RED - 0"
 *  1   |  "ORANGE - 1"
 *  2   |  "YELLOW - 2"
 *  3   |  "GREEN - 3"
 *  4   |  "BLUE - 4"
 *  5   |  "INDIGO - 5"
 *  6   |  "VIOLET - 6"
 *  7   |  "RED - 7"
 *  8   |  "ORANGE - 8"
 *  ...
 *
 */
static void _GenStringValue(char *dest, int maxLength, uint32_t i)
{
    static const char *colors[] = {
        "RED",
        "ORANGE",
        "YELLOW",
        "GREEN",
        "BLUE",
        "INDIGO",
        "VIOLET"
    };
    snprintf(dest, maxLength, "%s - %d", colors[i%7], i);
}

/*
 * _GenDoubleValue -- Helper routine to generate a double value, based on i.
 * Returns (2*(i%2)-1)*(i/13.0f)
 */
static double _GenDoubleValue(uint32_t i)
{
    double out;
    out = (2*((int)i%2)-1)*(i/13.0f);
    return out;
}

typedef enum
{
    TEST_BLOOM_DOUBLE_VALUE,
    TEST_BLOOM_STRING_VALUE
} TestBloomValueType;

/*
 * This routine:
 *  - Creates a bloom filter using <estimatedNumItems> and <falsePositiveRate>.
 *  - Adds <numItemsToInsert> into it.
 *  - Verifies that there are no false negatives.
 *  - Tests <numItemsToTest> values to determine false positive rate.
 *  - Verifies that the number of false positives is between
 *    <minFalsePositivesAllowed> and <maxFalsePositivesAllowed>
 *  - Uses RedBloom_InsertS or RedBloom_Insert depending on <valueType>.
 */
static void TestFalsePosNeg(
        RedTest suite, 
        uint32_t estimatedNumItems, 
        double falsePositiveRate,
        uint32_t numItemsToInsert,
        uint32_t numItemsToTest,
        uint32_t minFalsePositivesAllowed,
        uint32_t maxFalsePositivesAllowed,
        TestBloomValueType valueType)
{
    char subtestName[1024];
    char value[128];
    unsigned numFalseNegatives = 0;
    unsigned numFalsePositives = 0;
    uint32_t i;

    /* Create bloom filter */
    RedBloom bloom = RedBloom_New(estimatedNumItems, falsePositiveRate);
    snprintf(subtestName, 
            1024, 
            "TestFalsePosNeg(%d, %f) - Create bloom filter",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (bloom != NULL));

    /* Insert items. */
    for (i = 0; i < numItemsToInsert; i++)
    {
        if (valueType == TEST_BLOOM_STRING_VALUE)
        {
            _GenStringValue(value, 128, i);
            RedBloom_InsertS(bloom, value);
        }
        else
        {
            assert(valueType == TEST_BLOOM_DOUBLE_VALUE);
            double val = _GenDoubleValue(i);
            RedBloom_Insert(bloom, &val, sizeof(double));
        }
    }

    /* Count false negatives */
    numFalseNegatives = 0;
    for (i = 0; i < numItemsToInsert; i++)
    {
        if (valueType == TEST_BLOOM_STRING_VALUE)
        {
            _GenStringValue(value, 128, i);
            if (!RedBloom_MayContainS(bloom, value))
                numFalseNegatives++;
        }
        else
        {
            assert(valueType == TEST_BLOOM_DOUBLE_VALUE);
            double val = _GenDoubleValue(i);
            if (!RedBloom_MayContain(bloom, &val, sizeof(double)))
                numFalseNegatives++;
        }
    }
    snprintf(subtestName, 
            1024, 
            "TestFalsePosNeg(%d, %f) - No false negatives",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (numFalseNegatives == 0));

    /* Count false positives */
    numFalsePositives = 0;
    for (i = 0; i < numItemsToTest; i++)
    {
        /* For false positive testing, switch up the "valueType" */
        if (i % 2)
        {
            _GenStringValue(value, 128, numItemsToInsert + i);
            if (RedBloom_MayContainS(bloom, value))
                numFalsePositives++;
        }
        else
        {
            double val = _GenDoubleValue(numItemsToInsert + i);
            if (RedBloom_MayContain(bloom, &val, sizeof(double)))
                numFalsePositives++;
        }
    }
    snprintf(subtestName, 
            1024, 
            "TestFalsePosNeg(%d, %f) - Too many false positives",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (numFalsePositives <= maxFalsePositivesAllowed));
    snprintf(subtestName, 
            1024, 
            "TestFalsePosNeg(%d, %f) - Too few false positives",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (numFalsePositives >= minFalsePositivesAllowed));

    /* Cleanup */
    RedBloom_Free(bloom);
}

static void TestFalsePosNeg_Normal(
        RedTest suite,
        uint32_t estimatedNumItems, 
        double falsePositiveRate,
        TestBloomValueType valueType)
{
    TestFalsePosNeg(
        suite,
        estimatedNumItems,
        falsePositiveRate,
        estimatedNumItems,
        10000,
        /* With fewer than ~32 estimated items, the false positive rate may be
         * lower than expected due to rounding up to byte-lengthed bloom filter
         * size.  So we don't test minFalsePositivesAllowed with small-sized
         * filters.
         */
        (estimatedNumItems > 32) ? floor(5000*falsePositiveRate) : 0,
        ceil(20000*falsePositiveRate),
        valueType);
}

static void TestFalsePosNeg_Empty(
        RedTest suite,
        uint32_t estimatedNumItems, 
        double falsePositiveRate,
        TestBloomValueType valueType)
{
    TestFalsePosNeg(
        suite,
        estimatedNumItems,
        falsePositiveRate,
        0,
        1000,
        0,
        0,
        valueType);
}

static void TestFalsePosNeg_Saturated(
        RedTest suite,
        uint32_t estimatedNumItems, 
        double falsePositiveRate,
        TestBloomValueType valueType)
{
    TestFalsePosNeg(
        suite,
        estimatedNumItems,
        falsePositiveRate,
        estimatedNumItems*10,
        1000,
        950,
        1000,
        valueType);
}

int main(int argc, const char *argv[])
{
    RedTest suite = RedTest_Begin(argv[0], NULL, NULL);

    /* Test creation & destruction */
    {
        RedBloom bloom = RedBloom_New(100, 0.01);
        RedTest_Verify(suite, "Create bloom filter", (bloom != NULL));
        RedBloom_Free(bloom);
        RedTest_Verify(suite, "Free bloom filter (didn't crash)", true);
    }

    /*
     * Test false negative and positives counts under normal usage, where
     *      (# of items inserted) == (estimated # of items).
     *  The actual false positive rate must be within 50%-200% of the expected
     *  value to pass.  There should be no false negatives.
     */
    TestFalsePosNeg_Normal(suite, 1, .1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 10, 0.75, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 10, 0.001, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 100, 0.0001, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 1000, 0.1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 1000, 0.005, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 10000, 0.1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 10000, 0.005, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 100000, 0.1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 100000, 0.005, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 100000, 0.0001, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 1000000, 0.3, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 1000000, 0.01, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 1000000, 0.0001, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Normal(suite, 1000000, 0.00001, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Normal(suite, 10000000, 0.01, TEST_BLOOM_DOUBLE_VALUE);

    /*
     * Test false positive and negative counts with various empty bloom
     * filters.  There should be no false positives or negatives.
     */
    TestFalsePosNeg_Empty(suite, 1, .1, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Empty(suite, 10, 0.75, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Empty(suite, 100, 0.0001, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Empty(suite, 1000, 0.005, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Empty(suite, 10000, 0.005, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Empty(suite, 100000, 0.1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Empty(suite, 1000000, 0.3, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Empty(suite, 10000000, 0.01, TEST_BLOOM_STRING_VALUE);

    /*
     * Test false positive and negative counts with highly saturated bloom
     * filters.  There should be close to 100% false positives and no false
     * negatives.
     */
    TestFalsePosNeg_Saturated(suite, 1, .1, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Saturated(suite, 10, 0.75, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Saturated(suite, 100, 0.0001, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Saturated(suite, 1000, 0.005, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Saturated(suite, 10000, 0.005, TEST_BLOOM_STRING_VALUE);
    TestFalsePosNeg_Saturated(suite, 100000, 0.1, TEST_BLOOM_DOUBLE_VALUE);
    TestFalsePosNeg_Saturated(suite, 1000000, 0.3, TEST_BLOOM_STRING_VALUE);

    return RedTest_End(suite);
}
