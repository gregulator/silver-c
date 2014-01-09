#include "zhash.h"
#include "ztest.h"
#include "stdbool.h"

typedef struct
{
    double x, y, z, r;
} Sphere;

GENERATE_ZHASH_FIXED_KEY_IMPLEMENTATION(HashSphereToBool, Sphere, bool)

int main(void)
{
    HashSphereToBool h;
    Sphere s = {1.0, 2.0, 0.5, 0.25}, s2 = {1.0, 2.0, 0.5, 0.25};
    ZTest test = ZTEST_BEGIN("zhash test");
    h = HashSphereToBool_new();
    ZTEST_VERIFY(test, "HASH_new", h != NULL);
    ZTEST_VERIFY(test, "new hash has 0 items", HashSphereToBool_num_items(h) == 0);
    ZTEST_VERIFY(test, "new hash is empty", HashSphereToBool_is_empty(h) == true);

    ZTEST_VERIFY(test, "new hash doesnt have sphere", HashSphereToBool_has_key(h, s) == false);

    // insert sphere
    HashSphereToBool_insert(h, s, true);
    ZTEST_VERIFY(test, "insert", HashSphereToBool_has_key(h, s) == true);
    ZTEST_VERIFY(test, "insert", HashSphereToBool_has_key(h, s2) == true);

    bool rval, val;
    rval = HashSphereToBool_get(h, s, &val);
    ZTEST_VERIFY(test, "get return val", rval == true);
    ZTEST_VERIFY(test, "get out val", val == true);

    ZTEST_END(test);

    return HashSphereToBool_num_items(h);
}
