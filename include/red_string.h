/*
 * ZString - Easy-to-use interface for working with strings.
 * 
 * A string is created from C-style null-terminated char * using the ZString_New
 * routine.  The characters of a string can be accessed using the
 * ZString_GetChars routine.
 */

#ifndef STRINGS_INCLUDED
#define STRINGS_INCLUDED

#include <stdbool.h>

/*
 * A ZString represents a string of characters.
 */
typedef struct ZString * ZStringHandle;
typedef const struct ZString * ConstZStringHandle;

/*
 * A ZStringList represents an array of strings.
 */
typedef struct ZStringList * ZStringListHandle;

#if 0
/*
 * A RegExp represents a regular expression pattern.  Regular expression
 * patterns are a seperate object type from strings for improved performance,
 * since they need to be compiled under-the-covers.
 */
typedef void * RegExpHandle;

/*
 * RegExpNew -- Creates and compiles a regular expression pattern.  Pattern
 *      should follow the POSIX extended regular expression format (see "man 7
 *      regex").
 */
RegExpHandle RegExpNew(const char *pattern);

/*
 * RegExpFree -- Frees resources associated with the compiled regex pattern.
 */
void RegExpFree(RegExpHandle *phRegExp);
#endif

/*
 * ZString_New -- Create a ZString object from a null-terminated array of chars.
 *      This routine makes a copy of <src> and stores the copy in the ZString
 *      object.  If <src> is NULL or "" an empty string of length 0 is created.
 */
ZStringHandle ZString_New(const char *src);

/*
 * ZString_NewLength -- Create a ZString object from a null-terminated array of
 * chars, limited to at most <length> characters..
 *
 *      This routine makes a copy of <src> and stores the copy in the ZString
 *      object.  If <src> is NULL or "" an empty string of length 0 is created.
 */
ZStringHandle ZString_NewLength(const char *src, unsigned length);

/*
 * ZString_NewPrintf -- Perform an snprintf and store the result in a newly
 *      allocated string.
 */
ZStringHandle ZString_NewPrintf(const char *fmt, unsigned size, ...);

/*
 * ZString_Free -- Frees a string's memory.
 */
void ZString_Free(ZStringHandle *phZString);

/*
 * ZString_Set -- Sets the contents of <hOut> to <in>.
 */
void ZString_Set(ZStringHandle hOut, const char *in);

/*
 * ZString_Set -- Clears the contents of <hOut> by setting it to "\0".
 */
void ZString_Clear(ZStringHandle hOut);

/*
 * ZString_Length -- Returns the number of characters in a string.
 */
unsigned ZString_Length(const ZStringHandle hZString);

/*
 * ZString_Bytes -- Returns the length in bytes of a string.
 */
unsigned ZString_Bytes(ZStringHandle hZString);

/*
 * ZString_GetChars -- Returns a C-style null-terminated array of chars for use
 *      in functions like "printf.  The returned pointer is only valid as long
 *      as no other ZString* calls are made using <hZString>.
 */
const char * ZString_GetChars(const ZStringHandle hZString);

/*
 * ZString_Copy -- Sets <hResult> to contain a copy of <hSrc>.  The previous
 *      contents of <hResult> are discarded.  Does nothing if hSrc and hResult
 *      are the same.
 */
void ZString_Copy(ZStringHandle hResult, const ZStringHandle hSrc);

enum
{
    /* Ignore case when doing comparison. */
    STRING_COMPARE_IGNORE_CASE_FLAG = 0x1,

    /* Ignore whitespace differences when doing comparison. */
    STRING_COMPARE_IGNORE_WHITESPACE_FLAG = 0x2,
};

/* 
 * ZString_Compare -- Compares two strings.  Returns 0 if the strings match.
 *      Returns -1 if <hZStringA> is less than <hZStringB>.  Returns 1 if
 *      <hZStringA> is greater than <hZStringA>.  <stringComparisonFlags> must be
 *      the bitwise OR of zero or more STRING_COMPARE_* values, and they affect
 *      the comparison as described above.
 */
int ZString_Compare(
        const ZStringHandle hZStringA, 
        const ZStringHandle hZStringB,
        unsigned stringComparisonFlags);

/*
 * ZStringCompareChars - Same as ZStringCompare but with (char *)s.
 */
int ZString_CompareChars(
        const char * strA, 
        const char * strB,
        unsigned stringComparisonFlags);

/* 
 * ZString_ContainsChars -- Returns True if <hHaystack> contains the characters
 *      of <needle> as a substring.  Returns False otherwise.
 */
bool ZString_ContainsChars(ConstZStringHandle hHaystack, const char *needle);

/*
 * ZString_BeginsWith -- Returns True if <hZString> begins with the characters of
 *      <needle>.
 */
bool ZString_StartsWith(ConstZStringHandle hZString, const char *needle);

/* ZString_Search -- Returns position of first occurance of <c> in <hHaystack>,
 *      or -1 if it is not found.
 */
int ZString_Search(ConstZStringHandle hHaystack, const char c);

/*
 * ZString_Trim -- Removes whitespace from the left and right sides of <hZString>.
 */
void ZString_Trim(ZStringHandle hZString);

/*
 * ZString_RemoveFloat -- Read a floating-point number from the beginning of
 *      <hZString>, remove it from <hZString>, and return it. */
float ZString_RemoveFloat(ZStringHandle hZString);

/*
 * ZString_FirstNonWhitespaceChar -- Returns the first non-whitespace character
 *      of <hZString>.
 */
char ZString_FirstNonWhitespaceChar(ConstZStringHandle hZString);

/*
 * ZString_SubString -- Sets <hResult> to contain a substring of <hSrc>
 *      (discarding <hResult>'s contents).  <start> and <end> specify the
 *      inclusive range of characters to copy.  
 *
 *      0 indicates the first character in the string.
 *      (ZString_Length(hSrc) - 1) is the last character in the string.
 *
 *      Negative values can be used to count backwards starting with the end of
 *      the string.
 *
 *      -1 indicates the last character in the string.
 *      -(ZString_Length(hSrc)) indicates the first character in the string. 
 * 
 *      If <start> or <end> are outside the string, only characters that fall
 *      within the string are copied.  For example:
 *      
 *          // This code sets hDest to "Hell".
 *          ZStringHandle hSrc = ZString_New("Hello");
 *          ZStringHandle hDest = ZString_New(NULL);
 *          ZStringSubString(hDest, hSrc, -7, -2);
 *
 *      If <end> comes before <start>, the result is the empty string.
 *
 *      It is allowed for hResult to equal hSrc.
 */
void ZString_SubString(
        ZStringHandle hResult, 
        const ZStringHandle hSrc, 
        int start, 
        int end);

unsigned ZString_ToU32(ConstZStringHandle hZString);

/*
 * ZString_Reverse -- Reverses the contents of <hZString> in place.
 */
void ZString_Reverse(ZStringHandle hZString);

typedef enum
{
    STRING_CASE_UPPER,
    STRING_CASE_LOWER,
} ZStringCase; 

/* 
 * ZString_CaseConvert -- Changes the case of <hZString> in place.
 */
void ZString_CaseConvert(ZStringHandle hZString, ZStringCase newCase);

/*
 * ZString_Append -- Adds <hAppend> to the end of <hOriginal>.
 */
void ZString_Append(ZStringHandle hOriginal, const ZStringHandle hAppend);

/*
 * ZString_AppendChars -- Adds NULL-terminated string <pAppend> to the end of
 *      <hOriginal>.
 */
void ZString_AppendChars(ZStringHandle hOriginal, const char * pAppend);

/*
 * ZString_AppendPrintf -- Adds <hAppend> to the end of <hOriginal>.
 */
void ZString_AppendPrintf(ZStringHandle hOriginal, const char *fmt, unsigned size, ...);

/*
 * ZString_RemoveToChar -- Remove all characters in <hZString> to the left of (and
 *      including) the first occurance of <c>.  If <c> does not appear in
 *      <hZString>, this clears the string.
 */
void ZString_RemoveToChar(ZStringHandle hZString, char c);

/*
 * ZString_Hash -- Compute a hash string from <hSrc> and store it in <hResult>.
 *      It is valid for <hResult> and <hSrc> to be the same string.
 *
 * TODO: Let caller choose hash type (SHA1, etc)
 */
void ZString_Hash(ZStringHandle hResult, ConstZStringHandle hSrc);

/*
 * ZString_Rot13 -- Rot13 encryption/decryption.  Rot13 is extremely weak
 *      encryption and should not be used in situations where security is a
 *      real concern.  <hResult> and <hOriginal> can be the same string.
 *
 *      Note: g? in VIM will apply Rot13 encryption/decryption.
 */
void ZString_Rot13(ZStringHandle hResult, ZStringHandle hOriginal);

/*
 * ZString_Split -- Creates a new ZStringList object by splitting <hZString> into
 *      several smaller strings.  The <delimeter> character determines where
 *      the boundaries are, and all <delimeter> characters are removed.
 *
 *      For example: 
 *          splitting "This is a:  test" 
 *          with delimeter ' ' will produce:
 *
 *          "This"
 *          "is"
 *          "a:"
 *          ""
 *          "test"
 *
 *      Use '\n' for <delimeter> to split a multi-line string into individual
 *      lines.
 *
 *      The returned ZStringList object must be freed with ZString_ListFree() when
 *      you are finished using it.  The individual strings of the may be
 *      modified (ie, with ZStringAppend, ZStringReverse, etc), but MUST NOT be
 *      freed with ZStringFree.
 */
ZStringListHandle ZString_Split(ZStringHandle hZString, char delimiter);

/*
 * ZStringList_NumZStrings -- Returns the number of strings in <hList>.
 */
unsigned ZStringList_NumStrings(ZStringListHandle hList);

/*
 * ZStringList_GetString -- Returns the string in position <idx> of <hList>.
 *      Will ASSERT if <idx> is greater than or equal to the number of strings
 *      in <hList>.
 *
 *      The ZStringHandles returned by this routine must never be individually
 *      freed.  Instead, use ZStringList_Free to free the whole list when you
 *      are finished with it.
 */
ZStringHandle ZStringList_GetString(ZStringListHandle hList, unsigned idx);

void ZStringList_Join(ZStringHandle hString, ZStringListHandle hList, const char *joiner);

#if 0
/*
 * ZStringList_Free -- Frees all strings contained in <*phList> and all other
 *      resources associated with the list.  After calling ZStringList_Free you
 *      may no longer use strings obtained from <*phList> or BAD THINGS will
 *      happen.
 */
void ZStringList_Free(ZStringListHandle *phList);

typedef enum
{
    STRING_REPLACE_IGNORE_CASE_FLAG,
    STRING_REPLACE_ALL,
} ZStringReplaceFlags;

/*
 * ZStringReplace -- not yet supported
 */
/*void ZStringReplace(
        ZStringHandle hResult,
        const ZStringHandle hSrc,
        const ZStringHandle hFind,
        const ZStringHandle hReplacement,
        unsigned replaceFlags);*/

/*
 * ZStringReplaceRegExp -- Not yet supported
 */
/*void ZStringReplaceRegExp(
        ZStringHandle hResult,
        const ZStringHandle hSrc,
        const RegExpHandle hRegExp,
        const ZStringHandle hReplacement);*/


/* ZStringSplit -- Returns an array of words of <hZString>.  <hSeperator> If
 * <hSeperator> is NULL, then the words are separated by arbitrary strings of
 * whitespace characters.
 */
/*Array ZStringSplitChar(
        ZString
        const ZStringHandle hSeparator,
        )*/

#endif
#endif // STRINGS_INCLUDED
