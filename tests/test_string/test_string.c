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
    {
        RedString s0, s1;
        bool match;

        s0 = RedString_New("Red Fish");
        s1 = RedString_New("Blue Fish");
        RedString_Copy(s0, s0);
        match = !RedString_CompareChars(RedString_GetChars(s0), "Red Fish", 0);
        RedTest_Verify(suite, "Red Fish Length is 8", (RedString_Length(s0) == 8));
        RedTest_Verify(suite, "Content is \"Red Fish\"", match);

        RedString_Copy(s0, s1);
        match = !RedString_CompareChars(RedString_GetChars(s0), "Blue Fish", 0);
        RedTest_Verify(suite, "Blue Fish Length is 9", (RedString_Length(s0) == 9));
        RedTest_Verify(suite, "Content is \"Blue Fish\"", match);

        RedString_Clear(s0);
        match = !RedString_CompareChars(RedString_GetChars(s1), "Blue Fish", 0);
        RedTest_Verify(suite, "Blue Fish Length is 9", (RedString_Length(s1) == 9));
        RedTest_Verify(suite, "Content is \"Blue Fish\"", match);
        RedString_Copy(s1, s0);
        RedTest_Verify(suite, "Length is 0", (RedString_Length(s1) == 0));
        RedTest_Verify(suite, "Content is \"\"", match);

        RedString_Free(s0);
        RedString_Free(s1);
    }

    /* Test RedString_Compare */
    /* Test RedString_CompareChars */
    /* Test RedString_ContainsChars */
    /* Test RedString_StartsWith */
    /* Test RedString_Search */
    /* Test RedString_Trim */
    /* Test RedString_RemoveFloat */
    /* Test RedString_FirstNonWhitespaceChar */
    /* Test RedString_SubString */
    /* Test RedString_Reverse */
    /* Test RedString_CaseConvert */
    
    /* Test RedString_Append, AppendChars, AppendPrintf */
    {
        RedString s0, s1;
        bool match;

        s0 = RedString_New("MON");
        s1 = RedString_New(" TUE");
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON", 0);
        RedTest_Verify(suite, "MON Length is 3", (RedString_Length(s0) == 3));
        RedTest_Verify(suite, "Content is \"MON\"", match);

        RedString_Append(s0, s1);
        match = !RedString_CompareChars(RedString_GetChars(s1), " TUE", 0);
        RedTest_Verify(suite, " TUE Length is 4", (RedString_Length(s1) == 4));
        RedTest_Verify(suite, "Content is \" TUE\"", match);
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON TUE", 0);
        RedTest_Verify(suite, "MON TUE Length is 7", (RedString_Length(s0) == 7));
        RedTest_Verify(suite, "Content is \"MON TUE\"", match);

        RedString_AppendChars(s0, " WED");
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON TUE WED", 0);
        RedTest_Verify(suite, "MTW Length is 11", (RedString_Length(s0) == 11));
        RedTest_Verify(suite, "Content is \"MON TUE WED\"", match);

        RedString_AppendPrintf(s0, " %s", 100, "THU");
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON TUE WED THU", 0);
        RedTest_Verify(suite, "MTW Length is 15", (RedString_Length(s0) == 15));
        RedTest_Verify(suite, "Content is \"MON TUE WED THU\"", match);

        RedString_AppendPrintf(s0, "", 100);
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON TUE WED THU", 0);
        RedTest_Verify(suite, "MTW Length is 15", (RedString_Length(s0) == 15));
        RedTest_Verify(suite, "Content is \"MON TUE WED THU\"", match);

        RedString_AppendChars(s0, "");
        match = !RedString_CompareChars(RedString_GetChars(s0), "MON TUE WED THU", 0);
        RedTest_Verify(suite, "MTW Length is 15", (RedString_Length(s0) == 15));
        RedTest_Verify(suite, "Content is \"MON TUE WED THU\"", match);

        RedString_Free(s0);
        RedString_Free(s1);
    }


    /* Test RedString_RemoveToChar */
    /* Test RedString_ToNewChars */
    /* Test RedString_Rot13 */
    /* Test RedString_Split */
   
    {
        const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        RedString s0;
        RedStringList list;
        bool match;
        s0 = RedString_New("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
        list = RedString_Split(s0, ' ');
        RedString_Free(s0);

        RedTest_Verify(suite, "NumStrings == 12", (RedStringList_NumStrings(list) == 12));
        for (int i = 0; i < 12; i++)
        {
            s0 = RedStringList_GetString(list, i);
            match = !RedString_CompareChars(RedString_GetChars(s0), months[i], 0);
            RedTest_Verify(suite, "Month Length is 3", (RedString_Length(s0) == 3));
            RedTest_Verify(suite, "Content for month matches ", match);
        }

        s0 = RedString_New(NULL);
        RedStringList_Join(s0, list, " | ");
        RedTest_Verify(suite, "joined month length is 69", (RedString_Length(s0) == 69));
        match = !RedString_CompareChars(RedString_GetChars(s0), 
                "Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec", 0);
        RedTest_Verify(suite, " | joined content for months matches ", match);

        RedString_Clear(s0);
        RedStringList_Join(s0, list, NULL);
        RedTest_Verify(suite, "joined month length is 36", (RedString_Length(s0) == 36));
        match = !RedString_CompareChars(RedString_GetChars(s0), 
                "JanFebMarAprMayJunJulAugSepOctNovDec", 0);
        RedTest_Verify(suite, "NULL joined content for months matches ", match);

        RedString_Free(s0);
        RedStringList_Free(list);
    }

    {
        RedString s;
        RedStringList list;
        bool match;

        list = RedStringList_New();
        RedTest_Verify(suite, "NumStrings == 0", (RedStringList_NumStrings(list) == 0));

        RedStringList_AppendPrintf(list, "%1.1f", 1.5f);
        RedTest_Verify(suite, "NumStrings == 1", (RedStringList_NumStrings(list) == 1));
        RedStringList_AppendChars(list, "3.0");
        RedTest_Verify(suite, "NumStrings == 2", (RedStringList_NumStrings(list) == 2));
        RedStringList_AppendPrintf(list, "%s", "4.5");
        RedTest_Verify(suite, "NumStrings == 3", (RedStringList_NumStrings(list) == 3));

        s = RedString_New(NULL);
        RedStringList_Join(s, list, " ");

        RedTest_Verify(suite, "joined number length is 11", (RedString_Length(s) == 11));
        match = !RedString_CompareChars(RedString_GetChars(s), "1.5 3.0 4.5", 0);
        RedTest_Verify(suite, "number list joined content matches ", match);

        RedString_Free(s);
        RedStringList_Free(list);
    }

    /* Test RedStringList_NumStrings */
    /* Test RedStringList_AppendPrintf */
    /* Test RedStringList_AppendChars */
    /* Test RedStringList_ToNewChars */

    return RedTest_End(suite);
}
