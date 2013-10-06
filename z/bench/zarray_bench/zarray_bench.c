#include <zarray.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
    int *vals = ZARRAY_ALLOC(sizeof(int), 0);
    bool good = true;
    int i;
    for (i = 0; i < 10000000; i++)
        ZARRAY_APPEND(vals, i);
    for (i = 0; i < 10000000; i++)
        if (vals[i] != i)
            good = false;
    for (i = 0; i < 10000000; i++)
        ZARRAY_SHRINK_BY_ONE(vals);

    if (!good)
        printf("failed\n");

    return 0;
}
