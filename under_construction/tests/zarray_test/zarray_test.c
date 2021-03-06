#include <ztest.h>
#include <zarray.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    float x, y, z;
    float radius;
} Sphere;

int main(int argc, const char *argv[])
{
    ZTest zt = ZTEST_BEGIN(argv[0]);
    int i;
    ZARRAY(long double) numbers;
    long double val;
    ZARRAY(Sphere) spheres;
    ZARRAY(char) str0;
    ZARRAY(char) str1;
    int numGood;

    /* Allocate and test 0-size array */
    numbers = ZARRAY_NEW(long double, 0);
    ZTEST_VERIFY(zt, "0-element array allocation", numbers != NULL);
    ZTEST_VERIFY(zt, "0-element array num items", ZARRAY_NUM_ITEMS(numbers) == 0);
    ZARRAY_FREE(numbers);
    ZTEST_VERIFY(zt, "0-element array free doesn't crash", true);

    /* Another 0-size array */
    numbers = ZARRAY_NEW(long double, 0);
    ZTEST_VERIFY(zt, "0-element array allocation", numbers != NULL);
    ZTEST_VERIFY(zt, "0-element array num items", ZARRAY_NUM_ITEMS(numbers) == 0);

    ZARRAY_APPEND(numbers, 50.0);
    ZTEST_VERIFY(zt, "num items correct after append", ZARRAY_NUM_ITEMS(numbers) == 1);
    ZTEST_VERIFY(zt, "value correct after append", numbers->item[0] == 50.0);
    ZTEST_VERIFY(zt, "tail value correct after append", ZARRAY_TAIL(numbers) == 50.0);
    ZARRAY_FREE(numbers);
    ZTEST_VERIFY(zt, "1-element array free doesn't crash", true);

    /* Another 0-size array, appended with values, verify, then pop all values */
    numbers = ZARRAY_NEW(long double, 0);
    ZTEST_VERIFY(zt, "0-element array allocation", numbers != NULL);
    ZTEST_VERIFY(zt, "0-element array num items", ZARRAY_NUM_ITEMS(numbers) == 0);

    for (i = 0; i < 100; i++)
    {
        ZARRAY_APPEND(numbers, i*4.0);
        ZTEST_VERIFY(zt, "num items correct after append", ZARRAY_NUM_ITEMS(numbers) == (unsigned)i+1);
        ZTEST_VERIFY(zt, "value correct after append", numbers->item[i] == i*4.0);
        ZTEST_VERIFY(zt, "tail value correct after append", ZARRAY_TAIL(numbers) == i*4.0);
    }
    for (i = 0; i < 100; i++)
        ZTEST_VERIFY(zt, "All values correct", numbers->item[i] == i*4.0);

    for (i--; i >= 0; i--)
    {
        val = ZARRAY_POP(numbers);
        ZTEST_VERIFY(zt, "num items correct after pop", ZARRAY_NUM_ITEMS(numbers) == (unsigned)i);
        ZTEST_VERIFY(zt, "value correct after pop", val == i*4.0);
        if (i > 0)
            ZTEST_VERIFY(zt, "tail value correct after pop", ZARRAY_TAIL(numbers) == (i-1)*4.0);
    }
    ZARRAY_FREE(numbers);

    /* Allocate array of 100 Sphere structures */
    spheres = ZARRAY_NEW(Sphere, 100);
    ZTEST_VERIFY(zt, "100-struct num items", ZARRAY_NUM_ITEMS(spheres) == 100);
    for (i = 0; i < (signed)ZARRAY_NUM_ITEMS(spheres); i++)
    {
        ZARRAY_AT(spheres, i).x = 0.5*i;
        ZARRAY_AT(spheres, i).y = 0.25*i;
        ZARRAY_AT(spheres, i).z = 0.125*i;
        ZARRAY_AT(spheres, i).radius = 5*i;
    }
    while (ZARRAY_NUM_ITEMS(spheres) > 0)
    {
        Sphere sphere;
        i--;
        sphere = ZARRAY_POP(spheres);
        ZTEST_VERIFY(zt, "spheres[i].x correct", sphere.x == 0.5*i);
        ZTEST_VERIFY(zt, "spheres[i].y correct", sphere.y == 0.25*i);
        ZTEST_VERIFY(zt, "spheres[i].z correct", sphere.z == 0.125*i);
        ZTEST_VERIFY(zt, "spheres[i].r correct", sphere.radius == 5*i);
        ZTEST_VERIFY(zt, "num items correct", ZARRAY_NUM_ITEMS(spheres) == (unsigned)i);
    }
    ZTEST_VERIFY(zt, "100 items popped", i == 0);
    ZARRAY_FREE(spheres);

    /* Resize big arrays of chars */
    str0 = ZARRAY_NEW(char, 0);
    ZTEST_VERIFY(zt, "0-element array allocation", str0 != NULL);
    ZTEST_VERIFY(zt, "0-element array num items", ZARRAY_NUM_ITEMS(str0) == 0);
    #define TEN_MILLION 10000000
    #define ONE_MILLION 1000000
    #define ONE_THOUSAND 1000
    ZARRAY_RESIZE(str0, TEN_MILLION);
    ZTEST_VERIFY(zt, "Resize to 10mil items", ZARRAY_NUM_ITEMS(str0) == TEN_MILLION);
    for (i = 0; i < TEN_MILLION; i++)
        str0->item[i] = 'Z';

    ZARRAY_RESIZE(str0, ONE_MILLION);
    ZTEST_VERIFY(zt, "Resize to 1mil items", ZARRAY_NUM_ITEMS(str0) == ONE_MILLION);
    
    numGood = 0;
    for (i = 0; i < ONE_MILLION; i++)
        if (str0->item[i] == 'Z')
            numGood++;
    ZTEST_VERIFY(zt, "After resize shrink, data unaffected", numGood == ONE_MILLION);


    /* Grow big array of chars */
    str1 = ZARRAY_NEW(char, 0);
    for (i = 0; i < ONE_MILLION; i++)
        ZARRAY_GROW_BY_ONE(str1);
    ZTEST_VERIFY(zt, "GROW BY ONE to 1mil items", ZARRAY_NUM_ITEMS(str1) == ONE_MILLION);
    for (i = 0; i < ONE_MILLION; i++)
        str1->item[i] = 'Z';
    ZTEST_VERIFY(zt, "str1 and str0 both have 1 mil Zs", !strncmp(str0->item, str1->item, ONE_MILLION));

    for (i = 0; i < 9; i++)
        ZARRAY_GROW(str1, ONE_MILLION);
    ZTEST_VERIFY(zt, "GROW to 10mil items", ZARRAY_NUM_ITEMS(str1) == TEN_MILLION);
    ZTEST_VERIFY(zt, "str1 and str0 still both have 1 mil Zs", !strncmp(str0->item, str1->item, ONE_MILLION));

    /* Shrink char arrays */
    while (ZARRAY_NUM_ITEMS(str0) > ONE_THOUSAND)
        ZARRAY_SHRINK_BY_ONE(str0);
    ZTEST_VERIFY(zt, "str0 shrink-by-one to 1000 items", ZARRAY_NUM_ITEMS(str0) == ONE_THOUSAND);

    for (i = 0; i < 9; i++)
        ZARRAY_SHRINK(str1, ONE_MILLION);
    for (i = 0; i < 9; i++)
        ZARRAY_SHRINK(str1, 100*ONE_THOUSAND);
    ZARRAY_SHRINK(str1, 99*ONE_THOUSAND);
    ZTEST_VERIFY(zt, "str1 shrink-by-many to 1000 items", ZARRAY_NUM_ITEMS(str1) == ONE_THOUSAND);

    ZTEST_VERIFY(zt, "str1 and str0 now both have 1 mil Zs", !strncmp(str0->item, str1->item, ONE_THOUSAND));

    return ZTEST_END(zt);
}
