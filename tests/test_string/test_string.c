#include "red_string.h"
#include "red_test.h"

#include <stdio.h>

int main(int argc, const char *argv[])
{
    RedTest suite = RedTest_Begin(argv[0], NULL, NULL);

    /* Basic creation & destruction */
    {
        RedString s;
        s = RedString_New(NULL);
        RedTest_Verify(suite, "Create empty string using NULL", (s != NULL));
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);
        RedTest_Verify(suite, "RedString_Free (doesn't crash)", true);

        s = RedString_New("");
        RedTest_Verify(suite, "Create empty string", (s != NULL));
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);

        s = RedString_New("Hello");
        RedTest_Verify(suite, "Create empty string, initialized from chars", (s != NULL));
        RedTest_Verify(suite, "Length is 5", (RedString_Length(s) == 5));
        RedString_Free(s);
    }

    /* Test RedString_NewLength */
    {
        RedString s;
        s = RedString_NewLength(NULL, 0);
        RedTest_Verify(suite, "RedString_NewLength(NULL, 0)", (s != NULL));
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);

        s = RedString_NewLength(NULL, 100);
        RedTest_Verify(suite, "RedString_NewLength(NULL, 100)", (s != NULL));
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);

        s = RedString_NewLength("HOWDY", 0);
        RedTest_Verify(suite, "RedString_NewLength(HOWDY, 0)", (s != NULL));
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);

        s = RedString_NewLength("HOWDY", 1);
        RedTest_Verify(suite, "RedString_NewLength(HOWDY, 1)", (s != NULL));
        RedTest_Verify(suite, "Length is 1", (RedString_Length(s) == 1));
        RedString_Free(s);

        s = RedString_NewLength("HOWDY", 2);
        RedTest_Verify(suite, "RedString_NewLength(HOWDY, 2)", (s != NULL));
        RedTest_Verify(suite, "Length is 2", (RedString_Length(s) == 2));
        RedString_Free(s);

        s = RedString_NewLength("HOWDY", 5);
        RedTest_Verify(suite, "RedString_NewLength(HOWDY, 5)", (s != NULL));
        RedTest_Verify(suite, "Length is 5", (RedString_Length(s) == 5));
        RedString_Free(s);

        s = RedString_NewLength("HOWDY", 100);
        RedTest_Verify(suite, "RedString_NewLength(HOWDY, 100)", (s != NULL));
        RedTest_Verify(suite, "Length is 5", (RedString_Length(s) == 5));
        RedString_Free(s);
    }

    /* Test RedString_NewPrintf */
    {
        RedString s;
        s = RedString_NewPrintf("Hello", 0);
        RedTest_Verify(suite, "RedString_NewPrintf(\"Hello\", 0)", (s != NULL));
        RedTest_Verify(suite, "Hello Length is 0", (RedString_Length(s) == 0));
        RedString_Free(s);

        s = RedString_NewPrintf("Hello", 4);
        RedTest_Verify(suite, "RedString_NewPrintf(\"Hello\", 4)", (s != NULL));
        RedTest_Verify(suite, "Hello Length is 4", (RedString_Length(s) == 4));
        RedString_Free(s);

        s = RedString_NewPrintf("Hello", 18);
        RedTest_Verify(suite, "RedString_NewPrintf(\"Hello\", 18)", (s != NULL));
        RedTest_Verify(suite, "Hello Length is 5", (RedString_Length(s) == 5));
        RedString_Free(s);

        s = RedString_NewPrintf("My name is %s %s", 200, "slim", "shadey");
        RedTest_Verify(suite, "RedString_NewPrintf - slim shadey", (s != NULL));
        RedTest_Verify(suite, "Hello Length is 22", (RedString_Length(s) == 22));
        RedString_Free(s);
    }

    /* Test RedString_Set and RedString_Clear*/
    {
        RedString s;
        bool match;
        s = RedString_NewPrintf("%1.1f", 100, 2.5f);
        RedTest_Verify(suite, "New string not null", (s != NULL));
        RedTest_Verify(suite, "2.5 Length is 3", (RedString_Length(s) == 3));
        match = !RedString_CompareChars(RedString_GetChars(s), "2.5", 0);
        RedTest_Verify(suite, "Content is \"2.5\"", match);

        RedString_Set(s, NULL);
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        match = !RedString_CompareChars(RedString_GetChars(s), "", 0);
        RedTest_Verify(suite, "Content is \"\"", match);

        RedString_Set(s, "i am thor");
        RedTest_Verify(suite, "Length is 9", (RedString_Length(s) == 9));
        match = !RedString_CompareChars(RedString_GetChars(s), "i am thor", 0);
        RedTest_Verify(suite, "Content is \"i am thor\"", match);

        RedString_Set(s, "");
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        match = !RedString_CompareChars(RedString_GetChars(s), "", 0);
        RedTest_Verify(suite, "Content is \"\"", match);

        RedString_Set(s, "0123456789abcdef");
        RedTest_Verify(suite, "Length is 16", (RedString_Length(s) == 16));
        match = !RedString_CompareChars(RedString_GetChars(s), "0123456789abcdef", 0);
        RedTest_Verify(suite, "Content is \"\"", match);

        RedString_Clear(s);
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s) == 0));
        match = !RedString_CompareChars(RedString_GetChars(s), "", 0);
        RedTest_Verify(suite, "Content is \"\"", match);

        RedString_Free(s);
    }

    /* Test RedString_Copy */

    return RedTest_End(suite);
}
