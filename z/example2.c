#include "zarray.h"
#include <stdio.h>

int main(void)
{
    int *numbers = (int *)ZARRAY_ALLOC(sizeof(int), 1);
    int t;

    printf("%d\n", ZARRAY_NUM_ITEMS(numbers));
    numbers[0] = 4;

    ZARRAY_GROW_BY_ONE(numbers);
    ZARRAY_APPEND(numbers, 5);
    ZARRAY_APPEND(numbers, 6);
    ZARRAY_APPEND(numbers, 7);
    ZARRAY_APPEND(numbers, 8);
    ZARRAY_APPEND(numbers, 9);
    ZARRAY_APPEND(numbers, 10);
    ZARRAY_APPEND(numbers, 11);

    printf("%d\n", ZARRAY_NUM_ITEMS(numbers));
    ZARRAY_POP(numbers, t);
    printf("%d %d\n", t, ZARRAY_NUM_ITEMS(numbers));
    ZARRAY_POP(numbers, t);
    printf("%d %d\n", t, ZARRAY_NUM_ITEMS(numbers));
    ZARRAY_POP(numbers, t);
    printf("%d %d\n", t, ZARRAY_NUM_ITEMS(numbers));
    ZARRAY_POP(numbers, t);
    printf("%d %d\n", t, ZARRAY_NUM_ITEMS(numbers));

    ZARRAY_FREE(numbers);

    return 0;
}
