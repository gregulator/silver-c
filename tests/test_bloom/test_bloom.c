#include "red_bloom.h"
#include "red_test.h"

#include <math.h>
/*
 * _GenValue -- Helper routine gGenerate a string value, based on i.
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
 * Caller must free the returned string.
 */
static void _GenValue(char *dest, int maxLength, uint32_t i)
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
 * This routine:
 *  - Creates a bloom filter
 *  - Adds <numItemsToInsert> into it.
 *  - Verifies that there are no false negatives.
 *  - Verifies that the number of false positives is on the order of
 *    <falsePositiveRate>*<numItemsToInsert> (within 2X).
 */
static void TestFalsePositiveRate(
        RedTest suite, 
        uint32_t numItemsToInsert, 
        double falsePositiveRate)
{
    char subtestName[1024];
    char value[128];
    unsigned numFalseNegatives = 0;
    unsigned numFalsePositives = 0;
    uint32_t i;

    /* Create bloom filter */
    RedBloom bloom = RedBloom_New(numItemsToInsert, falsePositiveRate);
    snprintf(subtestName, 
            1024, 
            "TestFalsePositiveRate(%d, %f) - Create bloom filter",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (bloom != NULL));

    /* Insert items. */
    for (i = 0; i < numItemsToInsert; i++)
    {
        _GenValue(value, 128, i);
        RedBloom_InsertS(bloom, value);
    }

    /* Count false negatives */
    numFalseNegatives = 0;
    for (i = 0; i < numItemsToInsert; i++)
    {
        _GenValue(value, 128, i);
        if (!RedBloom_MayContainS(bloom, value))
            numFalseNegatives++;
    }
    snprintf(subtestName, 
            1024, 
            "TestFalsePositiveRate(%d, %f) - No false negatives",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (numFalseNegatives == 0));

    /* Count false positives */
    numFalsePositives = 0;
    for (i = 0; i < numItemsToInsert; i++)
    {
        _GenValue(value, 128, numItemsToInsert + i);
        if (RedBloom_MayContainS(bloom, value))
            numFalsePositives++;
    }
    snprintf(subtestName, 
            1024, 
            "TestFalsePositiveRate(%d, %f) - Not too many false positives",
            numItemsToInsert, 
            falsePositiveRate);
    fprintf(stderr, "%d %f\n", numFalsePositives, 2*falsePositiveRate*numItemsToInsert);
    RedTest_Verify(suite, subtestName, (numFalsePositives <= ceilf(2.0*falsePositiveRate*numItemsToInsert)));
    snprintf(subtestName, 
            1024, 
            "TestFalsePositiveRate(%d, %f) - Not too few false positives",
            numItemsToInsert, 
            falsePositiveRate);
    RedTest_Verify(suite, subtestName, (numFalsePositives >= floorf(.5*falsePositiveRate*numItemsToInsert)));

    /* Cleanup */
    RedBloom_Free(bloom);
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

    TestFalsePositiveRate(suite, 1, .1); /* expect 0 false positives */
    TestFalsePositiveRate(suite, 10, 0.75); /* expect 7.5 false positives */
    TestFalsePositiveRate(suite, 10, 0.001); /* expect 0 false positives */
    TestFalsePositiveRate(suite, 100, 0.0001); /* expect 0 false positives */
    TestFalsePositiveRate(suite, 1000, 0.1); /* expect 100 false positives, allow up to 200 */
    TestFalsePositiveRate(suite, 1000, 0.005); /* expect 5 false positives, allow up to 10 */
    TestFalsePositiveRate(suite, 10000, 0.1); /* expect 1000 false positives, allow up to 2000 */
    TestFalsePositiveRate(suite, 10000, 0.005); /* expect 50 false positives, allow up to 100 */
    TestFalsePositiveRate(suite, 100000, 0.1); /* expect 10000 false positives, allow up to 20000 */
    TestFalsePositiveRate(suite, 100000, 0.005); /* expect 500 false positives, allow up to 1000 */
    TestFalsePositiveRate(suite, 100000, 0.0001); /* expect 10 false positives, allow up to 20 */
    TestFalsePositiveRate(suite, 1000000, 0.3); /* expect 300000 false positives, allow up to 20000 */
    TestFalsePositiveRate(suite, 1000000, 0.01); /* expect 10000 false positives, allow up to 1000 */
    TestFalsePositiveRate(suite, 1000000, 0.0001); /* expect 100 false positives, allow up to 20 */
    TestFalsePositiveRate(suite, 1000000, 0.00001); /* expect 10 false positives, allow up to 20 */

    return RedTest_End(suite);
}
