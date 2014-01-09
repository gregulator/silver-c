libred - Do more with C.
=====

Collection of libraries that make C more powerful and easier to use without
sacrificing performance.

libred consists of several modules.  In some cases, alternative libraries exist
that you could use instead.  These alternatives may be faster or have features
that we don't support. So why choose libred?

 - **Ease-of-use and consistency**.  All libred modules follow the same coding
   styles and patterns, making the collection of modules holistically easy to
   learn and fun to use.

 - **Compatability**. All libred modules are ANSI c89-compliant, allowing you
   to incorporate the code into nearly any project.

 - **Public Domain License**.  The authors waive all copyrights, allowing you
   to use, copy, modify, and redistribute this code without restriction.

**PLANNED MODULES** 

 - `red_array` - Dynamic arrays that look like native C array.
 - `red_link` - Fast, generic linked lists
 - `red_hash` - Generic hash map implementation
 - `red_perfecthash` - Perfect hash map, for WORM applications.
 - `red_set` - Generic set data structure
 - `red_bloom` - Bloom filter data structure
 - `red_vec` - Vector math
 - `red_datetime` - Datetime module
 - _More coming soon..._

**STATUS**

UNSTABLE -- Code has only recently been comitted, and hasn't really been
tested.  We make no (forward or backward) compatibility guarantees at this
time.  Use this software at your own risk.
