#ifndef ZTEST_INCLUDED
#define ZTEST_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct _ZTestStruct
{
    char * testName;
    unsigned failCnt;
    unsigned curSubTest;
};

typedef struct _ZTestStruct * ZTest;

static ZTest ZTEST_BEGIN(const char *testname) 
{
    ZTest test;
    test = calloc(1, sizeof(struct _ZTestStruct));
    test->curSubTest = 0;
    test->failCnt = 0;
    test->testName = strndup(testname, 1024);
    return test;
}

#define ZTEST_VERIFY(ztest, subTestName, passCondition) \
    do { \
        if (!(passCondition)) \
        { \
            (ztest)->failCnt++; \
            fprintf(stderr, "%s: Subtest %d \'%s\' failed!\n",  \
                (ztest)->testName,  \
                (ztest)->curSubTest, \
                (subTestName)); \
            fflush(stderr); \
        } \
        (ztest)->curSubTest++; \
    } while (0)

static int ZTEST_END(ZTest ztest)
{
    bool passed = !(ztest->failCnt);

    if (passed)
    {
        fprintf(stderr, "%s PASSED.  %d subtests.\n", 
            (ztest)->testName, 
            (ztest)->curSubTest);
    }
    else
    {
        fprintf(stderr, "%s FAILED! %d of %d subtests pass. %d failed.\n",
            (ztest)->testName,
            (ztest)->curSubTest - (ztest)->failCnt,
            (ztest)->curSubTest,
            (ztest)->failCnt);
    }

    free(ztest->testName);
    free(ztest);

    return passed ? 0 : 1;
}

#endif
