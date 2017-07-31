/* Copyright (c) 2015-2016 Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                                    *
 * This software is provided 'as-is', without any express or implied                  *
 * warranty. In no event will the authors be held liable for any damages              *
 * arising from the use of this software.                                             *
 *                                                                                    *
 * Permission is granted to anyone to use this software for any purpose,              *
 * including commercial applications, and to alter it and redistribute it             *
 * freely, subject to the following restrictions:                                     *
 *                                                                                    *
 * 1. The origin of this software must not be misrepresented; you must not            *
 *    claim that you wrote the original software. If you use this software            *
 *    in a product, an acknowledgement in the product documentation would be          *
 *    appreciated but is not required.                                                *
 * 2. Altered source versions must be plainly marked as such, and must not be         *
 *    misrepresented as being the original software.                                  *
 * 3. This notice may not be removed or altered from any source distribution.         *
 */
#ifndef GUARD_DEBUG_NEW_CONF_INL
#define GUARD_DEBUG_NEW_CONF_INL 1

//////////////////////////////////////////////////////////////////////////
// Enable, to get all the old (non-conforming) macros
#ifndef DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#if defined(_DEBUG_NEW_WANT_DEPRECATED)
# define DEBUG_NEW_CONFIG_HAVE_DEPRECATED _DEBUG_NEW_WANT_DEPRECATED
#elif 0
# define DEBUG_NEW_CONFIG_HAVE_DEPRECATED 1
#else
# define DEBUG_NEW_CONFIG_HAVE_DEPRECATED 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED */

#if !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !defined(_DEBUG_NEW_CONF_INL)
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !defined(_DEBUG_NEW_CONF_INL)
#define _DEBUG_NEW_CONF_INL 1 // backwards compatibility with old name
#endif
#if !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !defined(_DEBUG_NEW_CONF_H)
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !defined(_DEBUG_NEW_CONF_H)
#define _DEBUG_NEW_CONF_H 1 // backwards compatibility with old name
#endif


#ifndef __has_include
#define __has_include(x) 0
#endif /* !__has_include */
#ifndef __has_feature
#define __has_feature(x) 0
#endif /* !__has_feature */
#ifndef __has_extension
#define __has_extension __has_feature
#endif /* !__has_extension */
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif /* !__has_declspec_attribute */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif /* !__has_attribute */
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif /* !__has_builtin */

#ifndef DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#if defined(__GLIBC__)
# define DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE 1
#else
# define DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE */
#ifndef DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#if defined(_WIN32) || defined(WIN32) || \
    defined(_WIN64) || defined(WIN64) || \
    defined(__WINDOWS__) || defined(__MINGW32__) || \
    defined(__MINGW64__) || defined(__CYGWIN__)
# define DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE 1
#else
# define DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE */
#ifndef DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#if defined(__APPLE__) || defined(__FreeBSD__)
# define DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE 1
#else
# define DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE>>
// - Provide an implementation of "malloc_msize".
//   NOTE: By default this is only enabled, if the native library
//         already provides such a function, but debug_new is able,
//         to always provide this (but only in debug-mode)
// - NOTE: Define to 1, to force debug_new, to  provide this intrinsic, if debug_new is enabled.
//         - Is defined as 0, this intrinsic will only be enabled,
//           if it can be provided, even when compiling with "DEBUG_NEW_CONFIG_ENABLED == 0"
// - Always having "DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE" enabled will
//   create inconsistencies between debug and release builds.
//   """
//    void *p = malloc(10);
//    #ifdef malloc_msize
//    printf("size = %d\n",(int)malloc_msize(p)); // "size = 10"
//    #endif // malloc_msize
//   """
// VALUES:
//   2 : AUTOMATIC : Enable, if supported by native API)
//   1 : ENABLED   : Won't work, if debug_new is disabled and the native API doesn't support it)
//   0 : DISABLED  : malloc_msize is never available
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#define DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE 2
#endif /* !DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */



// ** fast configuration modes:
//    for more information see associated configs

//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_FASTCFG_ENABLED>>
//  - fully enables debug_new: all features enabled
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_FASTCFG_ENABLED
#if defined(_DEBUG_NEW_FASTCFG_ENABLED) && DEBUG_NEW_CONFIG_HAVE_DEPRECATED
# define DEBUG_NEW_FASTCFG_ENABLED  _DEBUG_NEW_FASTCFG_ENABLED
#else
# define DEBUG_NEW_FASTCFG_ENABLED  0
#endif
#endif /* !DEBUG_NEW_FASTCFG_ENABLED */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_FASTCFG_LEAKFIX>>
//  - Runtime-based leak fixer.
//    - Fixes memory-leaks at runtime, without reporting or
//      including any source-based informations like 
//      "__FILE__", "__LINE__" and the likes.
//  - Imagine this:
//    - You were working on some application for months, and are
//      currently debugging some memory-leak.
//    - Your boss comes in and
//      - demands you to show him the program but due to some
//        really bad memory-leaks, it will only run for ~5 secs.
//      - forces you to publish a release __NOW__!
//    - Don't worry! just turn this on and the leaks will be fixed
//      for you (remember: debug_new can't call your fancy c++
//      destructors, when fixing leaks)
//      NOTE: For this to work properly, you should add
//            "mstate_push()" and "mstate_pop()"
//            to the main loop of your app.
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_FASTCFG_LEAKFIX
#if defined(_DEBUG_NEW_FASTCFG_LEAKFIX) && DEBUG_NEW_CONFIG_HAVE_DEPRECATED
# define DEBUG_NEW_FASTCFG_LEAKFIX  _DEBUG_NEW_FASTCFG_LEAKFIX
#else
# define DEBUG_NEW_FASTCFG_LEAKFIX  0
#endif
#endif /* !DEBUG_NEW_FASTCFG_LEAKFIX */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_FASTCFG_DISABLED>>
//  - fully disables debug_new: no debug-related features enabled
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_FASTCFG_DISABLED
#if defined(_DEBUG_NEW_FASTCFG_DISABLED) && DEBUG_NEW_CONFIG_HAVE_DEPRECATED
# define DEBUG_NEW_FASTCFG_DISABLED _DEBUG_NEW_FASTCFG_DISABLED
#else
# define DEBUG_NEW_FASTCFG_DISABLED 0
#endif
#endif /* !DEBUG_NEW_FASTCFG_DISABLED */


#if DEBUG_NEW_FASTCFG_ENABLED
// v just enable debug_new. Most other configs
//   will default to offer debug support.
# define DEBUG_NEW_CONFIG_ENABLED 1
#elif DEBUG_NEW_FASTCFG_LEAKFIX
# define DEBUG_NEW_CONFIG_ENABLED                     1 //< we need the basic debug_new
# define DEBUG_NEW_CONFIG_HAVE_STATS                  0 //< don't need stats
# define DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE          0 //< don't need clear-on-free
# define DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE      0 //< don't need report-leaks-on-shutdown
# define DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS              1 //< fix leaks, when they occur (thats your only job!)
# define DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC              0 //< don't need advanced allocation info
# define DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN 0 //< runtime-leak fixing is slow enough as it is (this isn't a problem anymore since win 95)
# define DEBUG_NEW_CONFIG_HAVE_LOG                    0 //< we don't need logs
// ** TODO: config for disabling logging and debug-information inclusion
#elif DEBUG_NEW_FASTCFG_DISABLED
# define DEBUG_NEW_CONFIG_ENABLED 0 //< disable debug_new
#endif


//////////////////////////////////////////////////////////////////////////
// <<_DEBUG>>
//  - Although this implication is technically correct,
//    anybody forgetting, to define "NDEBUG" in release-mode,
//    would include debug-informations.
//  - I personally find it more logical, to explicitly specify
//    a debug-mode, rather than explicitly saying, your building
//    in release-mode.
//  - You might not share my opinion in that matter.
//    And if you aren't, just enable this (it implies debug-mode,
//    if neither "NDEBUG" nor "_RELEASE" are defined):
//////////////////////////////////////////////////////////////////////////
#if 0 //< replace with "#if 1", to enable
# ifndef _DEBUG
#  if !defined(NDEBUG) && !defined(_RELEASE)
#   define _DEBUG 1
#  endif /* !NDEBUG && !_RELEASE */
# endif /* !_DEBUG */
#endif



//////////////////////////////////////////////////////////////////////////
// Set to 1, to statically link debug_new, when #include-ing <debug_new.h>
#ifndef DEBUG_NEW_CONFIG_LINK_STATICALLY
#if (defined(_ONE_SOURCE) || defined(ONE_SOURCE))
# define DEBUG_NEW_CONFIG_LINK_STATICALLY 1
#else
# define DEBUG_NEW_CONFIG_LINK_STATICALLY 0
#endif
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */



//////////////////////////////////////////////////////////////////////////
// <<main debug_new toggle>>
//  - Only if "DEBUG_NEW_CONFIG_ENABLED" is defined and true,
//    any of the features from debug_new will be enabled.
//  - When not enabled, you can safely compile for release mode,
//    knowing, that no information, like "__FILE__" or the like
//    will get leaked into your release.
//  - That is, why (by default) "DEBUG_NEW_CONFIG_ENABLED" will be defined,
//    when "_DEBUG" is defined.
//    ("_DEBUG" being the standard way of indicating debug builds)
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_ENABLED
#if defined(_WANT_DEBUG_NEW) && DEBUG_NEW_CONFIG_HAVE_DEPRECATED
// Support for old debug_new
# define DEBUG_NEW_CONFIG_ENABLED _WANT_DEBUG_NEW
#elif defined(_DEBUG)
# define DEBUG_NEW_CONFIG_ENABLED 1
#else /* _DEBUG */
# define DEBUG_NEW_CONFIG_ENABLED 0
#endif /* !_DEBUG */
#endif /* !DEBUG_NEW_CONFIG_ENABLED */

//////////////////////////////////////////////////////////////////////////
// Automatically initialize debug_new / the native API as soon as possible
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#define DEBUG_NEW_CONFIG_HAVE_AUTO_INIT 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_AUTO_INIT */



//////////////////////////////////////////////////////////////////////////
// <<main method initialization>>
//  - in c, I can't initialize debug_new through static initializers!
//  - so you (the user) have to tell me, when to initialize myself
//  - to make that easier and have less debug code scattered through
//    your application:
//    """
//     #ifndef __cplusplus
//     # define DEBUG_NEW_MAIN_HOOK 1
//     #endif /* !__cplusplus */
//    """
//    (NOTE: of course you can leave the "#ifndef ..." and "#endif ...", 
//           if your application is only intended to compile with c)
//  - add this to the top of the source file,
//    that implements your main method.
//    through some preprocessor magic,
//    I will jam myself right before the call to your
//    main method and initialize (and after your main method ends finalize)
//    debug_new (of course only, if "DEBUG_NEW_CONFIG_HAVE_AUTO_INIT" and 
//    therefor "DEBUG_NEW_CONFIG_ENABLED" and therefor "_DEBUG"; (by default) are
//    set as well. So don't worry; I won't overwrite anything in release-mode!)
//  - NOTE: If you aren't giving debug_new any other way of initialization,
//          either with the "DEBUG_NEW_MAIN_HOOK" trick, or with c++ static initialization,
//          you must call Dn_Init() and Dn_Quit() from you main method, if they are defined:
//          - debug_new will not define these macros, unless you really __HAVE__ to call them!
//    >> int main(int argc, char *argv[]) {
//    >> #ifdef Dn_Init
//    >>  Dn_Init();
//    >> #endif
//    >>   ...
//    >> #ifdef Dn_Quit
//    >>  Dn_Quit();
//    >> #endif
//    >>  return ...;
//    >> }
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_MAIN_HOOK
#if defined(_DEBUG_NEW_MAIN_HOOK_INIT) && DEBUG_NEW_CONFIG_HAVE_DEPRECATED
// Support for old debug_new
# define DEBUG_NEW_MAIN_HOOK _DEBUG_NEW_MAIN_HOOK_INIT
#elif defined(_ONE_SOURCE) || defined(ONE_SOURCE)
// v knowing, there will only be 1 source, we can just
//   straight up hook the main method, without linkage errors
# define DEBUG_NEW_MAIN_HOOK 1
#else /* _ONE_SOURCE || ONE_SOURCE */
// v define as 0, to not accidentally implement "main" in some
//   random header
# define DEBUG_NEW_MAIN_HOOK 0
#endif /* !_ONE_SOURCE || ONE_SOURCE */
#else /* !DEBUG_NEW_MAIN_HOOK */
#if !DEBUG_NEW_MAIN_HOOK && (defined(_ONE_SOURCE) || defined(ONE_SOURCE))
# undef DEBUG_NEW_MAIN_HOOK
# define DEBUG_NEW_MAIN_HOOK 1 /*< we __really__ only get 1 chance! */
#endif /* !DEBUG_NEW_MAIN_HOOK && (_ONE_SOURCE || ONE_SOURCE) */
#endif /* DEBUG_NEW_MAIN_HOOK */

#if defined(__cplusplus)
// Static initialization happens before the call to main
#undef DEBUG_NEW_MAIN_HOOK
#define DEBUG_NEW_MAIN_HOOK 0
#endif /* defined(__cplusplus) */


//////////////////////////////////////////////////////////////////////////
// Memory alignment of allocated blocks
//  - must be power of 2 (DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT == 2^n)
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT
#define DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT 16 
#endif /* !DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT */



//////////////////////////////////////////////////////////////////////////
// <<Tail Check>>
//  - check the tail of pointers, to debug errors
//    associated with writing past the allocated space of a pointer:
//  """
//   void some_func(void) {
//    int *array = new int[10];
//    for (unsigned int i = 0; i <= 10; ++i) { //< is should be "<" not "<="
//     array[i] = 10-i;
//    }
//    delete array; //< error will be triggered, telling you, 
//                  //  that you written past the end of "array"
//   }
//  """
// NOTE: undef or set to 0, to compile without tail check
// DEFAULT: 8 (sizeof(intmax_t))
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_TAILCHECK
#define DEBUG_NEW_CONFIG_HAVE_TAILCHECK 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_TAILCHECK */
#ifndef DEBUG_NEW_CONFIG_TAILCHECK_SIZE
#if DEBUG_NEW_CONFIG_HAVE_TAILCHECK
# define DEBUG_NEW_CONFIG_TAILCHECK_SIZE  8
#else /* DEBUG_NEW_CONFIG_HAVE_TAILCHECK */
# define DEBUG_NEW_CONFIG_TAILCHECK_SIZE  0
#endif /* !DEBUG_NEW_CONFIG_HAVE_TAILCHECK */
#endif /* !DEBUG_NEW_CONFIG_TAILCHECK_SIZE */



//////////////////////////////////////////////////////////////////////////
// <<Tail Check>>
//  - Character used, to mark memory space after the available space.
//    When allocating memory, "DEBUG_NEW_CONFIG_TAILCHECK_SIZE" bytes
//    behind the allocated memory will be filled with "DEBUG_NEW_CONFIG_TAILCHECK_CHAR"
//  - Yes, that means if your code were write "DEBUG_NEW_CONFIG_TAILCHECK_CHAR" past
//    the end, I wouldn't be able, to detect that. (but what are the chances?)
//  - default is '\xAA', because '\xAA' == "0b01010101", to maximize bit individuality
//////////////////////////////////////////////////////////////////////////
#if (DEBUG_NEW_CONFIG_TAILCHECK_SIZE != 0) && !defined(DEBUG_NEW_CONFIG_TAILCHECK_CHAR)
#define DEBUG_NEW_CONFIG_TAILCHECK_CHAR   '\xAA'
#endif /* DEBUG_NEW_CONFIG_TAILCHECK_SIZE && !DEBUG_NEW_CONFIG_TAILCHECK_CHAR */



//////////////////////////////////////////////////////////////////////////
// <<Memory Stats>>
//  - stats can be useful, to profile memory usage of your application
//  - stats include infos about:
//    - [total_calls_malloc]:            total amount of calls to "DEBUG_NEW_API_MALLOC" / "malloc"
//    - [total_calls_realloc]:           total amount of calls to "DEBUG_NEW_API_REALLOC" / "realloc"
//    - [total_calls_calloc]:            total amount of calls to "DEBUG_NEW_API_CALLOC" / "calloc"
//    - [total_calls_free]:              total amount of calls to "DEBUG_NEW_API_FREE" / "free"
//    - [c++][total_calls_new]:          total amount of calls to "operator new"
//    - [c++][total_calls_array_new]:    total amount of calls to "operator new[]"
//    - [c++][total_calls_delete]:       total amount of calls to "operator delete"
//    - [c++][total_calls_array_delete]: total amount of calls to "operator delete[]"
//    - [total_alloc_blocks]:            total amount of allocated memory blocks (n existing "mem_ptr_t")
//    - [max_alloc_blocks]:              max amount of memory blocks existing at any given time
//    - [total_alloc]:                   total memory allocated
//    - [max_alloc]:                     max amount of memory allocated at any given time
//  - Stats are also used, to quickly confirm leaks / frees
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_STATS
#define DEBUG_NEW_CONFIG_HAVE_STATS 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOC_ID>>
// NOTES:
//   - Every allocation is given an ID, based on the number
//     of allocation calls before.
//   --> Thereby, that ID will remain constant across debugging attempts
//       in a linearly executed scope (like a test)
//   - The ID will be print along with any other enabled information,
//     when debug_new is reporting a problem, like a leak.
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
#define DEBUG_NEW_CONFIG_HAVE_ALLOC_ID 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT>>
// NOTES:
//   - Adds functionality, for break at on specific allocation.
//     Useful, if your application application always produces a leak
//     on a specific allocation (like e.g.: 510).
//   - When this is enabled, one can setup a breakpoint, by calling:
//     "DEBUG_NEW_API_ALLOC_BREAKPOINT(510);", before the leak happens.
//   - A set breakpoint can be clear at any point, by calling:
//     "DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR();"
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
# define DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT 1
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
# define DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */



//////////////////////////////////////////////////////////////////////////
// <<Memory clear on free>>
//  - when freeing memory through "delete", "delete[]" or "free",
//    that memory will be set to 0.
//  - I always like it, when I set memory to 0 before deallocation,
//    because when dealing with dangling pointers, it always
//    makes it easier to identify deallocated structures:
//    """
//     void some_func(void) {
//      struct list_t {
//       list_t *prev;
//       list_t *next;
//       int value;
//       list_t(int v): value(v) {}
//      };
//      list_t *a = new list_t(10);
//      list_t *b = a->prev = new list_t(20);
//      b->next = a;
//      // ...
//      if (b->prev) b->prev->next = 0; 
//      // ** some guy forgot to unlink "b->next" as well
//      delete b;
//      // ...
//      if (a->next) a->next->prev = 0; 
//      if (a->prev) a->prev->next = 0; 
//      //^ this line will cause an access violation, because a->prev is a dangling pointer.
//      //  in a debugger view, this cannot be identified easily, because setting
//      //  "x->prev" and "x->next" to "0" before deallocating "x", is not required by the contract
//      //  with "DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE" enabled, memory will always be set to 0
//      //  and when inspecting a->prev in a debugger view, it will show all fields set to 0,
//      //  indicating, that this is a dangling pointer!
//      delete a;
//     }
//    """
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE
#define DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC>>
//  - Adds a debug handler for trying to allocate more memory,
//    than could ever be available at once.
//  - NOTES:
//    - Not to be confused with "DnPrivate_HandleOutOfMemory",
//      with gets called, when not enough memory is available at
//      the time of the allocation request.
//    - If disabled, the failed allocation will be handled
//      in the current context (with "DnPrivate_HandleOutOfMemory")
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
#define DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC */


//////////////////////////////////////////////////////////////////////////
// <<Autocreate global state>>
//  - leak and free detection works on a state basis.
//  - states are images of memory situations.
//  - there are 2 kind of states:
//    - global state (exists once per application)
//    - thread state (exists once per application thread)
//  - to detect memory leaks, states are pushed and popped,
//    and compared to active thread / global state.
//  - when "DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE" is defined,
//    debug_new will push a global state on application launch
//    and pop that state on shutdown
//  - technically "DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE" is my
//    version of <report-memory-leaks-on-shutdown>
//    --> #define DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE _REPORT_MEMORY_LEAKS_ON_SHUTDOWN ...
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE
#define DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE */


//////////////////////////////////////////////////////////////////////////
// <<Auto-fix memory leaks>>
//  - when detecting a memory leak, associated data will be logged.
//  - if "DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS" is defined, the pointer associated
//    with that leak will be freed afterwards
//    --> basically you can use "DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS", to keep testing / debugging
//        your application, without having to deal with those memory leaks first:
//  """
//   void some_func(void) {
//    mstate_push();
//     int *x = new int[42];
//     int *y = new int();
//    mstate_pop(); //< if "DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS" is defined,
//                  //  this is (in addition to memory leak warnings)
//                  //  equivalent to:
//                  //    """
//                  //     delete x;
//                  //     delete y;
//                  //    """
//                  //  as you can see, the problem here is, that the array
//                  //  memory only gets deallocated, but not destroyed.
//                  //   - "int::~int()" doesn't get called
//                  //     - this may not be too bad for int or any pod type for that matter,
//                  //       but non-pod classes and structs will only get deallocated; not destructed
//   }
//  """
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
#define DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS 0
#endif /* !DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL>>
//  - If defined, "free(0)" is allowed and ignored
//    (according to this: "http://www.cplusplus.com/reference/cstdlib/free/",
//     that is intended behavior, but it seams kind of weird to me...)
//    NOTES:
//     - In versions prior to "0.4.0", debug_new warned about freeing
//       0 pointers. (this simply disables that warning)
//     - Enabled by default, because __STDC__ behavior
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
#define DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL>>
//  - Similarly to "DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL", this allows
//    makes "realloc(0,X)" behave identically to "malloc(X)".
//  - Even though I see, where and why this is useful,
//    it still seems like something, that should result in an
//    access-violation, because its resizing <nothing>.
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL
#define DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL */


#if !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
#if defined(_MSC_VER)
# pragma message("Warning : Disabling DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL is deprecated! Consider using \"free_nn\"")
#else
# warning        "Warning : Disabling DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL is deprecated! Consider using \"free_nn\""
#endif
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL */

#if !DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL
#if defined(_MSC_VER)
# pragma message("Warning : Disabling DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL is deprecated! Consider using realloc_nn")
#else
# warning        "Warning : Disabling DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL is deprecated! Consider using realloc_nn"
#endif
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN>>
//  - Any leaks, that still remain, when "Dn_Quit"
//    is called will get freed manually
//    (shouldn't really be necessary, but feels 
//     like the right thing to do)
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN
#define DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION>>
//  - Adds recursion to "_Dn_Init" and "_Dn_Quit",
//    to prevent errors, when using the both
//    main-method-hook and Static initialization in c++
//  - Goes with the usual pro and cons
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
#define DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */



//////////////////////////////////////////////////////////////////////////
// <<_DEBUG_NEW_HAVE_LEAKS_LOG>>
//  - Adds logging capabilities to the debug_new standard printfunc
//  - Although this is why I added the possibility of overwriting
//    the print function (to add you own logging functionality),
//    This is just a debug library.
//    --> So here is that functionality in the standart.
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_LOG
#if defined(DEBUG_NEW_CONFIG_LOG_FILENAME)
# define DEBUG_NEW_CONFIG_HAVE_LOG 1
#else
# define DEBUG_NEW_CONFIG_HAVE_LOG 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_LOG */
#if DEBUG_NEW_CONFIG_HAVE_LOG && !defined(DEBUG_NEW_CONFIG_LOG_FILENAME)
#define DEBUG_NEW_CONFIG_LOG_FILENAME "debug_new.log"
#endif /* DEBUG_NEW_CONFIG_HAVE_LOG && !DEBUG_NEW_CONFIG_LOG_FILENAME */



// Disable features, that are unavailable / don't matter, if debug_new isn't enabled
#if !DEBUG_NEW_CONFIG_ENABLED
#undef DEBUG_NEW_CONFIG_HAVE_LOG
#undef DEBUG_NEW_CONFIG_LOG_FILENAME
#undef DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
#undef DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN
#undef DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
#undef DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE
#undef DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
#undef DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE
#undef DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
#undef DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
#undef DEBUG_NEW_CONFIG_HAVE_STATS
#undef DEBUG_NEW_CONFIG_HAVE_TAILCHECK
#undef DEBUG_NEW_CONFIG_TAILCHECK_SIZE
#undef DEBUG_NEW_CONFIG_TAILCHECK_CHAR
#undef DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT
#define DEBUG_NEW_CONFIG_HAVE_LOG 0
#define DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION 0
#define DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN 0
#define DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS 0
#define DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE 0
#define DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC 0
#define DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE 0
#define DEBUG_NEW_CONFIG_HAVE_ALLOC_ID 0
#define DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT 0
#define DEBUG_NEW_CONFIG_HAVE_STATS 0
#define DEBUG_NEW_CONFIG_HAVE_TAILCHECK 0
#define DEBUG_NEW_CONFIG_TAILCHECK_SIZE 0
#define DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT 1 // No guaranty given for this by default!
#endif /* !DEBUG_NEW_CONFIG_ENABLED */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS>>
//  - overwrite operator new, new[], delete and delete[]
//    outside of debug mode, to always call the
//    user definable "_OUT_OF_MEMORY" macro, when 
//    allocation fails
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS
#define DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS */

//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ>>
// VALUES:
//   1 : ENABLED
//   0 : DISABLED
#ifndef DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#define DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */

//////////////////////////////////////////////////////////////////////////
// Enable the aligned_malloc and friends functions (new in v101+)
#ifndef DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED>>
//   - Link debug_new extensions, even when debug_new is disabled:
//     >> // unlink debug_new
//     >> // if DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED is defined as 0,
//     >> // debug_new extensions, like mstate_push() and malloc_msize() will
//     >> // not get defined as stub.
//     >> void *p = malloc(42);
//     >> #include <debug_new_disable.inl>
//     >> #if DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED
//     >> mstate_push();   // Stub (always returns 1)
//     >> malloc_msize(p); // Linked agains the platform specific native function (_msize / malloc_size / malloc_usable_size)
//     >> ...
//     >> #else
//     >> mstate_push();   // Undefined function
//     >> malloc_msize(p); // Undefined function
//     >> ...
//     >> #endif
//     >> #include <debug_new_enable.inl>
//     >> malloc_msize(p); // Linked agains the debug/native api
//     >> free(p);
// VALUES:
//   1 : ENABLED
//   0 : DISABLED
#ifndef DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED
#define DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED 1
#endif /* !DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC>>
//  - If enabled, advanced allocation information can be stored
//    in pointer records.
//  - Most of the time, when debugging memory leaks,
//    the leak occurred in a custom allocator method, that simply got
//    called from elsewhere:
//    """
//     typedef struct {
//      int value;
//     } AwesomeOb_t;
//     AwesomeOb_t *alloc_AwesomeOb(int v) {
//      AwesomeOb_t *result = (AwesomeOb_t *)malloc(sizeof(AwesomeOb_t));
//      if (!result) { _OUT_OF_MEMORY(); return 0; }
//      result->value = v;
//      return result;
//     }
//     void free_AwesomeOb(AwesomeOb_t *ob) {
//      free(ob);
//     }
//    """
//    When using these custom method, debug_new can detect the leaks,
//    but can only tell you, where the call to malloc, for the leaked
//    object happened.
//    So you will know your application suffers from memory-leaks,
//    but you don't know, where exactly those object were allocated.
//  - with "DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC", you can add custom leak information
//    in the call to "malloc", (then called "mallocf"):
//    """
//     AwesomeOb_t *alloc_AwesomeOb(int v) {
//      AwesomeOb_t *result = (AwesomeOb_t *)mallocf(sizeof(AwesomeOb_t),"%d",v);
//      if (!result) { _OUT_OF_MEMORY(); return 0; }
//      result->value = v;
//      return result;
//     }
//    """
//    - While still not as good, as a proper traceback could be,
//      it might give you a hint, that will help you track down
//      your leak.
//  VALUES:
//    2 : Format information is ignored
//    1 : ENABLED
//    0 : DISABLED
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && defined(_DEBUG_NEW_ADVANCED_ALLOC)
// Support for old debug_new
#define DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC _DEBUG_NEW_ADVANCED_ALLOC
#else
#define DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC 1
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC */
#ifndef DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
// v if enabled, the "*alloc_ex" methods have their own access
//   (wouldn't recommend; gets kind of overwhelming...)
#define DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS 0
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS>>
//  - if enabled, debug_new will overwrite c/c++ default methods / operators:
//    - [c/c++]           malloc
//    - [c/c++]           realloc
//    - [c/c++]           calloc
//    - [c/c++]           free
//    - [c/c++]           strdup
//    - [c/c++]           wcsdup
//    - [c/c++]           mbsdup
//    - [c/c++]           strndup
//    - [c/c++]           wcsndup
//    - [c/c++]           mbsndup
//    - [c/c++]           _strdup
//    - [c/c++]           _wcsdup
//    - [c/c++]           _mbsdup
//    - [c/c++][posix]    malloc_usable_size
//    - [c/c++][windows]  _msize
//    - [c/c++][osx]      malloc_size
//    - [c++]             new
//    - [c++]             new[]
//  - Even if not enabled, debug_new always provides macro alternatives,
//    that behave exactly the same: "DEBUG_NEW_API_xxx", where "xxx" is
//    the name of the function / operator
//  - NOTE: These macros are even available, if debug_new is disabled,
//          in which case they call the regular debug-unrelated methods.
//  - NOTE: As a cross-platform generic malloc-size method, debug_new
//          provides "malloc_msize", if the platform already provides such a method.
//          To test for that capability, one can simply check for "#ifdef malloc_msize"
//          Although debug_new could technically always provide such a method,
//          it would only be able, to do so while enabled.
//          Therefor "malloc_msize" is defined __ONLY__ if it can
//          be provided in debug + release mode.
//  NOTE: You should never disable this, as functions like
//        malloc and mallocf may link against different apis!
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS
#define DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_DEBUG>>
//  - Enables features (mainly assertions), to debug debug_new itself
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_DEBUG
#define DEBUG_NEW_CONFIG_HAVE_DEBUG 0
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEBUG */

//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ>>
//  - Enable assertions, to make sure my newly invented
//    malloc_nz and friend functions are being used properly
#ifndef DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
#if DEBUG_NEW_CONFIG_ENABLED
# define DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ 1
#else /* DEBUG_NEW_CONFIG_ENABLED */
# define DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ 0
#endif /* !DEBUG_NEW_CONFIG_ENABLED */
#endif /* !DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ */


#ifndef DEBUG_NEW_CONFIG_HAVE_ASSERT
#if DEBUG_NEW_CONFIG_HAVE_DEBUG || \
    DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_CONFIG_HAVE_ASSERT 1
#else
# define DEBUG_NEW_CONFIG_HAVE_ASSERT 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_ASSERT */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_LIMITED_API>>
//  - blocks access to all debug_new methods starting with an "_"
//  - makes structures opaque, whose members may be changed / removed / renamed
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#if defined(_ONE_SOURCE) || defined(ONE_SOURCE)
// v always enable limited_api, when compiling for "_ONE_SOURCE",
//   because we will only get 1 chance for loading the *full* header
# define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 1
#else
# define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_LIMITED_API */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS>>
//  - adds some helpful macros, to allocation of memory
//  - These macros are available, even if debug_new is disabled,
//    but they only do simple stuff and don't add any debug
//    information, like:
//    """
//     void some_func(void) {
//      int *x = DEBUG_NEW_API(int);
//      free(x);
//     }
//    """
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && defined(_DEBUG_NEW_ALLOC_HELPERS)
// Support for old debug_new
# define DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS _DEBUG_NEW_ALLOC_HELPERS
#elif 1
# define DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS 1
#else
# define DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO>>
//  - adds debug_new version information vars
//  NOTE: only enabled, when debug_new is enabled, to not
//        work with the risk of it being added to
//        a release-mode application (although, like the license
//        says, an "acknowledgement [...] would be appreciated"...)
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO
#if DEBUG_NEW_CONFIG_ENABLED
# define DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO 1
#else /* DEBUG_NEW_CONFIG_ENABLED */
# define DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO 0
#endif /* !DEBUG_NEW_CONFIG_ENABLED */
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL>>
//  - Enables various experimental debug_new functions
//  - DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL == 0: disabled
//  - DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL == 1: enabled
//  - DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL == 2: enabled in tested environments
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL
#if 1
# define DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL 2
#elif 1
# define DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL 1
#else
# define DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL */



//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE>>
//  - Moves most things associated with
//    debug_new into a designated namespace
//  - NOTE: Requires c++ compiler
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE
#if defined(__cplusplus)
# define DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE 1
#else
# define DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE 0
#endif
#endif /* !DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE */



#ifndef DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT
# define DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT    debug_new
#endif /* !DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT */
#ifndef DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL
# define DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL  detail
#endif /* !DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL */
#if DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE
# define DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT   namespace DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT {
# define DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT   }
# define DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL namespace DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL {
# define DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL }
# define DEBUG_NEW_CXX_NAMESPACE_ROOT         ::DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT::
# define DEBUG_NEW_CXX_NAMESPACE_DETAIL       DEBUG_NEW_CXX_NAMESPACE_ROOT DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL::
#else /* DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE */
# define DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT   /* nothing */
# define DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT   /* nothing */
# define DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL /* nothing */
# define DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL /* nothing */
# define DEBUG_NEW_CXX_NAMESPACE_ROOT         /* nothing */
# define DEBUG_NEW_CXX_NAMESPACE_DETAIL       /* nothing */
#endif /* !DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE */



/* Platform/Compiler dependent configurations */
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS 1
#endif /* !_CRT_SECURE_NO_WARNINGS */
#endif /* _MSC_VER */
#ifndef DEBUG_NEW_COMPILER_HAVE_CXX11_NOEXCEPT
#if defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS) && \
    (__has_feature(cxx_noexcept) || \
    (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46) || \
    (defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 190023026)))
# define DEBUG_NEW_COMPILER_HAVE_CXX11_NOEXCEPT 1
#else
# define DEBUG_NEW_COMPILER_HAVE_CXX11_NOEXCEPT 0
#endif
#endif /* !DEBUG_NEW_COMPILER_HAVE_CXX11_NOEXCEPT */
#ifndef DEBUG_NEW_COMPILER_HAVE_WCHAR_T
#if defined(_NATIVE_WCHAR_T_DEFINED) || defined(__GNUC__)
# define DEBUG_NEW_COMPILER_HAVE_WCHAR_T 1
#else
# define DEBUG_NEW_COMPILER_HAVE_WCHAR_T 0
#endif
#endif

#if defined(WIN32)||defined(_WIN32)||\
    defined(WIN64)||defined(_WIN64)
#define DEBUG_NEW_PLATFORM_WINDOWS 1
#endif

#if defined(linux) || defined(__linux) || defined(__linux__) || \
    defined(__unix__) || defined(__unix) || defined(_POSIX_) || defined(__MACOSX__) || \
   (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE /*&& defined(__LP64__)*/) || \
    defined(__ANDROID__) || defined(__ANDROID) || defined(_ANDROID) || \
    defined(__android__) || defined(__android) || defined(_android)
#define DEBUG_NEW_PLATFORM_POSIX 1
#endif


#ifndef DEBUG_NEW_PLATFORM_64BIT
#if defined(_WIN64)||defined(WIN64)
# define DEBUG_NEW_PLATFORM_64BIT 1
#endif
#endif // !DEBUG_NEW_PLATFORM_64BIT

#ifndef DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE
#if defined(_MSC_VER) || defined(__GNUC__) || defined(__TPP_VERSION__)
# define DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE 1
#else
# define DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE 0
#endif
#endif /* !DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE */

#if !defined(__TINYC__) && !defined(NULL)
#ifdef __cplusplus
# define NULL    0
#else
# define NULL    ((void *)0)
#endif
#endif


#if __has_include(<features.h>) || \
    defined(DEBUG_NEW_PLATFORM_POSIX) ||\
   (defined(__GNUC__) && defined(DEBUG_NEW_PLATFORM_WINDOWS))||\
    defined(HAVE_FEATURES_H) ||\
    defined(HAVE_INCLUDE_FEATURES_H)
#include <features.h>
#endif

//////////////////////////////////////////////////////////////////////////
// TODO: Add support for more standard Libraries here!
#if defined(_MSC_VER)
#define DEBUG_NEW_COMPILER_HAVE_STRDUP              1 /* strdup */
#define DEBUG_NEW_COMPILER_HAVE_STRDUP2             1 /* _strdup */
#define DEBUG_NEW_COMPILER_HAVE_WCSDUP              1 /* wcsdup */
#define DEBUG_NEW_COMPILER_HAVE_WCSDUP2             1 /* _wcsdup */
#define DEBUG_NEW_COMPILER_HAVE_MBSDUP2             1 /* _mbsdup */
#define DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC 1 /* _aligned_malloc, ... */
#else
// Assume stdc standard conforming library
#endif

#if (defined(_SVID_SOURCE) && _SVID_SOURCE) ||\
    (defined(_BSD_SOURCE) && _BSD_SOURCE) ||\
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) ||\
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE &&\
     defined(_XOPEN_SOURCE_EXTENDED) && _XOPEN_SOURCE_EXTENDED) ||\
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) ||\
    (defined(__POSIX_VISIBLE) && __POSIX_VISIBLE >= 200809L) ||\
    (defined(__GNUC__) && defined(DEBUG_NEW_PLATFORM_WINDOWS))
#undef DEBUG_NEW_COMPILER_HAVE_STRDUP
#define DEBUG_NEW_COMPILER_HAVE_STRDUP  1 /* strdup */
#endif

#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) || \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) || \
    (defined(__POSIX_VISIBLE) && __POSIX_VISIBLE >= 200809L) ||\
    (defined(_GNU_SOURCE) && _GNU_SOURCE) ||\
    (defined(__GNUC__) && defined(DEBUG_NEW_PLATFORM_WINDOWS))
#undef DEBUG_NEW_COMPILER_HAVE_WCSDUP
#define DEBUG_NEW_COMPILER_HAVE_WCSDUP  1 /* wcsdup */
#endif

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L) || \
    (defined(__POSIX_VISIBLE) && __POSIX_VISIBLE >= 200809L) || \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700) || \
    (defined(_GNU_SOURCE) && _GNU_SOURCE) ||\
    (defined(__GNUC__) && defined(DEBUG_NEW_PLATFORM_WINDOWS))
#undef DEBUG_NEW_COMPILER_HAVE_STRNDUP
#define DEBUG_NEW_COMPILER_HAVE_STRNDUP 1
#endif

#ifndef DEBUG_NEW_COMPILER_HAVE_STRDUP
#define DEBUG_NEW_COMPILER_HAVE_STRDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP */
#ifndef DEBUG_NEW_COMPILER_HAVE_STRDUP2
#define DEBUG_NEW_COMPILER_HAVE_STRDUP2 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP2 */
#ifndef DEBUG_NEW_COMPILER_HAVE_STRNDUP
#define DEBUG_NEW_COMPILER_HAVE_STRNDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRNDUP */

#ifndef DEBUG_NEW_COMPILER_HAVE_WCSDUP
#define DEBUG_NEW_COMPILER_HAVE_WCSDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSDUP */
#ifndef DEBUG_NEW_COMPILER_HAVE_WCSDUP2
#define DEBUG_NEW_COMPILER_HAVE_WCSDUP2 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSDUP2 */
#ifndef DEBUG_NEW_COMPILER_HAVE_WCSNDUP
#define DEBUG_NEW_COMPILER_HAVE_WCSNDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSNDUP */

#ifndef DEBUG_NEW_COMPILER_HAVE_MBSDUP
#define DEBUG_NEW_COMPILER_HAVE_MBSDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSDUP */
#ifndef DEBUG_NEW_COMPILER_HAVE_MBSDUP2
#define DEBUG_NEW_COMPILER_HAVE_MBSDUP2 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSDUP2 */
#ifndef DEBUG_NEW_COMPILER_HAVE_MBSNDUP
#define DEBUG_NEW_COMPILER_HAVE_MBSNDUP 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSNDUP */

#ifndef DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC
#define DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC 0
#endif /* !DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC */

#ifndef DEBUG_NEW_CXX11_NOEXCEPT
#if defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS)
#if DEBUG_NEW_COMPILER_HAVE_CXX11_NOEXCEPT
# define DEBUG_NEW_CXX11_NOEXCEPT noexcept
#else
# define DEBUG_NEW_CXX11_NOEXCEPT throw()
#endif
#else
# define DEBUG_NEW_CXX11_NOEXCEPT /* nothing */
#endif
#endif /* !DEBUG_NEW_CXX11_NOEXCEPT */

#ifndef DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#if defined(_MSC_VER) || /*defined(__GNUC__) || defined(__clang__) ||*/ \
   (defined(__TPP_VERSION__) && __TPP_VERSION__ == 103 /* pre-__has_feature */) || \
    __has_feature(__tpp_pragma_region__)
# define DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION 1
#else
# define DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION 0
#endif
#endif /* !DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#ifndef DEBUG_NEW_ATTRIBUTE_STATIC_INLINE
#if defined(__cplusplus)
# define DEBUG_NEW_ATTRIBUTE_STATIC_INLINE inline
#elif defined(_MSC_VER)
# define DEBUG_NEW_ATTRIBUTE_STATIC_INLINE __inline
#elif defined(__GNUC__)
# define DEBUG_NEW_ATTRIBUTE_STATIC_INLINE static __inline
#else
# define DEBUG_NEW_ATTRIBUTE_STATIC_INLINE static
#endif
#endif

#if defined(_PREFAST_)
# include <sal.h>
#endif

#ifndef DEBUG_NEW_ATTRIBUTE_NOINLINE
#if __has_declspec_attribute(noinline) || \
   (defined(_MSC_VER) && _MSC_VER >= 1300)
# define DEBUG_NEW_ATTRIBUTE_NOINLINE __declspec(noinline)
#elif __has_attribute(noinline) || \
   (defined(__GNUC__) && ((__GNUC__ > 3) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)))
#if defined(__CUDACC__)
# define DEBUG_NEW_ATTRIBUTE_NOINLINE __attribute__((noinline))
#else
# define DEBUG_NEW_ATTRIBUTE_NOINLINE __attribute__((__noinline__))
#endif
#else
# define DEBUG_NEW_ATTRIBUTE_NOINLINE /* nothing */
#endif
#endif /* !DEBUG_NEW_ATTRIBUTE_NOINLINE */

#ifndef DEBUG_NEW_ATTRIBUTE_NORETURN
#if __has_declspec_attribute(noreturn) || \
   (defined(_MSC_VER) && _MSC_VER >= 1200)
# define DEBUG_NEW_ATTRIBUTE_NORETURN __declspec(noreturn)
#elif __has_attribute(noreturn) || defined(__clang__) || \
  (defined(__GNUC__) && ((__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ >= 5)))
# define DEBUG_NEW_ATTRIBUTE_NORETURN __attribute__((__noreturn__))
#else
# define DEBUG_NEW_ATTRIBUTE_NORETURN /* nothing */
#endif
#endif /* !DEBUG_NEW_ATTRIBUTE_NORETURN */

#ifndef DEBUG_NEW_ATTRIBUTE_MALLOC
#if __has_attribute(malloc) || \
   (defined(__GNUC__) && __GNUC__ >= 3)
# define DEBUG_NEW_ATTRIBUTE_MALLOC __attribute__((__malloc__))
#elif __has_declspec_attribute(restrict) || \
   (defined(_MSC_VER) && _MSC_VER >= 1400)
# define DEBUG_NEW_ATTRIBUTE_MALLOC __declspec(restrict)
#else
# define DEBUG_NEW_ATTRIBUTE_MALLOC
#endif
#endif /* !DEBUG_NEW_ATTRIBUTE_MALLOC */

#ifndef DEBUG_NEW_ATTRIBUTE_USE_RESULT
#if __has_attribute(warn_unused_result) || defined(__GNUC__)
# define DEBUG_NEW_ATTRIBUTE_USE_RESULT __attribute__((__warn_unused_result__))
#elif defined(_Check_return_)
# define DEBUG_NEW_ATTRIBUTE_USE_RESULT _Check_return_
#endif
#ifndef DEBUG_NEW_ATTRIBUTE_USE_RESULT
#define DEBUG_NEW_ATTRIBUTE_USE_RESULT /* nothing */
#endif /* !DEBUG_NEW_ATTRIBUTE_USE_RESULT */
#endif /* !DEBUG_NEW_ATTRIBUTE_USE_RESULT */

// __attribute__((__const__))
// >> The function always does the same, if given the same arguments
// >> Allows the compiler, to remove calls with the same arguments
#ifndef DEBUG_NEW_ATTRIBUTE_CONST
#if __has_declspec_attribute(noalias) || \
   (defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(__clang__))
# define DEBUG_NEW_ATTRIBUTE_CONST __declspec(noalias)
#elif __has_attribute(const) || \
     (defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6)))
# define DEBUG_NEW_ATTRIBUTE_CONST __attribute__((__const__))
#else
# define DEBUG_NEW_ATTRIBUTE_CONST /* nothing */
#endif
#endif /* !DEBUG_NEW_ATTRIBUTE_CONST */

#if __has_feature(cxx_constexpr) || \
   (defined(__cpp_constexpr) && __cpp_constexpr >= 200704) || (defined(__cplusplus) && \
   (defined(__IBMCPP__) && defined(__IBMCPP_CONSTEXPR) && __IBMCPP_CONSTEXPR) || \
   (defined(__SUNPRO_CC) && (__SUNPRO_CC >= 0x5130)) || \
   (defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__) >= 406 && (defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)) || \
   (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026))
# define DEBUG_NEW_CXX11_CONSTEXPR DEBUG_NEW_ATTRIBUTE_CONST constexpr
#else
# define DEBUG_NEW_CXX11_CONSTEXPR DEBUG_NEW_ATTRIBUTE_CONST
#endif

#ifndef DEBUG_NEW_UTIL_IS_2_POW_N
#define DEBUG_NEW_UTIL_IS_2_POW_N(x) (((x)&((x)-1))==0)
#endif /* !DEBUG_NEW_UTIL_IS_2_POW_N */


#ifndef DEBUG_NEW_ANNOTATION_RETURNS
#if defined(_Post_equal_to_)
#define DEBUG_NEW_ANNOTATION_RETURNS    _Post_equal_to_
#else
#define DEBUG_NEW_ANNOTATION_RETURNS(p) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RETURNS */

#ifndef DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT
// The return value of the function *should* be use
//  - There is no reason, to call the function, if you're not going to use the return value
//  - Same implementation as *must* use, but you ignoring it will not break stuff (it would just be stupid!)
#define DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_USE_RESULT
#endif /* !DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT */

#ifndef DEBUG_NEW_ANNOTATION_RETURNS_NONNULL
#if defined(_Ret_notnull_)
# define DEBUG_NEW_ANNOTATION_RETURNS_NONNULL _Ret_notnull_
#elif __has_attribute(returns_nonnull)
# define DEBUG_NEW_ANNOTATION_RETURNS_NONNULL __attribute__((__returns_nonnull__))
#else
# define DEBUG_NEW_ANNOTATION_RETURNS_NONNULL /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RETURNS_NONNULL */

#ifndef DEBUG_NEW_ANNOTATION_SUCCESS
#if defined(_Success_)
# define DEBUG_NEW_ANNOTATION_SUCCESS       _Success_
#else
# define DEBUG_NEW_ANNOTATION_SUCCESS(cond) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_SUCCESS */

#ifndef DEBUG_NEW_ANNOTATION_REQUIRES
#if defined(_Pre_satisfies_)
# define DEBUG_NEW_ANNOTATION_REQUIRES       _Pre_satisfies_
#else
# define DEBUG_NEW_ANNOTATION_REQUIRES(cond) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_REQUIRES */

#ifndef DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL
#if defined(_At_) && defined(_Maybenull_)
# define DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL(x) _At_(x,_Maybenull_)
#else
# define DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL */
#ifndef DEBUG_NEW_ANNOTATION_IN_NEVER_NULL
#if defined(_At_) && defined(_Notnull_)
# define DEBUG_NEW_ANNOTATION_IN_NEVER_NULL(x) _At_(x,_Notnull_)
#else
# define DEBUG_NEW_ANNOTATION_IN_NEVER_NULL(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_NEVER_NULL */

#ifndef DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL
#if defined(_Ret_maybenull_)
# define DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL _Ret_maybenull_
#else
# define DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL */
#ifndef DEBUG_NEW_ANNOTATION_RET_NEVER_NULL
#if defined(_Ret_notnull_)
# define DEBUG_NEW_ANNOTATION_RET_NEVER_NULL _Ret_notnull_
#else
# define DEBUG_NEW_ANNOTATION_RET_NEVER_NULL /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RET_NEVER_NULL */

#ifndef DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO
#if defined(_Pre_satisfies_)
# define DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(x) _Pre_satisfies_(x == 0 || x != 0)
#else
# define DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO */
#ifndef DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO
#if defined(_At_) && defined(_In_range_)
# define DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(x) _At_(x,_In_range_(!=,0))
#else
# define DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO */

#ifndef DEBUG_NEW_ANNOTATION_RET_MAYBE_ZERO
#if defined(_Post_satisfies_)
# define DEBUG_NEW_ANNOTATION_RET_MAYBE_ZERO _Post_satisfies_(return == 0 || return != 0)
#else
# define DEBUG_NEW_ANNOTATION_RET_MAYBE_ZERO /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RET_MAYBE_ZERO */
#ifndef DEBUG_NEW_ANNOTATION_RET_NEVER_ZERO
#if defined(_Ret_range_)
# define DEBUG_NEW_ANNOTATION_RET_NEVER_ZERO _Ret_range_(!=,0)
#else
# define DEBUG_NEW_ANNOTATION_RET_NEVER_ZERO /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_RET_NEVER_ZERO */




#ifndef DEBUG_NEW_ANNOTATION_IN_CSTR_OPT
#if defined(_At_) && defined(_In_opt_z_)
# define DEBUG_NEW_ANNOTATION_IN_CSTR_OPT(x) _At_(x,_In_opt_z_)
#else
# define DEBUG_NEW_ANNOTATION_IN_CSTR_OPT(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_CSTR_OPT */

#ifndef DEBUG_NEW_ANNOTATION_IN_CSTR
#if defined(_At_) && defined(_In_z_)
# define DEBUG_NEW_ANNOTATION_IN_CSTR(x) _At_(x,_In_z_)
#else
# define DEBUG_NEW_ANNOTATION_IN_CSTR(x) /* nothing */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_IN_CSTR */


#ifndef DEBUG_NEW_ANNOTATION_PRINTF_STR
#if defined(_Printf_format_string_)
# define DEBUG_NEW_ANNOTATION_PRINTF_STR _Printf_format_string_
#else
# define DEBUG_NEW_ANNOTATION_PRINTF_STR /* printf-style string */
#endif
#endif /* !DEBUG_NEW_ANNOTATION_PRINTF_STR */

#if defined(_Post_writable_byte_size_)
# define DEBUG_NEW_PRIVATE_ANNOTATION_MALLOC_WRITEABLE_MEMORY    _Post_writable_byte_size_
#else
# define DEBUG_NEW_PRIVATE_ANNOTATION_MALLOC_WRITEABLE_MEMORY(s) /* nothing */
#endif

#ifndef DEBUG_NEW_ANNOTATION_MALLOC
#define DEBUG_NEW_ANNOTATION_MALLOC_BASE(s) \
 DEBUG_NEW_ANNOTATION_SUCCESS(return != NULL) \
 DEBUG_NEW_PRIVATE_ANNOTATION_MALLOC_WRITEABLE_MEMORY(s) \
 DEBUG_NEW_ATTRIBUTE_USE_RESULT DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL \
 DEBUG_NEW_ATTRIBUTE_MALLOC 
#define DEBUG_NEW_ANNOTATION_MALLOC(s)    DEBUG_NEW_ANNOTATION_MALLOC_BASE(s) DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(s)
#define DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) DEBUG_NEW_ANNOTATION_MALLOC_BASE(s) DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(s)
#endif /* !DEBUG_NEW_ANNOTATION_MALLOC */

#ifndef DEBUG_NEW_ANNOTATION_FREE
#if defined(_At_) && defined(_Post_invalid_)
# define DEBUG_NEW_ANNOTATION_FREE_BASE(p) _At_(p,_Post_invalid_)
#else
# define DEBUG_NEW_ANNOTATION_FREE_BASE(p) /* nothing */
#endif
#define DEBUG_NEW_ANNOTATION_FREE(p)    DEBUG_NEW_ANNOTATION_FREE_BASE(p) DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL(p)
#define DEBUG_NEW_ANNOTATION_FREE_NN(p) DEBUG_NEW_ANNOTATION_FREE_BASE(p) DEBUG_NEW_ANNOTATION_IN_NEVER_NULL(p)
#endif /* !DEBUG_NEW_ANNOTATION_FREE */

#ifndef DEBUG_NEW_ANNOTATION_REALLOC
#define DEBUG_NEW_ANNOTATION_REALLOC(p,s)     DEBUG_NEW_ANNOTATION_FREE(p)    DEBUG_NEW_ANNOTATION_MALLOC(s)
#define DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)  DEBUG_NEW_ANNOTATION_FREE_NN(p) DEBUG_NEW_ANNOTATION_MALLOC(s)
#define DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)  DEBUG_NEW_ANNOTATION_FREE(p)    DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)
#define DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) DEBUG_NEW_ANNOTATION_FREE_NN(p) DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)
#endif /* !DEBUG_NEW_ANNOTATION_REALLOC */

#ifndef DEBUG_NEW_ANNOTATION_CALLOC
#if defined(_At_buffer_)&& defined(_Post_satisfies_)
# define DEBUG_NEW_PRIVATE_ANNOTATION_MEMSET(p,v,s) \
 _At_buffer_((unsigned char*)p,_ms_iter_,s,_Post_satisfies_(((unsigned char*)p)[_ms_iter_] == v))
#else
# define DEBUG_NEW_PRIVATE_ANNOTATION_MEMSET(p,v,s) /* nothing */
#endif
#define DEBUG_NEW_ANNOTATION_CALLOC_BASE(s) DEBUG_NEW_ANNOTATION_MALLOC_BASE(s) DEBUG_NEW_PRIVATE_ANNOTATION_MEMSET(return,0,s)
#define DEBUG_NEW_ANNOTATION_CALLOC(n,s)    DEBUG_NEW_ANNOTATION_CALLOC_BASE((n*s)) DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(n)  DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(s)
#define DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) DEBUG_NEW_ANNOTATION_CALLOC_BASE((n*s)) DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(n)  DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(s)
#endif /* !DEBUG_NEW_ANNOTATION_CALLOC */

#ifndef DEBUG_NEW_ANNOTATION_MSIZE
#define DEBUG_NEW_ANNOTATION_MSIZE_BASE(p) DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT
#define DEBUG_NEW_ANNOTATION_MSIZE(p)      DEBUG_NEW_ANNOTATION_MSIZE_BASE(p) DEBUG_NEW_ANNOTATION_IN_MAYBE_NULL(p) DEBUG_NEW_ANNOTATION_RET_MAYBE_ZERO
#define DEBUG_NEW_ANNOTATION_MSIZE_NN(p)   DEBUG_NEW_ANNOTATION_MSIZE_BASE(p) DEBUG_NEW_ANNOTATION_IN_NEVER_NULL(p) DEBUG_NEW_ANNOTATION_RET_NEVER_ZERO
#endif /* !DEBUG_NEW_ANNOTATION_MSIZE */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)    DEBUG_NEW_ANNOTATION_MALLOC   (s) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#define DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_BASE(s,a,o) DEBUG_NEW_ANNOTATION_REQUIRES(o == 0 || o < s)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)      DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC   (s,a) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_BASE(s,a,o)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)   DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_BASE(s,a,o)
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)    DEBUG_NEW_ANNOTATION_CALLOC_BASE(s) DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(s) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#define DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) DEBUG_NEW_ANNOTATION_CALLOC_BASE(s) DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(s) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_BASE        DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_BASE
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)      DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC   (s,a) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_BASE(s,a,o)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)   DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_BASE(s,a,o)
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_FREE
#define DEBUG_NEW_ANNOTATION_ALIGNED_FREE      DEBUG_NEW_ANNOTATION_FREE
#define DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN   DEBUG_NEW_ANNOTATION_FREE_NN
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_FREE */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a)     DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)    DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)
#define DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a)  DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)
#define DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a)  DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)    DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)
#define DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o)     DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)    DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)  DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)  DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)    DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE
#define DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a)    DEBUG_NEW_ANNOTATION_MSIZE   (p) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#define DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) DEBUG_NEW_ANNOTATION_MSIZE_NN(p) DEBUG_NEW_ANNOTATION_REQUIRES(DEBUG_NEW_UTIL_IS_2_POW_N(a))
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE */

#ifndef DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o)    DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE   (p,a)
#define DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a)
#endif /* !DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE */

#ifndef DEBUG_NEW_ANNOTATION_STRDUP
#define DEBUG_NEW_ANNOTATION_STRDUP(s) DEBUG_NEW_ANNOTATION_IN_CSTR(s) \
 DEBUG_NEW_PRIVATE_ANNOTATION_MALLOC_WRITEABLE_MEMORY(_String_length_(s)) \
 DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL DEBUG_NEW_ATTRIBUTE_USE_RESULT \
 DEBUG_NEW_ATTRIBUTE_MALLOC
#define DEBUG_NEW_ANNOTATION_STRDUP_NZ(s) DEBUG_NEW_ANNOTATION_IN_CSTR(s) \
 DEBUG_NEW_PRIVATE_ANNOTATION_MALLOC_WRITEABLE_MEMORY(_String_length_(s)) \
 DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL DEBUG_NEW_ATTRIBUTE_USE_RESULT \
 DEBUG_NEW_ATTRIBUTE_MALLOC
#endif /* !DEBUG_NEW_ANNOTATION_STRDUP */

#ifndef DEBUG_NEW_ANNOTATION_STRNDUP
#define DEBUG_NEW_ANNOTATION_STRNDUP_BASE \
 DEBUG_NEW_ANNOTATION_RET_MAYBE_NULL \
 DEBUG_NEW_ATTRIBUTE_MALLOC \
 DEBUG_NEW_ATTRIBUTE_USE_RESULT
#define DEBUG_NEW_ANNOTATION_STRNDUP(s,n)    DEBUG_NEW_ANNOTATION_STRNDUP_BASE DEBUG_NEW_ANNOTATION_IN_CSTR(s) DEBUG_NEW_ANNOTATION_IN_MAYBE_ZERO(n)
#define DEBUG_NEW_ANNOTATION_STRNDUP_NZ(s,n) DEBUG_NEW_ANNOTATION_STRNDUP_BASE DEBUG_NEW_ANNOTATION_IN_CSTR(s) DEBUG_NEW_ANNOTATION_IN_NEVER_ZERO(n)
#endif /* !DEBUG_NEW_ANNOTATION_STRNDUP */

#ifndef DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS
#if defined(_MSC_VER) || defined(__GNUC__) || \
    defined(__clang__) || defined(__TPP_VERSION__) || \
    defined(__TINYC__)
    // There should definitely be more here...
# define DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS 1
#else
# define DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS 0
#endif
#endif /* !DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS */

#define DEBUG_NEW_PRIVATE_PP_STR(x) #x
#define DEBUG_NEW_PP_STR(x)     DEBUG_NEW_PRIVATE_PP_STR(x)
#define DEBUG_NEW_PP_LINE_STR() DEBUG_NEW_PP_STR(__LINE__)

#if defined(_MSC_VER) && defined(_PREFAST_)
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH()      __pragma(warning(push))
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(id) __pragma(warning(disable:id))
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()       __pragma(warning(pop))
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME               __assume
#else
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH()      /* nothing */
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(id) /* nothing */
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()       /* nothing */
#endif
#ifndef DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME
#if defined(_MSC_VER)
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME    __noop
#else
# define DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME(x) (void)0
#endif
#endif /* !DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME */

#ifndef DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
#ifndef DEBUG_NEW_PRIVATE_PP_FUNC
#if defined(__INTELLISENSE__) || (defined(__FUNCTION__) && defined(_MSC_VER))
# define DEBUG_NEW_PRIVATE_PP_FUNC      __FUNCTION__
# define DEBUG_NEW_PRIVATE_HAVE_PP_FUNC 1
#else
# define DEBUG_NEW_PRIVATE_PP_FUNC      "??" "?"
# define DEBUG_NEW_PRIVATE_HAVE_PP_FUNC 0
#endif
#else /* !DEBUG_NEW_PRIVATE_PP_FUNC */
# define DEBUG_NEW_PRIVATE_HAVE_PP_FUNC 1
#endif /* DEBUG_NEW_PRIVATE_PP_FUNC */
#endif /* !DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */

//////////////////////////////////////////////////////////////////////////
// Memory manager choosing
//////////////////////////////////////////////////////////////////////////
#ifndef DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC
#if defined(DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL) &&\
   ((DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL == 1) && defined(DEBUG_NEW_PLATFORM_WINDOWS) || \
    (DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL == 2) && defined(DEBUG_NEW_PLATFORM_WINDOWS))
// v experimental win32 HeapAlloc allocator.
# define DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC 1
#endif
#endif /* !DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC */


//////////////////////////////////////////////////////////////////////////
// Fallback: Use stdc memory manager
#if !defined(DEBUG_NEW_CONFIG_USE_C_MALLOC) && \
    !defined(DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC)
#define DEBUG_NEW_CONFIG_USE_C_MALLOC 1
#endif



#if defined(__TINYC__)
// The tinyc preprocessor is a bit picky with this...
// NOTE:
//  - I tried, to use force expansion, but that puts it
//    into an infinite loop (that ends with out-of-memory)
//  - @Creator of tinyc: If you read this, the following crashes your thing!
#if 0
#define DEBUG_NEW_PRIVATE_CAST_ARGS_2(...) (__VA_ARGS__))
#define DEBUG_NEW_PRIVATE_CAST_ARGS(...)   DEBUG_NEW_PRIVATE_CAST_ARGS_2(__VA_ARGS__)
#define DEBUG_NEW_STATIC_CAST(...)         ((__VA_ARGS__)DEBUG_NEW_PRIVATE_CAST_ARGS
int x = DEBUG_NEW_STATIC_CAST(int)(DEBUG_NEW_STATIC_CAST(long)(42));
#endif
# define DEBUG_NEW_STATIC_CAST             /* nothing */
# define DEBUG_NEW_CONST_CAST              /* nothing */
# define DEBUG_NEW_REINTERPRET_CAST        /* nothing */
# define DEBUG_NEW_DYNAMIC_CAST            /* nothing */
# define DEBUG_NEW_LVALUE_CAST(...)       *(__VA_ARGS__*)&
#elif defined(__cplusplus) && DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS
# define DEBUG_NEW_STATIC_CAST(...)      static_cast< __VA_ARGS__ >
# define DEBUG_NEW_CONST_CAST(...)       const_cast< __VA_ARGS__ >
# define DEBUG_NEW_REINTERPRET_CAST(...) reinterpret_cast< __VA_ARGS__ >
# define DEBUG_NEW_DYNAMIC_CAST(...)     dynamic_cast< __VA_ARGS__ >
# define DEBUG_NEW_LVALUE_CAST(...)      reinterpret_cast< __VA_ARGS__ &>
#elif defined(__cplusplus)
# define DEBUG_NEW_STATIC_CAST(T)        static_cast< T >
# define DEBUG_NEW_CONST_CAST(T)         const_cast< T >
# define DEBUG_NEW_REINTERPRET_CAST(T)   reinterpret_cast< T >
# define DEBUG_NEW_DYNAMIC_CAST(T)       dynamic_cast< T >
# define DEBUG_NEW_LVALUE_CAST(T)        reinterpret_cast< T &>
#elif DEBUG_NEW_COMPILER_HAVE_C99_VARIADIC_MACROS
# define DEBUG_NEW_PRIVATE_CAST_ARGS(...)   (__VA_ARGS__))
# define DEBUG_NEW_STATIC_CAST(...)        ((__VA_ARGS__)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_CONST_CAST(...)         ((__VA_ARGS__)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_REINTERPRET_CAST(...)   ((__VA_ARGS__)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_DYNAMIC_CAST(...)       ((__VA_ARGS__)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_LVALUE_CAST(...)       *((__VA_ARGS__*)&DEBUG_NEW_PRIVATE_CAST_ARGS
#else
# define DEBUG_NEW_PRIVATE_CAST_ARGS(x)   (x))
# define DEBUG_NEW_STATIC_CAST(T)        ((T)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_CONST_CAST(T)         ((T)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_REINTERPRET_CAST(T)   ((T)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_DYNAMIC_CAST(T)       ((T)DEBUG_NEW_PRIVATE_CAST_ARGS
# define DEBUG_NEW_LVALUE_CAST(T)       *((T*)&DEBUG_NEW_PRIVATE_CAST_ARGS
#endif

#if defined(__TINYC__)
#define DEBUG_NEW_WTF_RPAREN )
#else
#define DEBUG_NEW_WTF_RPAREN /* nothing */
#endif

#ifndef DEBUG_NEW_UNREFERENCED
#define DEBUG_NEW_UNREFERENCED(x) (void)x
#endif /* !DEBUG_NEW_UNREFERENCED */

#ifndef DEBUG_NEW_CONFIG_DOC_ONLY
#if defined(__INTELLISENSE__) || defined(_PREFAST_)
# define DEBUG_NEW_CONFIG_DOC_ONLY 1
#else
# define DEBUG_NEW_CONFIG_DOC_ONLY 0
#endif
#endif /* !DEBUG_NEW_CONFIG_DOC_ONLY */

#ifndef DEBUG_NEW_BREAKPOINT
#if defined(DEBUG_NEW_PLATFORM_WINDOWS)
#if defined(_MSC_VER)
#ifdef __cplusplus
extern "C" { void __cdecl __debugbreak(void); }
#else /* __cplusplus */
void __cdecl __debugbreak(void);
#endif /* !__cplusplus */
# define DEBUG_NEW_BREAKPOINT __debugbreak
#else
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
# define DEBUG_NEW_BREAKPOINT DebugBreak
#endif
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
# define DEBUG_NEW_BREAKPOINT() ({__asm__ __volatile__("int $3\n\t");(void)0;})
#elif defined(HAVE_SIGNAL_H) || __has_include(<signal.h>)
#ifndef DEBUG_NEW_WITHOUT_HEADERS
#include <signal.h>
#endif /* !DEBUG_NEW_WITHOUT_HEADERS */
#ifdef SIGTRAP
# define DEBUG_NEW_BREAKPOINT() raise(SIGTRAP)
#endif
#endif
#ifndef DEBUG_NEW_BREAKPOINT
#define DEBUG_NEW_BREAKPOINT() DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_BREAKPOINT */
#endif /* !DEBUG_NEW_BREAKPOINT */

//////////////////////////////////////////////////////////////////////////
// Fix for debug_new working with vc++ standard headers.
// Disabled, because it is incompatible with <crtdbg.h> 
// and needs to suppress that header completely
// It is recommended, to disable debug_new temporarily,
// when including standard headers, as that solution
// guaranties platform independence:
// """
// #include <debug_new_disable.inl>
// #include <stdlib.h>
// #include <string.h>
// #include ...
// #include <debug_new_enable.inl>
// """
//////////////////////////////////////////////////////////////////////////
#if 0
// For some reason visual studio only does its #pragma push_macro("xxx")
// magic, if "_CRTDBG_MAP_ALLOC" and "_DEBUG" are defined...
// --> Fix vs standard header working with debug_new by default
#if defined(_MSC_VER) && !defined(_CRTDBG_MAP_ALLOC)
# define _CRTDBG_MAP_ALLOC 1
# ifndef _INC_CRTDBG
#  define _INC_CRTDBG      1 // suppress <crtdbg.h>
# endif /* !_INC_CRTDBG */
#endif /* _MSC_VER && !_CRTDBG_MAP_ALLOC */
#endif


#if DEBUG_NEW_CONFIG_LINK_STATICALLY
// When linking statically, we need to directly implement the limited API
#undef DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 1
#endif /* DEBUG_NEW_CONFIG_LINK_STATICALLY */

#ifndef DEBUG_NEW_ATTRIBUTE_DLLIMPORT
#if defined(DEBUG_NEW_PLATFORM_WINDOWS) || defined(__CYGWIN__)
#if __has_declspec_attribute(dllimport) || defined(_MSC_VER)
#define DEBUG_NEW_ATTRIBUTE_DLLIMPORT __declspec(dllimport)
#define DEBUG_NEW_ATTRIBUTE_DLLEXPORT __declspec(dllexport)
#elif __has_attribute(dllimport) || defined(__TINYC__) || \
     (defined(__GNUC__) && (__GNUC__*100+__GNUC_MINOR__) >= 205)
#define DEBUG_NEW_ATTRIBUTE_DLLIMPORT __attribute__((__dllimport__))
#define DEBUG_NEW_ATTRIBUTE_DLLEXPORT __attribute__((__dllexport__))
#endif
#else
#if __has_attribute(visibility) || \
   (defined(__GNUC__) && __GNUC__ >= 4)
#define DEBUG_NEW_ATTRIBUTE_DLLIMPORT __attribute__((__visibility__("default")))
#define DEBUG_NEW_ATTRIBUTE_DLLEXPORT __attribute__((__visibility__("default")))
#endif
#endif
#ifndef DEBUG_NEW_ATTRIBUTE_DLLIMPORT
#define DEBUG_NEW_ATTRIBUTE_DLLIMPORT /* nothing */
#endif
#ifndef DEBUG_NEW_ATTRIBUTE_DLLEXPORT
#define DEBUG_NEW_ATTRIBUTE_DLLEXPORT /* nothing */
#endif
#endif

#if DEBUG_NEW_CONFIG_LINK_STATICALLY
# define DEBUG_NEW_FUNC_DECL(T)      static T
# define DEBUG_NEW_FUNC_IMPL(T)      static T
# define DEBUG_NEW_DATA_DECL(T,decl) /* nothing */
# define DEBUG_NEW_DATA_IMPL(T,decl) static T decl
#elif defined(DEBUG_NEW_CONFIG_BUILDING_SOURCE)
#ifdef DEBUG_NEW_CONFIG_LINK_DLLEXPORT
# define DEBUG_NEW_FUNC_DECL(T)      extern DEBUG_NEW_ATTRIBUTE_DLLEXPORT T
# define DEBUG_NEW_DATA_DECL(T,decl) extern DEBUG_NEW_ATTRIBUTE_DLLEXPORT T decl
#else
# define DEBUG_NEW_FUNC_DECL(T)      extern T
# define DEBUG_NEW_DATA_DECL(T,decl) extern T decl
#endif
# define DEBUG_NEW_FUNC_IMPL(T)      T
# define DEBUG_NEW_DATA_IMPL(T,decl) T decl
#elif defined(DEBUG_NEW_CONFIG_LINK_DLLIMPORT)
# define DEBUG_NEW_FUNC_DECL(T)      extern DEBUG_NEW_ATTRIBUTE_DLLIMPORT T
# define DEBUG_NEW_DATA_DECL(T,decl) extern DEBUG_NEW_ATTRIBUTE_DLLIMPORT T decl
# define DEBUG_NEW_FUNC_IMPL(T)      static T
# define DEBUG_NEW_DATA_IMPL(T,decl) static T decl
#else
# define DEBUG_NEW_FUNC_DECL(T)      extern T
# define DEBUG_NEW_FUNC_IMPL(T)      static T
# define DEBUG_NEW_DATA_DECL(T,decl) extern T decl
# define DEBUG_NEW_DATA_IMPL(T,decl) static T decl
#endif

#define DEBUG_NEW_VERSION 101
#define DEBUG_NEW_PRIVATE_VERSION_STRING \
 "v" DEBUG_NEW_PP_STR(DEBUG_NEW_VERSION)
/* v copyright string, to be included in the version string */
#define DEBUG_NEW_PRIVATE_VERSION_COPYRIGHT \
 "Copyright (c) 2015-2016 Cyder aka Griefer@Work (https://sourceforge.net/users/grieferatwork)"

#if DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
static char const Dn_VersionStr[] = 
 "Debug_new version " DEBUG_NEW_PRIVATE_VERSION_STRING "\n"
 DEBUG_NEW_PRIVATE_VERSION_COPYRIGHT;
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#endif /* DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO */


//////////////////////////////////////////////////////////////////////////
// Deprecated config macros
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#define _DEBUG_NEW_FASTCFG_ENABLED  DEBUG_NEW_FASTCFG_ENABLED
#define _DEBUG_NEW_FASTCFG_LEAKFIX  DEBUG_NEW_FASTCFG_LEAKFIX
#define _DEBUG_NEW_FASTCFG_DISABLED DEBUG_NEW_FASTCFG_DISABLED
#ifndef _WANT_DEBUG_NEW
#define _WANT_DEBUG_NEW                      DEBUG_NEW_CONFIG_ENABLED
#endif
#define _DEBUG_NEW_AUTO_INIT                 DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#ifndef _DEBUG_NEW_MAIN_HOOK_INIT
#define _DEBUG_NEW_MAIN_HOOK_INIT            DEBUG_NEW_MAIN_HOOK
#endif
#define _DEBUG_NEW_ALIGNMENT                 DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT
#define _DEBUG_NEW_TAILCHECK                 DEBUG_NEW_CONFIG_TAILCHECK_SIZE
#define _DEBUG_NEW_WITH_STATS                DEBUG_NEW_CONFIG_HAVE_STATS
#define _DEBUG_NEW_WANT_ALLOC_ID             DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
#define _DEBUG_NEW_WANT_BREAK_AT_ALLOC_ID    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
#define _DEBUG_NEW_CLEAR_ON_FREE             DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE
#define _DEBUG_NEW_TOO_BIG_ALLOC             DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
#define _DEBUG_NEW_AUTO_GLOBAL_STATE         DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE
#define _DEBUG_NEW_FIX_LEAKS                 DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
#define _DEBUG_NEW_ADVANCED_ALLOC            DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC
#define _DEBUG_NEW_ADVANCED_ALLOC_OWN_ACCESS DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
#define _DEBUG_NEW_OVERWRITE_APIS            DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS
#define _DEBUG_NEW_HAVE_MALLOC_USABLE_SIZE   DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#define _DEBUG_NEW_HAVE__MSIZE               DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#define _DEBUG_NEW_HAVE_MALLOC_SIZE          DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#define _DEBUG_NEW_HAVE___MALLOC_SIZE        DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#define _DEBUG_NEW_ALLOW_FREE_NULL           DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
#define _DEBUG_NEW_ALLOW_REALLOC_NULL        DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL
#define _DEBUG_NEW_FREE_LEAKS_ON_SHUTDOWN    DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN
#define _DEBUG_NEW_HAVE_INIT_QUIT_RECURSION  DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
#define _DEBUG_NEW_LOG_FILENAME              DEBUG_NEW_CONFIG_LOG_FILENAME
#define _DEBUG_NEW_HAVE_LOG                  DEBUG_NEW_CONFIG_HAVE_LOG
#define _DEBUG_DEBUG_NEW                     DEBUG_NEW_CONFIG_HAVE_DEBUG
#define _DEBUG_NEW_LIMITED_API               DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#define _DEBUG_NEW_ALLOC_HELPERS             DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS
#define _DEBUG_NEW_WANT_NAMESPACE            DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE
#define _NAMESPACE_NAME_DEBUG_NEW            DEBUG_NEW_CONFIG_CXX_NAMESPACE_ROOT
#define _NAMESPACE_NAME_DEBUG_NEW_HIDDEN     DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL
#ifndef _DEBUG_NEW_WANT_DEPRECATED
#define _DEBUG_NEW_WANT_DEPRECATED           DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#endif /* !_DEBUG_NEW_WANT_DEPRECATED */
//#define _DEBUG_NEW_MSCV_COMPILETIME_OPTIMIZATIONS
#define _DEBUG_NEW_STATIC_VERINFO            DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO
#define _DEBUG_NEW_EXPERIMENTAL              DEBUG_NEW_CONFIG_HAVE_USES_EXPERIMENTAL
#define _NAMESPACE_ENTER_DEBUG_NEW           DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#define _NAMESPACE_LEAVE_DEBUG_NEW           DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#define _NAMESPACE_ENTER_DEBUG_NEW_HIDDEN    DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL
#define _NAMESPACE_LEAVE_DEBUG_NEW_HIDDEN    DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL
#define _NS_DEBUG_NEW                        DEBUG_NEW_CXX_NAMESPACE_ROOT
#define _NS_DEBUG_NEW_HIDDEN                 DEBUG_NEW_CXX_NAMESPACE_ROOT DEBUG_NEW_CXX_NAMESPACE_DETAIL
#ifndef __FUNC__
#define __FUNC__                             DEBUG_NEW_PRIVATE_PP_FUNC
#endif /* !__FUNC__ */
#ifndef __HAVE_FUNC__
#define __HAVE_FUNC__                        DEBUG_NEW_PRIVATE_PP_FUNC
#endif /* !__HAVE_FUNC__ */
#ifndef _STMT_STR
#define __STMT_STR                           DEBUG_NEW_PRIVATE_PP_STR
#define _STMT_STR                            DEBUG_NEW_PP_STR
#endif
#ifndef _NOTHROW
#define _NOTHROW                             DEBUG_NEW_CXX11_NOEXCEPT
#endif
#ifndef __LINE_STR__
#define __LINE_STR__                         DEBUG_NEW_PP_LINE_STR()
#endif
#ifdef DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS
#define _DEBUG_NEW_OVERWRITE_NO_DEBUG_OPERATOR_APIS \
  DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS
#endif /* DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS */
#define _DEBUG_NEW_VERSION_MAJOR  (DEBUG_NEW_VERSION/100)
#define _DEBUG_NEW_VERSION_MINOR  ((DEBUG_NEW_VERSION%100)/10)
#define _DEBUG_NEW_VERSION_PATCH  (DEBUG_NEW_VERSION%10)
#define _DEBUG_NEW_VERSION_TO_HEX(major,minor,patch) ( \
 (((unsigned int)((unsigned char)(major))) << 16) |    \
 (((unsigned int)((unsigned char)(minor))) <<  8) |    \
 (((unsigned int)((unsigned char)(patch)))      ) )
#define _DEBUG_NEW_VERSION_HEX     _DEBUG_NEW_VERSION_TO_HEX(_DEBUG_NEW_VERSION_MAJOR,_DEBUG_NEW_VERSION_MINOR,_DEBUG_NEW_VERSION_PATCH)
#define _DEBUG_NEW_VERSION_STR     DEBUG_NEW_PRIVATE_VERSION_STRING
#define _DEBUG_NEW_COPYRIGHT       DEBUG_NEW_PRIVATE_VERSION_COPYRIGHT
#if DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
typedef struct Dn_Version_t {
 unsigned char v_major; /*< major version. */
 unsigned char v_minor; /*< minor version. */
 unsigned char v_patch; /*< patch version. */
} Dn_Version_t;
static const Dn_Version_t Dn_Version = {
 _DEBUG_NEW_VERSION_MAJOR,
 _DEBUG_NEW_VERSION_MINOR,
 _DEBUG_NEW_VERSION_PATCH};
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#endif /* DEBUG_NEW_CONFIG_HAVE_STATIC_VERINFO */
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE
#pragma once
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE */
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !_DEBUG_NEW_CONF_H */
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !_DEBUG_NEW_CONF_INL */
#endif /* !GUARD_DEBUG_NEW_CONF_INL */
