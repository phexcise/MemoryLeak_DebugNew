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
#ifndef GUARD_DEBUG_NEW_H
#define GUARD_DEBUG_NEW_H 1

#include "debug_new_conf.inl"
#if !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !defined(_DEBUG_NEW_H)
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !defined(_DEBUG_NEW_H)
#define _DEBUG_NEW_H 1
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !_DEBUG_NEW_H */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE
#pragma once
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE */

#if 0
#undef placement
#undef new
#undef malloc
#undef realloc
#undef calloc
#undef free
#endif

//////////////////////////////////////////////////////////////////////////
// Version history
//////////////////////////////////////////////////////////////////////////
// ::debug_new v102:: (deemon prerelease version)
//   - Fixed a few bugs in the detection of native library support for 'strdup' and 'strndup'
// ::debug_new v101:: (deemon prerelease version)
//   - Changed argument requirements for strdup to always return non-NULL argument
//     - Similar to how 'strlen' would work, 'strdup(NULL)' is just as illegal as 'strdup_nz(NULL)' was
//     - Now the only thing different between 'strdup' and 'strdup_nz' is that 'strdup_nz' requires a non-empty string,
//       while 'strdup' returns NULL in the case of 'strdup("")'. And 'strdup_nz("")' causes undefined behavior.
//   - Refactored a couple more identifiers I missed last time
//   - Fixed identifier name references on some of the documentation macros.
//   - Added aligned memory apis
//   - Fixed a bug that may have caused debug_new to link
//     incorrectly before being disabled+enabled once
//   - Fixed compatibility with older c compilers
//   - Fixed a bug in 'Dn_Init()' / 'Dn_Quit()' macro definitions
//     that would have caused the user to possible have to 
//     write 'Dn_Init()()' and 'Dn_Quit()()'
//   - Fixed a bug where 'realloc(p,0)' didn't act like 'free(p)'
//   - _OUT_OF_MEMORY is now deprecated
//     - As a consequence, DEBUG_NEW_API_GETMEM and DEBUG_NEW_API_REALLOCMEM are deprecated as well.
// ::debug_new v100::
//   - First official, stable release
//   - Major refactoring, to get identifiers standard conforming
//     - __debug_*     --> DEBUG_NEW_API_*
//     - _DEBUG_NEW_*  --> DEBUG_NEW_CONFIG_* / DEBUG_NEW_CONFIG_HAVE_*
//     - __malloc_size --> malloc_msize
//     - malloc_ex     --> mallocf
//   - debug_new can now be linked statically (s.a.: DEBUG_NEW_CONFIG_LINK_STATICALLY)
//   - Removed dependency on _OUT_OF_MEMORY() from <debug_new.c>
//   - Added optional, optimized allocation functions (malloc_nz and friends)
//   - Full support for all [_][str/wcs/mbs][n]dup functions
//   - Allocation functions are inlined in release mode, to maximize performance
//   - Deprecated compatibility with all the old identifiers
//     (disabled by default; lets leave the past behind us!)
//   - Cleaned up debug_new linkage modes:
//     - with    DEBUG_NEW_CONFIG_ENABLED: linked against debug_new (<debug_new_enable.inl>) / stdc (<debug_new_disable.inl>) api
//     - without DEBUG_NEW_CONFIG_ENABLED: linked against native    (<debug_new_enable.inl>) / stdc (<debug_new_disable.inl>) api
//   - Added allocation Ids and the ability, to set a breakpoint for a specific allocation
//   - Added support for more __declspec and __attribute__
//   - Added a test script for unix
//   - Removed __realloc_new
//////////////////////////////////////////////////////////////////////////
// ::ancient beta::
//   - See archives on SourceForge
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// debug_new linkage APIs
//  - debug_new : The debug-api this library was built for
//  - stdc :      The standard C api (<stdlib.h> and friends)
//  - native :    The platform-specific native API (fallback for this is the stdc api)
//                This is also, where optimizations using malloc_nz can make a difference!
//                Currently, the WIN32 HeapAlloc() apis are supported,
//                to provide special optimizations
//////////////////////////////////////////////////////////////////////////

#define DEBUG_NEW_PRIVATE_HEADER_NEED_STDINT_H 1 /* <stdint.h> : uintptr_t, ... */
#define DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H 1 /* <stdlib.h> : malloc, ... */
#ifdef __cplusplus
#define DEBUG_NEW_PRIVATE_HEADER_NEED_CXX_NEW  1 /* <new> : c++ placement-new operators */
#endif /* __cplusplus */

#if DEBUG_NEW_CONFIG_ENABLED
#define DEBUG_NEW_PRIVATE_HEADER_NEED_STDDEF_H 1 /* <stddef.h> : NULL */
#define DEBUG_NEW_PRIVATE_HEADER_NEED_STDARG_H 1 /* <stdarg.h> : va_list */
#endif /* DEBUG_NEW_CONFIG_ENABLED */

// --- TODO ---
//  - no-report mode, where debug_new is basically just a
//    runtime engine for fixing memory-leaks.
//  - add support for "atos" / "addr2line", to figure out the file and line
//    for those places where I can't use macros (like operator delete)
//    NOTE: also add a compiler switch to always use this instead of macros,
//          because it reduces the application size and could make debug_new
//          potentially run faster (by not having to include access information
//          in almost every call...)!


//////////////////////////////////////////////////////////////////////////
// Native implementations of debug_new functions / extensions

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Native memory API Hooks
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if defined(DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H 1    /* <Windows.h> : ... */
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#if DEBUG_NEW_CONFIG_LINK_STATICALLY
DEBUG_NEW_DATA_IMPL(void *,DnPrivateGlobal_WinProcessHeap = NULL);
#else /* DEBUG_NEW_CONFIG_LINK_STATICALLY */
DEBUG_NEW_DATA_DECL(void *,DnPrivateGlobal_WinProcessHeap);
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
# define DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s)       HeapAlloc((HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,0,s)
# define DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL(n,s)     HeapAlloc((HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,HEAP_ZERO_MEMORY,(n)*(s))
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s)   HeapReAlloc((HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,0,p,s)
# define DEBUG_NEW_API_NATIVE_FREE_NN_IMPL(p)         HeapFree((HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,0,p)
# define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL(p) HeapSize((HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,0,p)
# define DEBUG_NEW_API_NATIVE_INITIALIZE() (DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap = (void *)GetProcessHeap())
//# define DEBUG_NEW_API_NATIVE_FINALIZE() (DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap = NULL)
#elif defined(DEBUG_NEW_CONFIG_USE_C_MALLOC)
// Default implementation of the native c/c++ standard library
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H 1 /* <stdlib.h> : malloc, ... */
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H */
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H 1 /* <string.h> : strdup, strndup */
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H */
# define DEBUG_NEW_API_NATIVE_MALLOC             malloc
# define DEBUG_NEW_API_NATIVE_CALLOC             calloc
# define DEBUG_NEW_API_NATIVE_REALLOC            realloc
# define DEBUG_NEW_API_NATIVE_FREE               free
# if DEBUG_NEW_COMPILER_HAVE_STRDUP2
#  define DEBUG_NEW_API_NATIVE_STRDUP            _strdup
# else /* DEBUG_NEW_COMPILER_HAVE_STRDUP2 */
#  define DEBUG_NEW_API_NATIVE_STRDUP            strdup
# endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP2 */
# if DEBUG_NEW_COMPILER_HAVE_WCSDUP2
#  define DEBUG_NEW_API_NATIVE_WCSDUP            _wcsdup
# else /* DEBUG_NEW_COMPILER_HAVE_WCSDUP2 */
#  define DEBUG_NEW_API_NATIVE_WCSDUP            wcsdup
# endif /* !DEBUG_NEW_COMPILER_HAVE_WCSDUP2 */
# if DEBUG_NEW_COMPILER_HAVE_MBSDUP2
#  define DEBUG_NEW_API_NATIVE_MBSDUP            _mbsdup
# else /* DEBUG_NEW_COMPILER_HAVE_MBSDUP2 */
#  define DEBUG_NEW_API_NATIVE_MBSDUP            mbsdup
# endif /* !DEBUG_NEW_COMPILER_HAVE_MBSDUP2 */
# define DEBUG_NEW_API_NATIVE_STRNDUP            strndup
# define DEBUG_NEW_API_NATIVE_WCSNDUP            wcsndup
# define DEBUG_NEW_API_NATIVE_MBSNDUP            mbsndup
# ifndef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE
#  if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#   define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE(p)  malloc_usable_size(DEBUG_NEW_CONST_CAST(void*)(p))
#  elif DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#   ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H
#   define DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H 1 /* <malloc.h> : _msize */
#   endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H */
#   define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE(p)  _msize(DEBUG_NEW_CONST_CAST(void*)(p))
#  elif DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#   define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE(p)  malloc_size(DEBUG_NEW_CONST_CAST(void*)(p))
#  endif
# endif /* !DEBUG_NEW_API_NATIVE_MALLOC_MSIZE */
#else
# error "Invalid memory manager selected for debug_new"
#endif
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// Automatic malloc_msize configuration
#if (DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE == 2) && \
   (defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE) || \
    defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN) || \
    defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL))
// We can support malloc_msize natively (so we always enable it)
#undef DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#define DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE 1
#elif (DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE == 1 && !DEBUG_NEW_CONFIG_ENABLED) || \
      (DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE != 0)
// "I'm sorry Dave, I'm afraid I can't do that..."
#undef DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#define DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE 0
#elif DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE != 0
#undef DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#define DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE 0
#endif





#ifndef DEBUG_NEW_PRIVATE_EMPTY_EXPR
#define DEBUG_NEW_PRIVATE_EMPTY_EXPR  ((void)0)
#endif /* !DEBUG_NEW_PRIVATE_EMPTY_EXPR */

//////////////////////////////////////////////////////////////////////////
// <<Leaks / Frees explanation>>
//  - Leak: forgot to deallocate a pointer:
//     """
//      void some_func(void) {
//       mstate_push();
//        void *p = malloc(10);
//       mstate_pop(); //< leak: p was not deallocated
//      }
//     """
//  - free: deallocated a pointer from outside the current state:
//     """
//      void some_func(void) {
//       void *p = malloc(10);
//       mstate_push();
//        free(p);
//       mstate_pop(); //< free: p was freed
//      }
//     """
//////////////////////////////////////////////////////////////////////////


#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Platform-specific Mutex / TLS / ThreadID implementations
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_ENABLED
#if !defined(_NO_THREADS) && !defined(NO_THREADS)
#if defined(DEBUG_NEW_PLATFORM_WINDOWS)
# define DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_MUTEX  1
# define DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_THREAD 1
# define DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_TLS    1
#else
# define DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_MUTEX  1
# define DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_THREAD 1
# define DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_TLS    1
#endif
#endif /* !_NO_THREADS && !NO_THREADS */

#ifndef DEBUG_NEW_OS_THREAD_ID_T
#if defined(DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_THREAD)
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
# define DEBUG_NEW_OS_THREAD_ID_T    DWORD
# define DEBUG_NEW_OS_THREAD_SELF    GetCurrentThreadId
#elif defined(DWORD)
# define DEBUG_NEW_OS_THREAD_ID_T    DWORD
#else
# define DEBUG_NEW_OS_THREAD_ID_T    unsigned long /*< "DWORD" is actually "unsigned long" */
#endif
#elif defined(DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_THREAD)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H */
# define DEBUG_NEW_OS_THREAD_ID_T     pthread_t
# if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#  define DEBUG_NEW_OS_THREAD_SELF    pthread_self
# endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#else
# define DEBUG_NEW_OS_THREAD_ID_T    int
# if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#  define DEBUG_NEW_OS_THREAD_SELF() 0
# endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#endif
#endif /* !DEBUG_NEW_OS_THREAD_ID_T */

// ** mutex and tls API is only available when the limited api is requested
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#ifndef DEBUG_NEW_OS_TLS_T
#if defined(DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_TLS)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
# define DEBUG_NEW_OS_TLS_T                DWORD
# define DEBUG_NEW_OS_TLS_INIT(self)       (((self)=TlsAlloc())!=TLS_OUT_OF_INDEXES)
# define DEBUG_NEW_OS_TLS_QUIT             TlsFree
# define DEBUG_NEW_OS_TLS_GET_FAST(self,v) (*((void **)(&(v))) = TlsGetValue((self)))
# define DEBUG_NEW_OS_TLS_GET(self,v)      ((DEBUG_NEW_OS_TLS_GET_FAST(self,v) != 0) || (GetLastError() == ERROR_SUCCESS))
# define DEBUG_NEW_OS_TLS_SET(self,v)      (TlsSetValue((self),(void *)(v)))
#elif defined(DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_TLS)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H */
# define DEBUG_NEW_OS_TLS_T                pthread_key_t
# define DEBUG_NEW_OS_TLS_INIT(self)       (pthread_key_create(&(self),0) == 0)
# define DEBUG_NEW_OS_TLS_QUIT(self)       (pthread_key_delete(self) == 0)
# define DEBUG_NEW_OS_TLS_GET_FAST(self,v) (*((void **)(&(v))) = pthread_getspecific((self)))
# define DEBUG_NEW_OS_TLS_GET(self,v)      (DEBUG_NEW_OS_TLS_GET_FAST(self,v),1)
# define DEBUG_NEW_OS_TLS_SET(self,v)      (pthread_setspecific((self),(void *)(v)) == 0)
#else
# define DEBUG_NEW_OS_TLS_T                void *
# define DEBUG_NEW_OS_TLS_INIT(self)       (((self) = 0),1)
# define DEBUG_NEW_OS_TLS_QUIT(self)       (1)
# define DEBUG_NEW_OS_TLS_GET_FAST(self,v) (*((void **)(&(v))) = (self),1)
# define DEBUG_NEW_OS_TLS_GET(self,v)      (DEBUG_NEW_OS_TLS_GET_FAST(self,v),1)
# define DEBUG_NEW_OS_TLS_SET(self,v)      (((self) = (void *)(v)),1)
#endif
#endif /* !DEBUG_NEW_OS_TLS_T */
#ifndef DEBUG_NEW_OS_MUTEX_T
#if defined(DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_MUTEX)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
# define DEBUG_NEW_OS_MUTEX_T                 RTL_CRITICAL_SECTION
# define DEBUG_NEW_OS_MUTEX_INIT(m)           do{InitializeCriticalSection((LPCRITICAL_SECTION)&(m));}while(0)
# define DEBUG_NEW_OS_MUTEX_QUIT(m)           do{DeleteCriticalSection((LPCRITICAL_SECTION)&(m));}while(0)
# define DEBUG_NEW_OS_MUTEX_ENTER(m)          (EnterCriticalSection((LPCRITICAL_SECTION)&(m)),1)
# define DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(m)  EnterCriticalSection((LPCRITICAL_SECTION)&(m))
# define DEBUG_NEW_OS_MUTEX_LEAVE(m)          (LeaveCriticalSection((LPCRITICAL_SECTION)&(m)),1)
# define DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(m)  LeaveCriticalSection((LPCRITICAL_SECTION)&(m))
# define DEBUG_NEW_OS_MUTEX_TRY_ENTER(m)      (TryEnterCriticalSection((LPCRITICAL_SECTION)&(m)) != FALSE)
#elif defined(DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_MUTEX)
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H 1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H */
# ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H
# define DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H   1
# endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H */
# define DEBUG_NEW_OS_MUTEX_T        pthread_mutex_t
# define DEBUG_NEW_OS_MUTEX_INIT(m)\
do{\
 pthread_mutexattr_t attr;\
 pthread_mutexattr_init(&attr);\
 pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);\
 pthread_mutex_init(&(m),&attr);\
 pthread_mutexattr_destroy(&attr);\
}while(0)
# define DEBUG_NEW_OS_MUTEX_QUIT(m)           do{pthread_mutex_destroy(&(m));}while(0)
# define DEBUG_NEW_OS_MUTEX_ENTER(m)          (pthread_mutex_lock(&(m)) >= 0)
# define DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(m) (void)pthread_mutex_lock(&(m))
# define DEBUG_NEW_OS_MUTEX_LEAVE(m)          (pthread_mutex_unlock(&(m)) >= 0)
# define DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(m) (void)pthread_mutex_unlock(&(m))
# define DEBUG_NEW_OS_MUTEX_TRY_ENTER(m)      (pthread_mutex_trylock(&(m)) == 0)
#else
# define DEBUG_NEW_OS_MUTEX_INIT(m)           do{}while(0)
# define DEBUG_NEW_OS_MUTEX_QUIT(m)           do{}while(0)
# define DEBUG_NEW_OS_MUTEX_ENTER(m)          1
# define DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(m) (void)0
# define DEBUG_NEW_OS_MUTEX_LEAVE(m)          1
# define DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(m) (void)0
# define DEBUG_NEW_OS_MUTEX_TRY_ENTER(m)      1
#endif
#endif /* !DEBUG_NEW_OS_MUTEX_T */
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

#undef DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_THREAD
#undef DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_MUTEX
#undef DEBUG_NEW_CONFIG_PLATFORM_USE_PTHREAD_TLS
#undef DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_THREAD
#undef DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_MUTEX
#undef DEBUG_NEW_CONFIG_PLATFORM_USE_WIN32_TLS
#endif /* DEBUG_NEW_CONFIG_ENABLED */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Generic aligned memory API Macros
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#ifndef DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT
// Minimum alignment of pointers returned by aligned_malloc and friends.
#define DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT  sizeof(void *)
#endif /* DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT */

#ifndef DEBUG_NEW_PRIVATE_UTIL_ALIGNED_MALLOC
// Generic aligned memory allocator (without offset)
#define DEBUG_NEW_PRIVATE_UTIL_ALIGNED_MALLOC(result,base_alloc,s,a,header_size,init)\
do{\
 size_t sz_pad,sz_total;\
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) --(a);\
 else (a) = DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1;\
 sz_pad = header_size+(a);\
 sz_total = sz_pad+(s);\
 if ((s) > sz_total) (result) = NULL; /* Integer Overflow. */ else {\
  uintptr_t p = (uintptr_t)base_alloc(sz_total);\
  if (!p) (result) = NULL; /* Not enough memory. */ \
  else init((result)=(void *)((p+sz_pad)&~(a)),(void *)p,s);\
 }\
}while(0)
#endif /* DEBUG_NEW_PRIVATE_UTIL_ALIGNED_MALLOC */
#ifndef DEBUG_NEW_PRIVATE_UTIL_ALIGNED_OFFSET_MALLOC
// Generic aligned memory allocator (with offset)
#define DEBUG_NEW_PRIVATE_UTIL_ALIGNED_OFFSET_MALLOC(result,base_alloc,s,a,o,header_size,init)\
do{\
 size_t sz_pad,sz_total,sz_skip;\
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) --(a);\
 else (a) = DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1;\
 sz_skip = (-(o))&(header_size-1);\
 sz_pad = header_size+sz_skip+(a);\
 sz_total = sz_pad+(s);\
 if ((s) > sz_total) (result) = NULL; /* Integer Overflow. */ else {\
  uintptr_t p = (uintptr_t)base_alloc(sz_total);\
  if (!p) (result) = NULL; /* Not enough memory. */ \
  else init(((uintptr_t)((result)=(void *)(\
   ((p+sz_pad+(o))&~(a))-(o))))-sz_skip,(void *)p,s);\
 }\
}while(0)
#endif /* DEBUG_NEW_PRIVATE_UTIL_ALIGNED_OFFSET_MALLOC */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */



//////////////////////////////////////////////////////////////////////////
// Access generators
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Access generators
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_ASSERT || DEBUG_NEW_CONFIG_ENABLED
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
# define DEBUG_NEW_PRIVATE_MAKE_ACCESS(file,line,expr,func) file "\0" line "\0" expr "\0" func
# define DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS(expr)           __FILE__ "\0" DEBUG_NEW_PP_LINE_STR() "\0" expr "\0" DEBUG_NEW_PRIVATE_PP_FUNC
# define DEBUG_NEW_PRIVATE_EMPTY_ACCESS                     __FILE__ "\0" DEBUG_NEW_PP_LINE_STR() "\0\0" DEBUG_NEW_PRIVATE_PP_FUNC
#else /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
# define DEBUG_NEW_PRIVATE_MAKE_ACCESS(file,line,expr,func) file "\0" line "\0" expr
# define DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS(expr)           __FILE__ "\0" DEBUG_NEW_PP_LINE_STR() "\0" expr
# define DEBUG_NEW_PRIVATE_EMPTY_ACCESS                     __FILE__ "\0" DEBUG_NEW_PP_LINE_STR() "\0"
#endif /* !DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
#else /* DEBUG_NEW_CONFIG_HAVE_ASSERT || DEBUG_NEW_CONFIG_ENABLED */
# define DEBUG_NEW_PRIVATE_MAKE_ACCESS(file,line,expr,func) ""
# define DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS(expr)           ""
# define DEBUG_NEW_PRIVATE_EMPTY_ACCESS                     ""
#endif /* !DEBUG_NEW_CONFIG_HAVE_ASSERT && !DEBUG_NEW_CONFIG_ENABLED */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// Assertion macros
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Assertion Macros
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_ASSERT
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
DEBUG_NEW_FUNC_DECL(void) Dn_ReportAssertionFailed(char const *ac);
#ifdef __cplusplus
}
#endif /* __cplusplus */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
# define DN_ASSERT(cond)      (!(cond)?(DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_ReportAssertionFailed(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS(#cond)),DEBUG_NEW_BREAKPOINT(),0):1)
# define DN_ASSERTE(cond,...) do{if(!DN_ASSERT(cond))do __VA_ARGS__ while(0);}while(0)
#else /* DEBUG_NEW_CONFIG_HAVE_ASSERT */
# define DN_ASSERT(cond)      1
# define DN_ASSERTE(cond,...) do{}while(0)
#endif /* !DEBUG_NEW_CONFIG_HAVE_ASSERT */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


#ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H
#define DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H 1 /* <stdlib.h> : size_t */
#endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H */
#ifndef DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H
#define DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H 1 /* <string.h> : strlen, memcpy */
#endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H */
//#if !DEBUG_NEW_COMPILER_HAVE_STRDUP || !DEBUG_NEW_COMPILER_HAVE_STRDUP2
//#endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP || !DEBUG_NEW_COMPILER_HAVE_STRDUP2 */


// Fix placement new, when used with SAL
#if defined(_PREFAST_)
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE
#include <xstddef>
#include <sal.h>
inline _Post_equal_to_(_Where) void *__CRTDECL operator new(size_t, void *_Where) _THROW0() { return (_Where); }
inline void __CRTDECL operator delete(void *, void *) _THROW0() {}
#endif /* __PLACEMENT_NEW_INLINE */
#endif /* _PREFAST_ */


//////////////////////////////////////////////////////////////////////////
// #include required headers
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Inclusion of required headers
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#ifndef DEBUG_NEW_WITHOUT_HEADERS
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_CXX_NEW) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_CXX_NEW
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_CXX_NEW
#include <new>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_CXX_NEW */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H
#include <stdlib.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_STDLIB_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H
#include <string.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_STRING_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_STDARG_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_STDARG_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_STDARG_H
#include <stdarg.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_STDARG_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_STDDEF_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_STDDEF_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_STDDEF_H
#include <stddef.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_STDDEF_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_STDINT_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_STDINT_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_STDINT_H
#include <stdint.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_STDINT_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4820)
#endif /* _MSC_VER */
#include <malloc.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_MALLOC_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4820) /* 'xxx' : 'n' bytes sz_pad added after data member 'xxx::yyy' */
#pragma warning(disable:4668) /* 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */
#pragma warning(disable:4255) /* 'xxx' : no function prototype given: converting '()' to '(void)' */
#endif /* _MSC_VER */
#include <Windows.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H
#include <pthread.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_PTHREAD_H */
#if defined(DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H) && \
            DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H
#include <errno.h>
#endif /* DEBUG_NEW_PRIVATE_HEADER_NEED_ERRNO_H */
#endif /* !DEBUG_NEW_WITHOUT_HEADERS */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Definition of the used wchar_t type
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#if DEBUG_NEW_COMPILER_HAVE_WCHAR_T
typedef wchar_t DnWchar_t;
#elif defined(DEBUG_NEW_PLATFORM_WINDOWS)
typedef unsigned short DnWchar_t;
#elif 1
typedef unsigned int DnWchar_t;
#else
typedef unsigned long DnWchar_t;
#endif
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// Implement missing api functions / native api extensions
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) /* conditional expression is constant */
#endif
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Implementation of strnlen Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#define DEBUG_NEW_PRIVATE_MAKE_STRNLEN_FUNC(name,T)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE \
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t name(T const *s, size_t n) {\
 T const *p = s,*e=s+n;\
 while (p < e && *p) ++p;\
 return DEBUG_NEW_STATIC_CAST(size_t)(p-s);\
}
#define DEBUG_NEW_PRIVATE_MAKE_STRLEN_FUNC(name,T)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE \
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t name(T const *s) {\
 T const *p = s; while (*p) ++p;\
 return DEBUG_NEW_STATIC_CAST(size_t)(p-s);\
}
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
DEBUG_NEW_PRIVATE_MAKE_STRNLEN_FUNC(DnPrivate_StrNLen,char)
DEBUG_NEW_PRIVATE_MAKE_STRNLEN_FUNC(DnPrivate_MbsNLen,unsigned char)
DEBUG_NEW_PRIVATE_MAKE_STRNLEN_FUNC(DnPrivate_WcsNLen,DnWchar_t)
DEBUG_NEW_PRIVATE_MAKE_STRLEN_FUNC(DnPrivate_MbsLen,unsigned char)
#ifndef _MSC_VER
DEBUG_NEW_PRIVATE_MAKE_STRLEN_FUNC(DnPrivate_WcsLen,DnWchar_t)
#define DEBUG_NEW_PRIVATE_WCSLEN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_WcsLen
#else /* !_MSC_VER */
#define DEBUG_NEW_PRIVATE_WCSLEN  wcslen
#endif /* _MSC_VER */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#undef DEBUG_NEW_PRIVATE_MAKE_STRLEN_FUNC
#undef DEBUG_NEW_PRIVATE_MAKE_STRNLEN_FUNC
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */



#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Implementation of missing native API Functions (inline)
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#define DEBUG_NEW_PRIVATE_MAKE_STDC_STRNDUP(name,T)\
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH() \
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(6387) \
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRNDUP(s,l) \
T *name(T const *s, size_t l) {\
 size_t sl; T *result;\
 T const *si = s,*se = s+l;\
 DN_ASSERTE((l == 0 || s != NULL) && "Undefined behavior : " #name "(NULL,!= 0)",{return NULL;});\
 while (si != se && *si) ++si;\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  malloc(((sl = DEBUG_NEW_STATIC_CAST(size_t)(si-s))+1)*sizeof(T)));\
 if (result) DEBUG_NEW_REINTERPRET_CAST(T *)(memcpy(result,s,sl*sizeof(T)))[sl] = '\0';\
 return result;\
}\
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()
#define DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(name,T,strlen_func)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRDUP(s) \
T *name(T const *s) {\
 size_t sl; T *result;\
 if (s == NULL) return NULL;\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  malloc((sl = strlen_func(s)+1)*sizeof(T)));\
 if (result) memcpy(result,s,sl*sizeof(T));\
 return result;\
}

#if !DEBUG_NEW_COMPILER_HAVE_STRDUP
//////////////////////////////////////////////////////////////////////////
// [strdup] m-allocates a copy of "s"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(strdup,char,strlen)
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP */

#if !DEBUG_NEW_COMPILER_HAVE_STRDUP2
//////////////////////////////////////////////////////////////////////////
// [_strdup] Alias for "strdup"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(_strdup,char,strlen)
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRDUP2 */

#if !DEBUG_NEW_COMPILER_HAVE_STRNDUP
//////////////////////////////////////////////////////////////////////////
// [strndup] m-allocates a copy of "s" containing at most "l" characters (excluding the terminating \0 char)
DEBUG_NEW_PRIVATE_MAKE_STDC_STRNDUP(strndup,char)
#endif /* !DEBUG_NEW_COMPILER_HAVE_STRNDUP */

#if !DEBUG_NEW_COMPILER_HAVE_WCSDUP
//////////////////////////////////////////////////////////////////////////
// [wcsdup] m-allocates a copy of "s"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(wcsdup,DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t,DEBUG_NEW_PRIVATE_WCSLEN)
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSDUP */

#if !DEBUG_NEW_COMPILER_HAVE_WCSDUP2
//////////////////////////////////////////////////////////////////////////
// [_wcsdup] Alias for "wcsdup"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(_wcsdup,DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t,DEBUG_NEW_PRIVATE_WCSLEN)
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSDUP2 */

#if !DEBUG_NEW_COMPILER_HAVE_WCSNDUP
#undef DEBUG_NEW_COMPILER_HAVE_WCSNDUP
#define DEBUG_NEW_COMPILER_HAVE_WCSNDUP 1
//////////////////////////////////////////////////////////////////////////
// [wcsndup] m-allocates a copy of "s" containing at most "l" characters (excluding the terminating \0 char)
DEBUG_NEW_PRIVATE_MAKE_STDC_STRNDUP(wcsndup,DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t)
#endif /* !DEBUG_NEW_COMPILER_HAVE_WCSNDUP */

#if !DEBUG_NEW_COMPILER_HAVE_MBSDUP
#undef DEBUG_NEW_COMPILER_HAVE_MBSDUP
#define DEBUG_NEW_COMPILER_HAVE_MBSDUP 1
//////////////////////////////////////////////////////////////////////////
// [mbsdup] m-allocates a copy of "s"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(mbsdup,unsigned char,DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_MbsLen)
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSDUP */

#if !DEBUG_NEW_COMPILER_HAVE_MBSDUP2
#undef DEBUG_NEW_COMPILER_HAVE_MBSDUP2
#define DEBUG_NEW_COMPILER_HAVE_MBSDUP2 1
//////////////////////////////////////////////////////////////////////////
// [_mbsdup] Alias for "mbsdup"
DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP(_mbsdup,unsigned char,DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_MbsLen)
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSDUP2 */

#if !DEBUG_NEW_COMPILER_HAVE_MBSNDUP
#undef DEBUG_NEW_COMPILER_HAVE_MBSNDUP
#define DEBUG_NEW_COMPILER_HAVE_MBSNDUP 1
//////////////////////////////////////////////////////////////////////////
// [mbsndup] m-allocates a copy of "s" containing at most "l" characters (excluding the terminating \0 char)
DEBUG_NEW_PRIVATE_MAKE_STDC_STRNDUP(mbsndup,unsigned char)
#endif /* !DEBUG_NEW_COMPILER_HAVE_MBSNDUP */

#undef DEBUG_NEW_PRIVATE_MAKE_STDC_STRDUP
#undef DEBUG_NEW_PRIVATE_MAKE_STDC_STRNDUP

//////////////////////////////////////////////////////////////////////////
// Implement missing native-api functions
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT

#ifndef DEBUG_NEW_API_NATIVE_MALLOC_NZ
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL) && DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_API_NATIVE_MALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)
void *DnNative_MallocNZ(size_t s) {
 DN_ASSERTE(s != 0 && "Undefined behavior : malloc_nz(0)",{return NULL;});
 return DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s);
}
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
# define DEBUG_NEW_API_NATIVE_MALLOC_NZ  DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL
#else
# define DEBUG_NEW_API_NATIVE_MALLOC_NZ  DEBUG_NEW_API_NATIVE_MALLOC
#endif
#endif /* !DEBUG_NEW_API_NATIVE_MALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_MALLOC
# define DEBUG_NEW_API_NATIVE_MALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_Malloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_MALLOC(s)
void *DnNative_Malloc(size_t s) {
#ifdef DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL
 return s ? DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s) : NULL;
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ)
 return s ? DEBUG_NEW_API_NATIVE_MALLOC_NZ(s) : NULL;
#else
# error "Insufficient native memory api: No malloc function"
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_MALLOC */

#ifndef DEBUG_NEW_API_NATIVE_CALLOC_NZ
#if defined(DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL) && DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_API_NATIVE_CALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_CallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s)
void *DnNative_CallocNZ(size_t n, size_t s) {
 DN_ASSERTE(n*s != 0 && "Undefined behavior : calloc_nz(n,s) | n*s == 0",{return NULL;});
 return DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL(n,s);
}
#elif defined(DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL)
# define DEBUG_NEW_API_NATIVE_CALLOC_NZ  DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL
#else
# define DEBUG_NEW_API_NATIVE_CALLOC_NZ  DEBUG_NEW_API_NATIVE_CALLOC
#endif
#endif /* !DEBUG_NEW_API_NATIVE_CALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_CALLOC
# define DEBUG_NEW_API_NATIVE_CALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_Calloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_CALLOC(n,s)
void *DnNative_Calloc(size_t n, size_t s) {
#ifdef DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL
 return (n*s) ? DEBUG_NEW_API_NATIVE_CALLOC_NZ_IMPL(n,s) : NULL;
#elif defined(DEBUG_NEW_API_NATIVE_CALLOC_NZ)
 return (n*s) ? DEBUG_NEW_API_NATIVE_CALLOC_NZ(n,s) : NULL;
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC)
 void *result = DEBUG_NEW_API_NATIVE_MALLOC((s *= n));
 if (result) memset(result,0,s);
 return result;
#else
# error "Insufficient native memory api: No calloc function"
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_CALLOC */

#ifndef DEBUG_NEW_API_NATIVE_FREE_NN
#if defined(DEBUG_NEW_API_NATIVE_FREE_NN_IMPL) && DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_API_NATIVE_FREE_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_FreeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_FREE_NN(p)
void DnNative_FreeNN(void *p) {
 DN_ASSERTE(p != NULL && "Undefined behavior : free_nn(NULL)",{return;});
 DEBUG_NEW_API_NATIVE_FREE_NN_IMPL(p);
}
#elif defined(DEBUG_NEW_API_NATIVE_FREE_NN_IMPL)
# define DEBUG_NEW_API_NATIVE_FREE_NN DEBUG_NEW_API_NATIVE_FREE_NN_IMPL
#else
# define DEBUG_NEW_API_NATIVE_FREE_NN DEBUG_NEW_API_NATIVE_FREE
#endif
#endif /* !DEBUG_NEW_API_NATIVE_FREE_NN */

#ifndef DEBUG_NEW_API_NATIVE_FREE
# define DEBUG_NEW_API_NATIVE_FREE  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_Free
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_FREE(p)
void DnNative_Free(void *p) {
#ifdef DEBUG_NEW_API_NATIVE_FREE_NN_IMPL
 if (p) DEBUG_NEW_API_NATIVE_FREE_NN_IMPL(p);
#elif defined(DEBUG_NEW_API_NATIVE_FREE_NN)
 if (p) DEBUG_NEW_API_NATIVE_FREE_NN(p);
#else
# error "Insufficient native memory api: No free function"
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_FREE */

#ifndef DEBUG_NEW_API_NATIVE_REALLOC_NNZ
#if defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL) && DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_ReallocNNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s)
void *DnNative_ReallocNNZ(void *p, size_t s) {
 DN_ASSERTE(s != 0 && "Undefined behavior : realloc_nnz(...,0)",{DEBUG_NEW_API_NATIVE_FREE(p); return NULL;});
 DN_ASSERTE(p != NULL && "Undefined behavior : realloc_nnz(NULL,...)",{return DEBUG_NEW_API_NATIVE_MALLOC_NZ(s);});
 return DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s);
}
#elif defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL)
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ  DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL
#elif defined(DEBUG_NEW_API_NATIVE_REALLOC_NN)
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ  DEBUG_NEW_API_NATIVE_REALLOC_NN
#elif defined(DEBUG_NEW_API_NATIVE_REALLOC_NZ)
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ  DEBUG_NEW_API_NATIVE_REALLOC_NZ
#else
# define DEBUG_NEW_API_NATIVE_REALLOC_NNZ  DEBUG_NEW_API_NATIVE_REALLOC
#endif
#endif /* !DEBUG_NEW_API_NATIVE_REALLOC_NNZ */

#ifndef DEBUG_NEW_API_NATIVE_REALLOC_NN
# define DEBUG_NEW_API_NATIVE_REALLOC_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_ReallocNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)
void *DnNative_ReallocNN(void *p, size_t s) {
 DN_ASSERTE(p != NULL && "Undefined behavior : realloc_nn(NULL,...)",{return DEBUG_NEW_API_NATIVE_MALLOC(s);});
#if defined(DEBUG_NEW_API_NATIVE_FREE_NN_IMPL)
 if (!s) { DEBUG_NEW_API_NATIVE_FREE_NN_IMPL(p); return NULL; }
#else
 if (!s) { DEBUG_NEW_API_NATIVE_FREE_NN(p); return NULL; }
#endif
#if defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL)
 return DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s);
#else
 return DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s);
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_REALLOC_NN */

#ifndef DEBUG_NEW_API_NATIVE_REALLOC_NZ
# define DEBUG_NEW_API_NATIVE_REALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_ReallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)
void *DnNative_ReallocNZ(void *p, size_t s) {
 DN_ASSERTE(s != 0 && "Undefined behavior : realloc_nz(...,0)",{DEBUG_NEW_API_NATIVE_FREE(p); return NULL;});
#if defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL) && defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s);
#elif defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ(s);
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s);
#else
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ(s);
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_REALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_REALLOC
# define DEBUG_NEW_API_NATIVE_REALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_Realloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_REALLOC(p,s)
void *DnNative_Realloc(void *p, size_t s) {
 if (!s) { DEBUG_NEW_API_NATIVE_FREE(p); return NULL; }
#if defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL) && defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s);
#elif defined(DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ_IMPL(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ(s);
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(s);
#else
 return p ? DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s) : DEBUG_NEW_API_NATIVE_MALLOC_NZ(s);
#endif
}
#endif /* !DEBUG_NEW_API_NATIVE_REALLOC */

#ifndef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL) && DEBUG_NEW_CONFIG_HAVE_ASSERT_CONFORMING_MALLOC_NZ
# define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MallocMSizeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_MSIZE_NN(p)
size_t DnNative_MallocMSizeNN(void *p) {
 DN_ASSERTE(p != NULL && "Undefined behavior : malloc_msize_nn(NULL)",{return 0;});
 return DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL(p);
}
#elif defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL)
# define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN  DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL
#endif
#endif /* DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN */

#ifndef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL) || \
    defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN)
# define DEBUG_NEW_API_NATIVE_MALLOC_MSIZE DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MallocMSize
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_MSIZE(p)
size_t DnNative_MallocMSize(void *p) {
#ifdef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL
 return p ? DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL(p) : 0;
#else
 return p ? DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN(p) : 0;
#endif
}
#endif
#endif /* DEBUG_NEW_API_NATIVE_MALLOC_MSIZE */



//////////////////////////////////////////////////////////////////////////
// Native aligned malloc functions
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ        DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedMallocNZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetMallocNZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN          DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedFreeNN
#if defined(DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_IMPL) && \
    defined(DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ_IMPL) && \
    defined(DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN_IMPL)
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)
void *DnNative_AlignedMallocNZ(size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_malloc_nz(s,a) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_malloc_nz(s,a) | s == 0",{return NULL;});
 return DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_IMPL(s,a);
}
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
void *DnNative_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_malloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_malloc_nz(s,a,o) | o != 0 && o >= s",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_offset_malloc_nz(s,a,o) | s == 0",{return NULL;});
 return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ_IMPL(s,a,o);
}
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p)
void DnNative_AlignedFreeNN(void *p) {
 DN_ASSERTE(p != NULL && "Undefined behavior: aligned_free_nn(NULL)",{return;});
 DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN_IMPL(p);
}
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ
#if defined(DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_IMPL)
#define DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ        DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedCallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)
void *DnNative_AlignedCallocNZ(size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_calloc_nz(s,a) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_calloc_nz(s,a) | s == 0",{return NULL;});
 return DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_IMPL(s,a);
}
#endif
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ */
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ
#if defined(DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ_IMPL)
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetCallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
void *DnNative_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_calloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | o != 0 && o >= s",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | s == 0",{return NULL;});
 return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ_IMPL(s,a,o);
}
#endif
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ */
#else
// We need, to implemented our own aligned allocation API (based on the non-aligned API)
// [BEGIN ALIGNED MEMORY API]
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146)
#endif
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN)
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE        sizeof(void*)
#define DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED(resp,basep,s) (((void**)(resp))[-1]=(basep))
#else
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE        (sizeof(void*)+sizeof(size_t))
#define DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED(resp,basep,s)\
do{\
 size_t *loc_resp = (uintptr_t *)(resp);\
 ((void**)loc_resp)[-1] = (basep);\
 loc_resp[-2] = (s);\
}while(0)
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASESIZE(p)  \
 *(size_t *)((((uintptr_t)(p))&~(\
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-1))-sizeof(size_t))
#endif
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p)  \
 *(void **)((((uintptr_t)(p))&~(\
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-1))-\
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE)
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_ALLOC(base_alloc,result,s,a)          DEBUG_NEW_PRIVATE_UTIL_ALIGNED_MALLOC(result,base_alloc,s,a,DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE,DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED)
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_OFFSET_ALLOC(base_alloc,result,s,a,o) DEBUG_NEW_PRIVATE_UTIL_ALIGNED_OFFSET_MALLOC(result,base_alloc,s,a,o,DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE,DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED)
#define DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_CALLOC_BASE(s) DEBUG_NEW_API_NATIVE_CALLOC(1,s)
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)
void *DnNative_AlignedMallocNZ(size_t s, size_t a) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_malloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_malloc_nz(s,a,o) | s == 0",{return NULL;});
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_ALLOC(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL,result,s,a);
#else
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_ALLOC(DEBUG_NEW_API_NATIVE_MALLOC_NZ,result,s,a);
#endif
 return result;
}
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
void *DnNative_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_malloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_malloc_nz(s,a,o) | o != 0 && o >= s",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_offset_malloc_nz(s,a,o) | s == 0",{return NULL;});
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_OFFSET_ALLOC(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL,result,s,a,o);
#else
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_OFFSET_ALLOC(DEBUG_NEW_API_NATIVE_MALLOC_NZ,result,s,a,o);
#endif
 return result;
}
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p)
void DnNative_AlignedFreeNN(void *p) {
 DN_ASSERTE(p != NULL && "Undefined behavior: aligned_free_nn(NULL)",{return;});
 DEBUG_NEW_API_NATIVE_FREE_NN(DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p));
}
#define DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ        DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedCallocNZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetCallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)
void *DnNative_AlignedCallocNZ(size_t s, size_t a) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_calloc_nz(s,a) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_calloc_nz(s,a) | s == 0",{return NULL;});
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_ALLOC(DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_CALLOC_BASE,result,s,a);
 return result;
}
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
void *DnNative_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_calloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | o != 0 && o >= s",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | s == 0",{return NULL;});
 DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_OFFSET_ALLOC(DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_CALLOC_BASE,result,s,a,o);
 return result;
}

#ifdef DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC
// Use the Win32 HeapAlloc function, to optimize reallocation of aligned memory
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetReallocNNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
void *DnNative_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o) {
 uintptr_t old_basep,new_basep,result;
 size_t sz_copy,sz_total,sz_skip,baseoff;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_realloc_nnz(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | p == NULL",{return s ? DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) : NULL;});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | s == 0",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | o != 0 && o >= s",{return p;});
 old_basep = (uintptr_t)DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p);
 if (a > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) --a;
 else a = DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1;
 sz_skip = (-o)&(DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-1);
 baseoff = (uintptr_t)p-old_basep;
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL)
 sz_copy = DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL((void *)old_basep)-baseoff;
#else
 sz_copy = DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN((void *)old_basep)-baseoff;
#endif
 sz_copy = sz_copy < s ? sz_copy : s;
 sz_total = DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+sz_skip+a+s;
 if (s > sz_total) return NULL; /* Integer Overflow. */
 if ((old_basep+a+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+sz_skip) < (uintptr_t)p) {
  goto mn; // Not enough space, to expand
 } else {
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6001)
#endif
  if ((new_basep = (uintptr_t)HeapReAlloc(
   (HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,
   HEAP_REALLOC_IN_PLACE_ONLY,(void *)old_basep,sz_total)) != 0) {
   result = ((new_basep+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+sz_skip+a+o)&~a)-o;
   return memmove((void *)result,(void const *)(new_basep+baseoff),sz_copy);
  } else {
#ifdef _PREFAST_
#pragma warning(pop)
#endif
mn: // malloc_new
   if ((new_basep = 
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
    (uintptr_t)DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(sz_total)
#else
    (uintptr_t)DEBUG_NEW_API_NATIVE_MALLOC_NZ(sz_total)
#endif
    ) == 0) return NULL;
  }
 }
 //if (ptr == ((uintptr_t)p-baseoff)) return p;
 result = ((new_basep+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+sz_skip+a+o)&~a)-o;
 memcpy((void *)result,(void const *)(old_basep+baseoff),sz_copy);
#if defined(DEBUG_NEW_API_NATIVE_FREE_NN_IMPL)
 DEBUG_NEW_API_NATIVE_FREE_NN_IMPL((void *)old_basep);
#else
 DEBUG_NEW_API_NATIVE_FREE_NN((void *)old_basep);
#endif
 DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED(result-sz_skip,(void *)new_basep,s);
 return (void *)result;
}
#define DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedReallocNNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a)
void *DnNative_AlignedReallocNNZ(void *p, size_t s, size_t a) {
 uintptr_t old_basep,new_basep,result;
 size_t sz_copy,sz_total,baseoff;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_realloc_nnz(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_realloc_nnz(p,s,a) | p == NULL",{return s ? DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a) : NULL;});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_realloc_nnz(p,s,a,o) | s == 0",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL;});
 old_basep = (uintptr_t)DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p);
 if (a > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) --a;
 else a = DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1;
 baseoff = (uintptr_t)p-old_basep;
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL)
 sz_copy = DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL((void *)old_basep)-baseoff;
#else
 sz_copy = DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN((void *)old_basep)-baseoff;
#endif
 sz_copy = sz_copy < s ? sz_copy : s;
 sz_total = DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+a+s;
 if (s > sz_total) return NULL; /* Integer Overflow. */
 if ((old_basep+a+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE) < (uintptr_t)p) {
  goto mn; // Not enough space, to expand
 } else {
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6001)
#endif
  if ((new_basep = (uintptr_t)HeapReAlloc(
   (HANDLE)DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_WinProcessHeap,
   HEAP_REALLOC_IN_PLACE_ONLY,(void *)old_basep,sz_total)) != 0) {
   if (old_basep != new_basep) return memmove( // This shouldn't be able to happen, but I can't promise that. So...
    (void *)((new_basep+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+a)&~a),
    (void const *)(new_basep+baseoff),sz_copy);
   return p;
  } else {
#ifdef _PREFAST_
#pragma warning(pop)
#endif
mn: // malloc_new
   if ((new_basep = 
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL)
    (uintptr_t)DEBUG_NEW_API_NATIVE_MALLOC_NZ_IMPL(sz_total)
#else
    (uintptr_t)DEBUG_NEW_API_NATIVE_MALLOC_NZ(sz_total)
#endif
    ) == 0) return NULL;
  }
 }
 //if (ptr == ((uintptr_t)p-baseoff)) return p;
 result = (new_basep+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE+a)&~a;
 memcpy((void *)result,(void const *)(old_basep+baseoff),sz_copy);
#if defined(DEBUG_NEW_API_NATIVE_FREE_NN_IMPL)
 DEBUG_NEW_API_NATIVE_FREE_NN_IMPL((void *)old_basep);
#else
 DEBUG_NEW_API_NATIVE_FREE_NN((void *)old_basep);
#endif
 DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED(result,(void *)new_basep,s);
 return (void *)result;
}
#endif /* DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC */

#ifdef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetMallocMSizeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o)
size_t DnNative_AlignedOffsetMallocMSizeNN(void *p, size_t a, size_t o) {
 size_t sz_head,sz_foot,sz_skip;
 uintptr_t base;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behaviour: aligned_offset_malloc_msize_nn(p,a,o) | a: non-power-of-2",{return 0;});
 DN_ASSERTE(p != NULL && "Undefined behaviour: aligned_offset_malloc_msize_nn(p,a,o) | p == NULL",{return 0;});
 sz_head = (uintptr_t)p-(base = (uintptr_t)DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p));
 sz_skip = (-o)&(DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-1);
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) {
  sz_foot = sz_skip+(a-1)+
   DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-sz_head;
 } else {
  sz_foot = sz_skip+(DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1)+
   DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-sz_head;
 }
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL)
 return DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL((void*)base)-(sz_head+sz_foot);
#else
 return DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN((void*)base)-(sz_head+sz_foot);
#endif
}
#define DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedMallocMSizeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a)
size_t DnNative_AlignedMallocMSizeNN(void *p, size_t a) {
 size_t sz_head,sz_foot;
 uintptr_t base;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behaviour: aligned_malloc_msize_nn(p,a) | a: non-power-of-2",{return 0;});
 DN_ASSERTE(p != NULL && "Undefined behaviour: aligned_malloc_msize_nn(p,a) | p == NULL",{return 0;});
 sz_head = (uintptr_t)p-(base = (uintptr_t)DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR(p));
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) {
  sz_foot = (a-1)+DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-sz_head;
 } else {
  sz_foot = (DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1)+
   DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE-sz_head;
 }
#if defined(DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL)
 return DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN_IMPL((void*)base)-(sz_head+sz_foot);
#else
 return DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN((void*)base)-(sz_head+sz_foot);
#endif
}
#else /* DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN */
// Required mainly, to always get a working realloc!
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetMallocMSizeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o)
size_t DnNative_AlignedOffsetMallocMSizeNN(void *p, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behaviour: aligned_offset_malloc_msize_nn(p,a,o) | a: non-power-of-2",{return 0;});
 DN_ASSERTE(p != NULL && "Undefined behaviour: aligned_offset_malloc_msize_nn(p,a,o) | p == NULL",{return 0;});
 (void)a,(void)o;
 return DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASESIZE(p);
}
#define DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedMallocMSizeNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a)
size_t DnNative_AlignedMallocMSizeNN(void *p, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behaviour: aligned_malloc_msize_nn(p,a) | a: non-power-of-2",{return 0;});
 DN_ASSERTE(p != NULL && "Undefined behaviour: aligned_malloc_msize_nn(p,a) | p == NULL",{return 0;});
 (void)a;
 return DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASESIZE(p);
}
#endif /* !DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN */

#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_CALLOC_BASE
#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_OFFSET_ALLOC
#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_ALLOC
#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASEADDR
#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_BASESIZE
#undef DEBUG_NEW_PRIVATE_API_NATIVE_INIT_ALIGNED
#undef DEBUG_NEW_PRIVATE_API_NATIVE_ALIGNED_HEADER_SIZE
#ifdef _MSC_VER
#pragma warning(pop)
#endif
// [END ALIGNED MEMORY API]
#endif
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC */




// Add missing aligned memory function (based on partial implementation)
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetCallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
void *DnNative_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_calloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | o != 0 && o >= s",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_offset_calloc_nz(s,a,o) | s == 0",{return NULL;});
 result = DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o);
 if (result) memset(result,0,s);
 return result;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ */
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedCallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)
void *DnNative_AlignedCallocNZ(size_t s, size_t a) {
 void *result;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_calloc_nz(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE(s != 0 && "Undefined behaviour: aligned_calloc_nz(s,a,o) | s == 0",{return NULL;});
 result = DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a);
 if (result) memset(result,0,s);
 return result;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedMalloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)
void *DnNative_AlignedMalloc(size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_malloc(s,a) | a: non-power-of-2",{return NULL;});
 return s ? DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a) : 0;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetMalloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)
void *DnNative_AlignedOffsetMalloc(size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_malloc(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_malloc(s,a,o) | o != 0 && o >= s",{return NULL;});
 return s ? DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) : NULL;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedCalloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)
void *DnNative_AlignedCalloc(size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_calloc(s,a) | a: non-power-of-2",{return NULL;});
 return s ? DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a) : NULL;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC */
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetCalloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)
void *DnNative_AlignedOffsetCalloc(size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_calloc(s,a,o) | a: non-power-of-2",{return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behaviour: aligned_offset_calloc(s,a,o) | o != 0 && o >= s",{return NULL;});
 return s ? DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) : NULL;
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_FREE
#define DEBUG_NEW_API_NATIVE_ALIGNED_FREE  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedFree
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)
void DnNative_AlignedFree(void *p) {
 if (p) DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_FREE */
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedMallocMSize
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a)
size_t DnNative_AlignedMallocMSize(void *p, size_t a) {
 return p ? DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN(p,a) : 0;
}
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_FREE */
#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetMallocMSize
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o)
size_t DnNative_AlignedOffsetMallocMSize(void *p, size_t a, size_t o) {
 return p ? DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN(p,a,o) : 0;
}
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_FREE */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetReallocNNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
void *DnNative_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o) {
 void *result; // Realloc implementation using malloc + msize
 size_t sz_old;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_realloc_nnz(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | p == NULL",{return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o);});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | s == 0",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behavior : aligned_offset_realloc_nnz(p,s,a,o) | o != 0 && o >= s",{return p;});
 sz_old = DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN(p,a,o);
 result = DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o);
 if (result) memcpy(result,p,s < sz_old ? s : sz_old),DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p);
 return result;
}
#else /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
# error "Incomplete aligned memory API (Missing realloc functions)"
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedReallocNNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a)
void *DnNative_AlignedReallocNNZ(void *p, size_t s, size_t a) {
 void *result; // Realloc implementation using malloc + msize
 size_t sz_old;
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_realloc_nnz(p,s,a) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_realloc_nnz(p,s,a) | p == NULL",{return DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a);});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_realloc_nnz(p,s,a) | s == 0",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL;});
 sz_old = DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN(p,a);
 result = DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a);
 if (result) memcpy(result,p,s < sz_old ? s : sz_old),DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p);
 return result;
}
#else /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
# error "Incomplete aligned memory API (Missing realloc functions)"
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetReallocNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
void *DnNative_AlignedOffsetReallocNN(void *p, size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_realloc_nn(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_offset_realloc_nn(p,s,a,o) | p == NULL",{return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o);});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behavior : aligned_offset_realloc_nn(p,s,a,o) | o != 0 && o >= s",{return p;});
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL; }
 return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetReallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
void *DnNative_AlignedOffsetReallocNZ(void *p, size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_realloc_nz(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_offset_realloc_nz(p,s,a,o) | s == NULL",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE(p); return NULL;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behavior : aligned_offset_realloc_nz(p,s,a,o) | o != 0 && o >= s",{return p;});
 return p
  ? DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
  : DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedOffsetRealloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
void *DnNative_AlignedOffsetRealloc(void *p, size_t s, size_t a, size_t o) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_offset_realloc(p,s,a,o) | a: non-power-of-2",{return p;});
 DN_ASSERTE((o == 0 || o < s) && "Undefined behavior : aligned_offset_realloc(p,s,a,o) | o != 0 && o >= s",{return p;});
#if 1
 if (!p) return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o);
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL; }
#else
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE(p); return NULL; }
 if (!p) return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o);
#endif
 return DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN
#define DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedReallocNN
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a)
void *DnNative_AlignedReallocNN(void *p, size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_realloc_nn(p,s,a) | a: non-power-of-2",{return p;});
 DN_ASSERTE(p != NULL && "Undefined behavior : aligned_realloc_nn(p,s,a) | p == NULL",{return DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a);});
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL; }
 return DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ
#define DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedReallocNZ
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a)
void *DnNative_AlignedReallocNZ(void *p, size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_realloc_nz(p,s,a) | a: non-power-of-2",{return p;});
 DN_ASSERTE(s != 0 && "Undefined behavior : aligned_realloc_nz(p,s,a) | s == NULL",{DEBUG_NEW_API_NATIVE_ALIGNED_FREE(p); return NULL;});
 return p
  ? DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
  : DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ */

#ifndef DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC
#define DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC  DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_AlignedRealloc
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a)
void *DnNative_AlignedRealloc(void *p, size_t s, size_t a) {
 DN_ASSERTE(DEBUG_NEW_UTIL_IS_2_POW_N(a) && "Undefined behavior: aligned_realloc(p,s,a) | a: non-power-of-2",{return p;});
#if 1
 if (!p) return DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a);
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p); return NULL; }
#else
 if (!s) { DEBUG_NEW_API_NATIVE_ALIGNED_FREE(p); return NULL; }
 if (!p) return DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a);
#endif
 return DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a);
}
#endif /* !DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC */





#define DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNC(name,T,strlen_func,str_name)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRDUP(s) \
T *name(T const *s) {\
 size_t sl; T *result;\
 DN_ASSERTE(s != NULL && "Undefined behavior : " str_name "(NULL)",{return NULL;});\
 if ((sl = strlen_func(s)) == 0) return NULL;\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  DEBUG_NEW_API_NATIVE_MALLOC_NZ((sl+1)*sizeof(T)));\
 if (result) DEBUG_NEW_REINTERPRET_CAST(T *)(memcpy(result,s,sl*sizeof(T)))[sl] = '\0';\
 return result;\
}
#define DEBUG_NEW_PRIVATE_MAKE_STRNDUP_FUNC(name,T,str_name)\
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH() \
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(6387 28182) \
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRNDUP(s,l) \
T *name(T const *s, size_t l) {\
 size_t sl; T *result;\
 T const *si = s,*se = s+l;\
 DN_ASSERTE(s != NULL && "Undefined behavior : " str_name "(NULL,...)",{return NULL;});\
 DN_ASSERTE(l != 0 && "Undefined behavior : " str_name "(...,0)",{return NULL;});\
 if (!l) return NULL;\
 while (si != se && *si) ++si;\
 if (si == s) return NULL;\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  DEBUG_NEW_API_NATIVE_MALLOC_NZ(((sl = DEBUG_NEW_STATIC_CAST(size_t)(si-s))+1)*sizeof(T)));\
 if (result) DEBUG_NEW_REINTERPRET_CAST(T *)(memcpy(result,s,sl*sizeof(T)))[sl] = '\0';\
 return result;\
}\
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()
#define DEBUG_NEW_PRIVATE_MAKE_STRDUP_NZ_FUNC(name,T,strlen_func,str_name)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRDUP_NZ(s) \
T *name(T const *s) {\
 T *result; size_t sl;\
 DN_ASSERTE(s != NULL && "Undefined behavior : " str_name "_nz(NULL) (NULL string)",{return NULL;});\
 DN_ASSERTE(*s != 0 && "Undefined behavior : " str_name "_nz(\"\\0...\") (empty string)",{\
  return DEBUG_NEW_REINTERPRET_CAST(T*)(DEBUG_NEW_API_NATIVE_CALLOC_NZ(1,sizeof(T)));});\
 sl = strlen_func(s)+1;\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  DEBUG_NEW_API_NATIVE_MALLOC_NZ(sl*sizeof(T)));\
 if (result) memcpy(result,s,sl*sizeof(T));\
 return result;\
}
#define DEBUG_NEW_PRIVATE_MAKE_STRNDUP_NZ_FUNC(name,T,str_name)\
DEBUG_NEW_ATTRIBUTE_STATIC_INLINE DEBUG_NEW_ANNOTATION_STRNDUP_NZ(s,l) \
T *name(T const *s, size_t l) {\
 size_t sl; T *result;\
 T const *si = s+1,*se = s+l;\
 DN_ASSERTE(l != 0 && "Undefined behavior : " str_name "_nz(...,0)",{return NULL;});\
 DN_ASSERTE(s != NULL && "Undefined behavior : " str_name "_nz(NULL,...) (NULL string)",{return NULL;});\
 DN_ASSERTE(*s != '\0' && "Undefined behavior : " str_name "_nz(\"\\0...\",...) (empty string)",{\
  return DEBUG_NEW_REINTERPRET_CAST(T*)(DEBUG_NEW_API_NATIVE_CALLOC_NZ(1,sizeof(T)));});\
 while (si != se && *si) ++si;\
 sl = DEBUG_NEW_STATIC_CAST(size_t)(si-s);\
 DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME(\
  sl != (size_t)-1); /*< Don't worry! No overflow to 0 here! */\
 result = DEBUG_NEW_REINTERPRET_CAST(T *)(\
  DEBUG_NEW_API_NATIVE_MALLOC_NZ((sl+1)*sizeof(T)));\
 if (result) DEBUG_NEW_REINTERPRET_CAST(T *)(memcpy(result,s,sl*sizeof(T)))[sl] = 0;\
 return result;\
}

#ifndef DEBUG_NEW_API_NATIVE_STRDUP
#define DEBUG_NEW_API_NATIVE_STRDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_StrDup
DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNC(DnNative_StrDup,char,strlen,"strdup")
#endif /* !DEBUG_NEW_API_NATIVE_STRDUP */
#ifndef DEBUG_NEW_API_NATIVE_STRNDUP
#define DEBUG_NEW_API_NATIVE_STRNDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_StrNDup
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_FUNC(DnNative_StrNDup,char,"strndup")
#endif /* !DEBUG_NEW_API_NATIVE_STRNDUP */
#ifndef DEBUG_NEW_API_NATIVE_STRDUP_NZ
#define DEBUG_NEW_API_NATIVE_STRDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_StrDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRDUP_NZ_FUNC(DnNative_StrDupNZ,char,strlen,"strdup")
#endif /* !DEBUG_NEW_API_NATIVE_STRDUP_NZ */
#ifndef DEBUG_NEW_API_NATIVE_STRNDUP_NZ
#define DEBUG_NEW_API_NATIVE_STRNDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_StrNDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_NZ_FUNC(DnNative_StrNDupNZ,char,"strndup")
#endif /* !DEBUG_NEW_API_NATIVE_STRNDUP_NZ */

#ifndef DEBUG_NEW_API_NATIVE_WCSDUP
#define DEBUG_NEW_API_NATIVE_WCSDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_WcsDup
DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNC(DnNative_WcsDup,DnWchar_t,DEBUG_NEW_PRIVATE_WCSLEN,"wcsdup")
#endif /* !DEBUG_NEW_API_NATIVE_WCSDUP */
#ifndef DEBUG_NEW_API_NATIVE_WCSNDUP
#define DEBUG_NEW_API_NATIVE_WCSNDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_WcsNDup
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_FUNC(DnNative_WcsNDup,DnWchar_t,"wcsndup")
#endif /* !DEBUG_NEW_API_NATIVE_WCSNDUP */
#ifndef DEBUG_NEW_API_NATIVE_WCSDUP_NZ
#define DEBUG_NEW_API_NATIVE_WCSDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_WcsDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRDUP_NZ_FUNC(DnNative_WcsDupNZ,DnWchar_t,DEBUG_NEW_PRIVATE_WCSLEN,"wcsdup")
#endif /* !DEBUG_NEW_API_NATIVE_WCSDUP_NZ */
#ifndef DEBUG_NEW_API_NATIVE_WCSNDUP_NZ
#define DEBUG_NEW_API_NATIVE_WCSNDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_WcsNDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_NZ_FUNC(DnNative_WcsNDupNZ,DnWchar_t,"wcsndup")
#endif /* !DEBUG_NEW_API_NATIVE_WCSNDUP_NZ */

#ifndef DEBUG_NEW_API_NATIVE_MBSDUP
#define DEBUG_NEW_API_NATIVE_MBSDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MbsDup
DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNC(DnNative_MbsDup,unsigned char,DnPrivate_MbsLen,"mbsdup")
#endif /* !DEBUG_NEW_API_NATIVE_MBSDUP */
#ifndef DEBUG_NEW_API_NATIVE_MBSNDUP
#define DEBUG_NEW_API_NATIVE_MBSNDUP DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MbsNDup
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_FUNC(DnNative_MbsNDup,unsigned char,"mbsndup")
#endif /* !DEBUG_NEW_API_NATIVE_MBSNDUP */
#ifndef DEBUG_NEW_API_NATIVE_MBSDUP_NZ
#define DEBUG_NEW_API_NATIVE_MBSDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MbsDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRDUP_NZ_FUNC(DnNative_MbsDupNZ,unsigned char,DnPrivate_MbsLen,"mbsdup")
#endif /* !DEBUG_NEW_API_NATIVE_MBSDUP_NZ */
#ifndef DEBUG_NEW_API_NATIVE_MBSNDUP_NZ
#define DEBUG_NEW_API_NATIVE_MBSNDUP_NZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnNative_MbsNDupNZ
DEBUG_NEW_PRIVATE_MAKE_STRNDUP_NZ_FUNC(DnNative_MbsNDupNZ,unsigned char,"mbsndup")
#endif /* !DEBUG_NEW_API_NATIVE_MBSNDUP_NZ */

#undef DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNC
#undef DEBUG_NEW_PRIVATE_MAKE_STRNDUP_FUNC
#undef DEBUG_NEW_PRIVATE_MAKE_STRDUP_NZ_FUNC
#undef DEBUG_NEW_PRIVATE_MAKE_STRNDUP_NZ_FUNC
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#ifdef __cplusplus
}
#endif /* __cplusplus */
#ifdef _MSC_VER
#pragma warning(pop)
#endif



#if DEBUG_NEW_CONFIG_ENABLED

//////////////////////////////////////////////////////////////////////////
// return address
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Compiler-specific, intrinsic return_address support
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#ifndef DEBUG_NEW_PRIVATE_RETURN_ADDRESS
#if defined(_MSC_VER) && !defined(__clang__) && defined(DEBUG_NEW_PLATFORM_WINDOWS)
//# include <intrin.h> 
// v prevent including the whole <intrin.h>
#ifdef __cplusplus
extern "C" { void *_ReturnAddress(void); }
#define DEBUG_NEW_PRIVATE_RETURN_ADDRESS  ::_ReturnAddress
#else /* __cplusplus */
void *_ReturnAddress(void);
#define DEBUG_NEW_PRIVATE_RETURN_ADDRESS  _ReturnAddress
#endif /* !__cplusplus */
#pragma intrinsic(_ReturnAddress)
#elif defined(__GNUC__)
# define DEBUG_NEW_PRIVATE_RETURN_ADDRESS() __builtin_return_address(0)
#else 
# define DEBUG_NEW_PRIVATE_RETURN_ADDRESS() 0
#endif
#endif /* !DEBUG_NEW_PRIVATE_RETURN_ADDRESS */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */




DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Internal Structure and Macro definitions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
typedef int Dn_AllocType_t;
#define DEBUG_NEW_ALLOC_CALL_TYPE_NONE             0
#define DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC           1
#define DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC          2
#define DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC           3
#define DEBUG_NEW_ALLOC_CALL_TYPE_FREE             4
#ifdef __cplusplus
#define DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW          5
#define DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW    6
#define DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE       7
#define DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE 8
#endif /* __cplusplus */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC   9
#define DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_REALLOC  10
#define DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_CALLOC   11
#define DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE     12
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

#ifndef DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD
#define DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD 1024
#endif /* !DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD */
#ifndef DEBUG_NEW_PRIVATE_FILE_AND_LINE_FORMAT
#define DEBUG_NEW_PRIVATE_FILE_AND_LINE_FORMAT "%s(%s) : "
#endif /* !DEBUG_NEW_PRIVATE_FILE_AND_LINE_FORMAT */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4820)
#endif

// v magic used, to validate pointers
typedef char Dn_Magic_t[4];
static const Dn_Magic_t Dn_Magic = { 'D','B','G','N' }; //< DBGN --> DeBuGNew

//typedef char Dn_Access_t;
typedef struct Dn_StatePtr_t
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
{
 void                    *p_self;    /*< self pointer (to initial block memory). */
 Dn_AllocType_t           p_type;    /*< type of allocation. */
 size_t                   p_size;    /*< size of this allocation. */
 char const              *p_access;  /*< allocation access data. */
 void const              *p_address; /*< allocation code address. */
 DEBUG_NEW_OS_THREAD_ID_T p_thread;  /*< thread of this allocation. */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
 char                    *p_alloc_info;
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
 size_t                   p_alloc_id;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
Dn_StatePtr_t;

typedef struct Dn_MemPtr_t 
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
{
 struct Dn_MemPtr_t  *p_global_next; /*< prev global pointer. */
 struct Dn_MemPtr_t  *p_global_prev; /*< next global pointer. */
 struct Dn_MemPtr_t  *p_thread_next; /*< prev thread pointer. */
 struct Dn_MemPtr_t  *p_thread_prev; /*< next thread pointer. */
 struct Dn_StatePtr_t p_state;       /*< state of this pointer. */
 Dn_Magic_t           p_magic;       /*< == Dn_Magic. */
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
Dn_MemPtr_t;

#if DEBUG_NEW_CONFIG_HAVE_STATS
typedef struct Dn_Stat_t {
 /* Do __NOT__ use this structure directly (anymore)!       *
  * Use the macros provided below, to ensure compatibility  *
  * with future version.                                    */
 unsigned int total_calls_malloc;
 unsigned int total_calls_realloc;
 unsigned int total_calls_calloc;
 unsigned int total_calls_free;
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
 unsigned int total_calls_new;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
 unsigned int total_calls_array_new;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
 unsigned int total_calls_delete;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
 unsigned int total_calls_array_delete;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
 unsigned int total_calls_aligned_malloc;
 unsigned int total_calls_aligned_realloc;
 unsigned int total_calls_aligned_calloc;
 unsigned int total_calls_aligned_free;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
 unsigned int total_alloc_blocks;
 unsigned int max_alloc_blocks;
 size_t total_alloc;
 size_t max_alloc;
} Dn_Stat_t;

//////////////////////////////////////////////////////////////////////////
// Debug_New Stat Interface
//////////////////////////////////////////////////////////////////////////
#define DnStat_GetTotalCalls_Malloc(s)  ((unsigned int const)(s)->total_calls_malloc)
#define DnStat_GetTotalCalls_Realloc(s) ((unsigned int const)(s)->total_calls_realloc)
#define DnStat_GetTotalCalls_Calloc(s)  ((unsigned int const)(s)->total_calls_calloc)
#define DnStat_GetTotalCalls_Free(s)    ((unsigned int const)(s)->total_calls_free)
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
# define DnStat_GetTotalCalls_New(s)     ((unsigned int const)(s)->total_calls_new)
#else /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW */
# define DnStat_GetTotalCalls_New(s)     (0)
#endif /* !DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
# define DnStat_GetTotalCalls_ArrayNew(s)  ((unsigned int const)(s)->total_calls_array_new)
#else /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW */
# define DnStat_GetTotalCalls_ArrayNew(s)  (0)
#endif /* !DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
# define DnStat_GetTotalCalls_Delete(s)  ((unsigned int const)(s)->total_calls_delete)
#else /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
# define DnStat_GetTotalCalls_Delete(s)  (0)
#endif /* !DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
# define DnStat_GetTotalCalls_ArrayDelete(s)  ((unsigned int const)(s)->total_calls_array_delete)
#else /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
# define DnStat_GetTotalCalls_ArrayDelete(s)  (0)
#endif /* !DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
// TODO: DnStat_GetTotal...(s)  ...
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#define DnStat_GetTotalAllocBlocks(s)  ((unsigned int const)(s)->total_alloc_blocks)
#define DnStat_GetMaxAllocBlocks(s)    ((unsigned int const)(s)->max_alloc_blocks)
#define DnStat_GetTotalAlloc(s)        ((size_t const)(s)->total_alloc)
#define DnStat_GetMaxAlloc(s)          ((size_t const)(s)->max_alloc)
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define DnStat_GetTotalCalls_AlignedMalloc(s)  ((unsigned int const)(s)->total_calls_aligned_malloc)
#define DnStat_GetTotalCalls_AlignedRealloc(s) ((unsigned int const)(s)->total_calls_aligned_realloc)
#define DnStat_GetTotalCalls_AlignedCalloc(s)  ((unsigned int const)(s)->total_calls_aligned_calloc)
#define DnStat_GetTotalCalls_AlignedFree(s)    ((unsigned int const)(s)->total_calls_aligned_free)
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#else /* DEBUG_NEW_CONFIG_HAVE_STATS */
/* NOTE: Some c compilers include extensions for allowing empty structs (like gcc). *
 *       If your compiler supports this extension, add its define to this list      *
 *       and pm me, so I can add it here!                                           */
#if defined(__cplusplus) || defined(__GNUC__)
typedef struct Dn_Stat_t {} Dn_Stat_t;
#else
typedef struct Dn_Stat_t { char _placeholder; } Dn_Stat_t;
#endif
#define DnStat_GetTotalCalls_Malloc(s)      0
#define DnStat_GetTotalCalls_Realloc(s)     0
#define DnStat_GetTotalCalls_Calloc(s)      0
#define DnStat_GetTotalCalls_Free(s)        0
#define DnStat_GetTotalCalls_New(s)         0
#define DnStat_GetTotalCalls_ArrayNew(s)    0
#define DnStat_GetTotalCalls_Delete(s)      0
#define DnStat_GetTotalCalls_ArrayDelete(s) 0
#define DnStat_GetTotalAllocBlocks(s)       0
#define DnStat_GetMaxAllocBlocks(s)         0
#define DnStat_GetTotalAlloc(s)             0
#define DnStat_GetMaxAlloc(s)               0
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS */
#if !DEBUG_NEW_CONFIG_HAVE_STATS || !DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define DnStat_GetTotalCalls_AlignedMalloc(s)  0
#define DnStat_GetTotalCalls_AlignedRealloc(s) 0
#define DnStat_GetTotalCalls_AlignedCalloc(s)  0
#define DnStat_GetTotalCalls_AlignedFree(s)    0
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS || !DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

typedef struct Dn_State_t 
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
{
 struct Dn_State_t *s_prev;              /*< prev state. */
 char const        *s_access;            /*< access used, to push this state. */
 void const        *s_access_addr;       /*< code address, where this state got created. */
#if DEBUG_NEW_CONFIG_HAVE_STATS
 Dn_Stat_t          s_stats;             /*< stats at the time of this state being pushed. */
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
 size_t             s_npointer_states;   /*< amount of pointer states. */
 Dn_StatePtr_t      s_pointer_states[1]; /*< list of pointer states (len == s_npointer_states). */
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
Dn_State_t;

typedef void (*Dn_PrintFunc_t) (char const *msg);

typedef struct Dn_PrintFuncStack_t
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
{
 struct Dn_PrintFuncStack_t *ps_prev;   /*< [0..1] previous printing function. */
 Dn_PrintFunc_t              ps_func;   /*< [1..1] the actual function. */
 char const                 *ps_access; /*< [1..1] access used, to push this print function. */
 void const                 *ps_addr;   /*< [1..1] code address, where this print function got pushed. */
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
Dn_PrintFuncStack_t;


typedef struct Dn_Thread_t 
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
{
 struct Dn_Thread_t      *t_next;      /*< [0..1] next thread. */
 struct Dn_Thread_t      *t_prev;      /*< [0..1] prev thread. */
 DEBUG_NEW_OS_THREAD_ID_T t_thread_id; /*< thread id. */
 Dn_MemPtr_t             *t_allocs;    /*< [0..1] allocation of this thread. */
 Dn_State_t              *t_state;     /*< [0..1] current state of this thread. */
 Dn_PrintFuncStack_t     *t_print;     /*< [0..1] print function of this thread. */
#if DEBUG_NEW_CONFIG_HAVE_STATS
 Dn_Stat_t                t_stats;     /*< memory stats of this thread */
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
#ifdef DEBUG_NEW_OS_MUTEX_T
 DEBUG_NEW_OS_MUTEX_T     t_lock;      /*< lock for allocs, state and print. */
#endif /* DEBUG_NEW_OS_MUTEX_T */
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
Dn_Thread_t;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// Global vars
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Global variables
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_DATA_DECL(Dn_MemPtr_t *,DnGlobal_AllocChain);  /*< [0..1] global allocation chain (last alloc). */
DEBUG_NEW_DATA_DECL(Dn_State_t *,DnGlobal_GlobalState);  /*< [0..1] global state list. */
DEBUG_NEW_DATA_DECL(Dn_Thread_t *,DnGlobal_Threads);     /*< [0..1] list of known + active threads. */
DEBUG_NEW_DATA_DECL(Dn_PrintFunc_t,DnGlobal_DefaultPrintFunc); /*< [1..1] default function used for printing. */
#ifdef DEBUG_NEW_OS_MUTEX_T 
DEBUG_NEW_DATA_DECL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_AllocChainLock);  /*< lock for DnGlobal_AllocChain and DnGlobal_GlobalState. */
DEBUG_NEW_DATA_DECL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_ThreadsLock);     /*< lock for DnGlobal_Threads. */
DEBUG_NEW_DATA_DECL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_PrintLock);       /*< lock for printing data and DnGlobal_DefaultPrintFunc. */
DEBUG_NEW_DATA_DECL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_GlobalStateLock); /*< lock for the global memory state. */
#endif /* DEBUG_NEW_OS_MUTEX_T */
#ifdef DEBUG_NEW_OS_TLS_T
DEBUG_NEW_DATA_DECL(DEBUG_NEW_OS_TLS_T,DnPrivateGlobal_CurrentThread);
#endif /* DEBUG_NEW_OS_TLS_T */
#if DEBUG_NEW_CONFIG_HAVE_LOG
DEBUG_NEW_DATA_DECL(void *,DnPrivateGlobal_LogFile); /*< [0..1] its actually a "FILE *", but I don't want to include <stdio.h> because of the overhead. */
#endif /* DEBUG_NEW_CONFIG_HAVE_LOG */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
DEBUG_NEW_DATA_DECL(size_t,DnPrivateGlobal_NextAllocID);
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Internal memory state management API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_STATS
//////////////////////////////////////////////////////////////////////////
// Stat helpers
//////////////////////////////////////////////////////////////////////////
// DnStat_Merge: merges "right" into "left":
DEBUG_NEW_FUNC_DECL(void) DnStat_Merge(Dn_Stat_t *left, Dn_Stat_t const *right);
// DnStat_GetGlobal: fills "stats" with current global statistics; "stats" must be assigned
DEBUG_NEW_FUNC_DECL(void) DnStat_GetGlobal(Dn_Stat_t *stats); //< calculate total memory stats
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Stat_GetGlobalAlreadyLocked(Dn_Stat_t *stats);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */

//////////////////////////////////////////////////////////////////////////
// Thread removal (removes the calling thread from the thread info list)
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnThread_Rem(char const *ac, void const *addr); // optional: remove calling thread from the thread list (should be called from thread finalizer)
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_RemAlreadyLocked(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_RemInternal(struct Dn_Thread_t *thread, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_RemInternalAlreadyLocked(struct Dn_Thread_t *thread, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_Free(struct Dn_Thread_t *thread, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) DnThread_Get(void); // get current thread info
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) DnPrivate_Thread_GetAlreadyLocked(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) DnPrivate_Thread_Find(DEBUG_NEW_OS_THREAD_ID_T thread_id);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) DnPrivate_Thread_FindAlreadyLocked(DEBUG_NEW_OS_THREAD_ID_T thread_id);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_PushPointer(struct Dn_Thread_t *thread, struct Dn_MemPtr_t *p);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Thread_PopPointer(struct Dn_MemPtr_t *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Global_PushPointer(struct Dn_MemPtr_t *p);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Global_PopPointer(struct Dn_MemPtr_t *p);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// Print function helpers
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Print function API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t) Dn_GetDefaultPrintFunc(void);
DEBUG_NEW_FUNC_DECL(void) Dn_SetDefaultPrintFunc(Dn_PrintFunc_t func);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t) DnThread_GetPrintFunc(void);
DEBUG_NEW_FUNC_DECL(int) DnThread_PushPrintFunc(Dn_PrintFunc_t func, char const *ac);
DEBUG_NEW_FUNC_DECL(int) DnThread_PopPrintFunc(char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_Thread_PushPrintFunc2(Dn_PrintFunc_t func, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_Thread_PopPrintFunc2(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t) DnPrivate_GetDefaultPrintFuncAlreadyLocked(void);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_SetDefaultPrintFuncAlreadyLocked(Dn_PrintFunc_t func);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_Thread_PopPrintFuncInternal(Dn_Thread_t *thread, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_Thread_PopPrintFuncInternalAlreadyLocked(Dn_Thread_t *thread, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT unsigned int) DnThread_GetPrintFuncStackSize(void);
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// Internal handler methods
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Internal undefined behavior handler API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
// v needed for "operator new" (adds address information)
DEBUG_NEW_FUNC_DECL(void) DnPrivate_ProcessCXXPointer(void *ptr, char const *ac, void const *addr);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleShouldBePlacementCall(void *ptr, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleOutOfMemory(size_t alloc_s, char const *ac, void const *addr);
#if DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleTooBigAlloc(size_t alloc_s, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleNotXXXViolation(char const *call_overview, char const *func_name, char const *alt_name, char const *violation_value, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleStrDupNull(char const *func_name, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleInvalidMagic(Dn_MemPtr_t const *rec, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleUnalignedPointer(void const *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleTailCorruption(Dn_MemPtr_t const *p, char const *ac, void const *addr, void const *corruption_address);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleNoPopState(char const *ac, void const *addr, int is_global);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleNoPopPrintFunc(Dn_Thread_t const *thread, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleInvalidDeallocKind(char const *ac, void const *addr, Dn_MemPtr_t const *p, Dn_AllocType_t should_be, Dn_AllocType_t is);
#if !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleFreeNullPointer(char const *ac, void const *addr);
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_FixMemoryLeak(Dn_MemPtr_t *ptr, char const *ac, void const *addr); //< always available, because used in quit method
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Internal printing helper API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_DefaultPrintFunc_Impl(char const *msg);
//////////////////////////////////////////////////////////////////////////
// Internal dumping methods
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_InvalidMemoryLeak(Dn_StatePtr_t const *state); /*< leak (forgot to delete a pointer). */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_InvalidMemoryFree(Dn_StatePtr_t const *state); /*< free (freeing a var from outside the current thread state). */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_InvalidMemoryLeakAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_InvalidMemoryFreeAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state);
// v compares allocs to state and dumps differences
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_Dump_StateErrors(char const *ac, void const *addr, Dn_MemPtr_t *allocs, Dn_State_t const *state, ptrdiff_t offset_of_prev);
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_Dump_ThreadStateErrors(char const *ac, void const *addr, Dn_MemPtr_t *allocs, Dn_State_t const *state);
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_Dump_GlobalStateErrors(char const *ac, void const *addr, Dn_MemPtr_t *allocs, Dn_State_t const *state);
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_Pointer_FindFutureState(Dn_MemPtr_t const *left, Dn_StatePtr_t const *right, ptrdiff_t offset_of_prev);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

//////////////////////////////////////////////////////////////////////////
// Info dumping methods (call, to list (possibly) useful information)
// HINT: if supported by your debugger, try calling these from a breakpoint
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnPrint_FileAndLine(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrint_Thread(DEBUG_NEW_OS_THREAD_ID_T const thread);
DEBUG_NEW_FUNC_DECL(void) DnPrint_ExprInFunc(char const *ac);
DEBUG_NEW_FUNC_DECL(void) DnPrint_ObjectAt(Dn_StatePtr_t const *state);
DEBUG_NEW_FUNC_DECL(void) DnPrint_SizeApproximation(size_t s);
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#define DnPrint_SizeAproximation DnPrint_SizeApproximation
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_FileAndLineAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_ThreadAlreadyLocked(Dn_PrintFunc_t print_func, DEBUG_NEW_OS_THREAD_ID_T const thread);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_ExprInFuncAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_ObjectAtAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_SizeApproximationAlreadyLocked(Dn_PrintFunc_t print_func, size_t s);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_AccessMethod(char const *ac, void const *addr, Dn_AllocType_t method);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_AccessMethodAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac, void const *addr, Dn_AllocType_t method);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_LeaksFirstMessage(Dn_State_t const *state, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_LeaksFirstMessageAlreadyLocked(Dn_PrintFunc_t print_func, Dn_State_t const *state, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_LeakStatistics(char const *ac, void const *addr, unsigned int ntotal_leaks, size_t total_leak, unsigned int ntotal_frees, size_t total_free);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print_LeakStatisticsAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac, void const *addr, unsigned int ntotal_leaks, size_t total_leak, unsigned int ntotal_frees, size_t total_free);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Statistics dumping API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_CONFIG_HAVE_STATS
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_Stats(char const *prefix, Dn_Stat_t *stats);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_StatsAlreadyLocked(Dn_PrintFunc_t print_func, char const *prefix, Dn_Stat_t *stats);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_ThreadStats(Dn_Thread_t *thread);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
DEBUG_NEW_FUNC_DECL(void) DnDump_ThreadStatsEx(DEBUG_NEW_OS_THREAD_ID_T thread_id);
DEBUG_NEW_FUNC_DECL(void) DnDump_ThreadStats(void);
DEBUG_NEW_FUNC_DECL(void) DnDump_GlobalStats(void);
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_Allocations(Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_AllocationsAlreadyLocked(Dn_PrintFunc_t print_func, Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_MostAllocations(Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev, int most_calls, int merge);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Dump_MostAllocationsAlreadyLocked(Dn_PrintFunc_t print_func, Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev, int most_calls, int merge);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

//////////////////////////////////////////////////////////////////////////
// <<DnDump_GlobalAllocs>>
//  - Dumps all allocations created by the current thread
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnDump_ThreadAllocs(void);
DEBUG_NEW_FUNC_DECL(void) DnDump_ThreadAllosEx(DEBUG_NEW_OS_THREAD_ID_T thread_id);

//////////////////////////////////////////////////////////////////////////
// <<DnDump_GlobalAllocs>>
//  - Dumps all current allocations globally
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnDump_GlobalAllocs(void);

//////////////////////////////////////////////////////////////////////////
// <<DnDump_MostThreadAllocs>>
//  - Dumps sorted lists of the biggest memory consumers of the current thread
//  - if "most_calls" is true, sorting and dumping will be performed
//    based on the amount of allocation calls, meaning that 2 small
//    allocations will be ranked higher than 1 big.
//    (if "most_calls" is false, sorting will be based on allocation sizes)
//  - If "merge" is true, allocations are dumped after being merged,
//    meaning that 2 allocations from the same code address will
//    get added together
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnDump_MostThreadAllocs(int most_calls, int merge);
DEBUG_NEW_FUNC_DECL(void) DnDump_MostThreadAllocsEx(DEBUG_NEW_OS_THREAD_ID_T thread_id, int most_calls, int merge);

//////////////////////////////////////////////////////////////////////////
// <<DnDump_MostGlobalAllocs>>
//  - Dumps sorted lists of the biggest memory consumers of the application
//  - s.a.: "DnDump_MostThreadAllocs"
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(void) DnDump_MostGlobalAllocs(int most_calls, int merge);
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Pointer validation API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
//////////////////////////////////////////////////////////////////////////
// Pointer check methods
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_TAILCHECK_SIZE
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT void const *) DnCheck_Tail(Dn_MemPtr_t const *ptr); //< returns the exact address, where the first corruption occurred or 0
#endif /* DEBUG_NEW_CONFIG_TAILCHECK_SIZE */
// v return false, if the magic is invalid
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnCheck_Record(Dn_MemPtr_t const *rec, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnCheck_Pointer(void const *ptr, char const *ac); //< check ptr for magic, corruption, ...
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *) DnPointer_GetFile(void const *ptr, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPointer_GetLine(void const *ptr, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *) DnPointer_GetLineStr(void const *ptr, char const *ac);
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *) DnPointer_GetFunc(void const *ptr, char const *ac);
#endif
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *) DnPointer_GetExpr(void const *ptr, char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *) DnPointer_GetMesg(void const *ptr, char const *ac);
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t) DnPointer_GetID(void const *ptr, char const *ac);
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_MemPtr_t *) DnRecord_Get(void *ptr, char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_CheckRecordInternal(Dn_MemPtr_t const *rec, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_CheckPointerInternal(void const *ptr, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_MemPtr_t *) DnPrivate_GetRecord(void *ptr, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *) DnAccess_GetFile(char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *) DnAccess_GetLine(char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *) DnAccess_GetExpr(char const *ac); //< optional; return &'\0' if allocated with new
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *) DnAccess_GetFunc(char const *ac);
#else /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
#ifdef __cplusplus 
// v preserve namespace integrity
inline DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *DnAccess_GetFunc(char const *) { return ""; }
#else /* __cplusplus */
# define DnAccess_GetFunc(ac) ""
#endif /* !__cplusplus */
#endif /* !DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

// v these are called from "debug_new.c" --> and therefor don't require file and line information
#define DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL  DEBUG_NEW_PRIVATE_MAKE_ACCESS("","","Report memoryLeaks on shutdown (push)","")
#define DEBUG_NEW_PRIVATE_ACCESS_QUIT_INTERNAL  DEBUG_NEW_PRIVATE_MAKE_ACCESS("","","Report memoryLeaks on shutdown (pop)","")

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Global / Thread memory state API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
//////////////////////////////////////////////////////////////////////////
// Thread state pushing / popping
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(int) DnState_PushThread(char const *ac); /*< push calling thread state. */
// v pop latest state of the calling thread.
//   returns: amount of inconsistencies found (leaks / frees)
DEBUG_NEW_FUNC_DECL(unsigned int) DnState_PopThread(char const *ac);
// ^ compares state from last push to current state and dumps leaks

//////////////////////////////////////////////////////////////////////////
// Global state pushing / popping
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_DECL(int) DnState_PushGlobal(char const *ac); /*< push global state. */
DEBUG_NEW_FUNC_DECL(unsigned int) DnState_PopGlobal(char const *ac);  /*< pop global state. */
// ^ compares state from last push to current state and dumps leaks

#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(int) DnPrivate_State_PushThread(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_State_PopThread(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(int) DnPrivate_State_PushGlobal(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(unsigned int) DnPrivate_State_PopGlobal(char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// Initialization / Finalization
// NOTE: init and quit will automatically 
//       push and pop an initial global state,
//       if DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE is defined as 1 
// NOTE: these methods allow for recursion, if "DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION" is defined
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Initialization / Finalization API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) Dn_Initialized(void);
DEBUG_NEW_FUNC_DECL(void) Dn_InitInternal(char const *ac);
DEBUG_NEW_FUNC_DECL(void) Dn_QuitInternal(char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_InitInternal(char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_QuitInternal(char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

//////////////////////////////////////////////////////////////////////////
// print methods
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region print API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_FUNC_DECL(void) DnPrint(char const *msg);
DEBUG_NEW_FUNC_DECL(void) DnPrintf(char const *msg, ...);
DEBUG_NEW_FUNC_DECL(void) DnVPrintf(char const *msg, va_list args);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Print(Dn_PrintFunc_t func, char const *msg);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_Printf(Dn_PrintFunc_t func, char const *msg, ...);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_VPrintf(Dn_PrintFunc_t func, char const *msg, va_list args);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_PrintAlreadyLocked(Dn_PrintFunc_t func, char const *msg);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_PrintfAlreadyLocked(Dn_PrintFunc_t func, char const *msg, ...);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_VPrintfAlreadyLocked(Dn_PrintFunc_t func, char const *msg, va_list args);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// Debug allocation / deallocation / reallocation / callocation routines
//////////////////////////////////////////////////////////////////////////

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region malloc API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnDebug_Malloc(Dn_AllocType_t mode, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnDebug_MallocNZ(Dn_AllocType_t mode, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_FREE(p) void) DnDebug_Free(Dn_AllocType_t mode, void *p, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_FREE_NN(p) void) DnDebug_FreeNN(Dn_AllocType_t mode, void *p, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnDebug_Realloc(void *p, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnDebug_ReallocNZ(void *p, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnDebug_ReallocNN(void *p, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnDebug_ReallocNNZ(void *p, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnDebug_Calloc(size_t n, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnDebug_CallocNZ(size_t n, size_t s, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t) DnDebug_MallocMSize(void *p, char const *ac);                                            //
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t) DnDebug_MallocMSizeNN(void *p, char const *ac);          // these 2 behave exactly the same, but this method
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t) DnDebug_MallocMSizeC(void const *p, char const *ac);                                   // is only const on osx, so we want to keep it that way.
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t) DnDebug_MallocMSizeCNN(void const *p, char const *ac); //
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnPrivate_Debug_Malloc(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnPrivate_Debug_MallocNZ(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_FREE(p) void) DnPrivate_Debug_Free(Dn_AllocType_t mode, void *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_FREE_NN(p) void) DnPrivate_Debug_FreeNN(Dn_AllocType_t mode, void *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_FreeNoCheck(Dn_AllocType_t mode, Dn_MemPtr_t *rec, char const *ac);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_FreeNoCheck2(Dn_AllocType_t mode, Dn_MemPtr_t *rec, char const *ac, void const *addr);
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#define DnDebug_MSize          DnDebug_MallocMSize
#define DnDebug_MCSize         DnDebug_MallocMSizeC
#define _DnDebug_FreeNoCheck   DnPrivate_FreeNoCheck
#define _DnDebug_Malloc2       DnPrivate_Debug_Malloc
#define _DnDebug_Free2         DnPrivate_Debug_Free
#define __DnDebug_FreeNoCheck2 DnPrivate_FreeNoCheck2
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnPrivate_Debug_Realloc(void *p, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnPrivate_Debug_ReallocNZ(void *p, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnPrivate_Debug_ReallocNN(void *p, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnPrivate_Debug_ReallocNNZ(void *p, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnPrivate_Debug_Calloc(size_t n, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnPrivate_Debug_CallocNZ(size_t n, size_t s, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t) DnPrivate_Debug_MallocMSize(void const *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t) DnPrivate_Debug_MallocMSizeNN(void const *p, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region mallocf API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnDebug_Mallocf(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnDebug_MallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnDebug_Reallocf(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnDebug_ReallocfNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnDebug_ReallocfNN(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnDebug_ReallocfNNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnDebug_Callocf(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnDebug_CallocfNZ(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnDebug_VMallocf(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnDebug_VMallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnDebug_VReallocf(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnDebug_VReallocfNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnDebug_VReallocfNN(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnDebug_VReallocfNNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnDebug_VCallocf(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnDebug_VCallocfNZ(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#define DnDebug_Malloc_Exf   DnDebug_Mallocf
#define DnDebug_Realloc_Exf  DnDebug_Reallocf
#define DnDebug_Calloc_Exf   DnDebug_Callocf
#define DnDebug_Malloc_VExf  DnDebug_VMallocf
#define DnDebug_Realloc_VExf DnDebug_VReallocf
#define DnDebug_Calloc_VExf  DnDebug_VCallocf
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnPrivate_Debug_Mallocf(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnPrivate_Debug_MallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnPrivate_Debug_Reallocf(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnPrivate_Debug_ReallocfNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnPrivate_Debug_ReallocfNN(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnPrivate_Debug_ReallocfNNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnPrivate_Debug_Callocf(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnPrivate_Debug_CallocfNZ(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *) DnPrivate_Debug_VMallocf(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *) DnPrivate_Debug_VMallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *) DnPrivate_Debug_VReallocf(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *) DnPrivate_Debug_VReallocfNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *) DnPrivate_Debug_VReallocfNN(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *) DnPrivate_Debug_VReallocfNNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *) DnPrivate_Debug_VCallocf(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *) DnPrivate_Debug_VCallocfNZ(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region strdup API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) char *) DnDebug_StrDup(char const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) char *) DnDebug_StrDupNZ(char const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) char *) DnDebug_StrNDup(char const *str, size_t n, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) char *) DnDebug_StrNDupNZ(char const *str, size_t n, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) DnWchar_t *) DnDebug_WcsDup(DnWchar_t const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) DnWchar_t *) DnDebug_WcsDupNZ(DnWchar_t const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) DnWchar_t *) DnDebug_WcsNDup(DnWchar_t const *str, size_t n, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) DnWchar_t *) DnDebug_WcsNDupNZ(DnWchar_t const *str, size_t n, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) unsigned char *) DnDebug_MbsDup(unsigned char const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) unsigned char *) DnDebug_MbsDupNZ(unsigned char const *str, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) unsigned char *) DnDebug_MbsNDup(unsigned char const *str, size_t n, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) unsigned char *) DnDebug_MbsNDupNZ(unsigned char const *str, size_t n, char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) char *) DnPrivate_Debug_StrDup(char const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) char *) DnPrivate_Debug_StrDupNZ(char const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) char *) DnPrivate_Debug_StrNDup(char const *str, size_t n, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) char *) DnPrivate_Debug_StrNDupNZ(char const *str, size_t n, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) DnWchar_t *) DnPrivate_Debug_WcsDup(DnWchar_t const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) DnWchar_t *) DnPrivate_Debug_WcsDupNZ(DnWchar_t const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) DnWchar_t *) DnPrivate_Debug_WcsNDup(DnWchar_t const *str, size_t n, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) DnWchar_t *) DnPrivate_Debug_WcsNDupNZ(DnWchar_t const *str, size_t n, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP(str) unsigned char *) DnPrivate_Debug_MbsDup(unsigned char const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) unsigned char *) DnPrivate_Debug_MbsDupNZ(unsigned char const *str, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) unsigned char *) DnPrivate_Debug_MbsNDup(unsigned char const *str, size_t n, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) unsigned char *) DnPrivate_Debug_MbsNDupNZ(unsigned char const *str, size_t n, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Aligned Allocation API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnDebug_AlignedMalloc(size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnDebug_AlignedMallocNZ(size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnDebug_AlignedOffsetMalloc(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnDebug_AlignedCalloc(size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnDebug_AlignedCallocNZ(size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnDebug_AlignedOffsetCalloc(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnDebug_AlignedRealloc(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnDebug_AlignedReallocNZ(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnDebug_AlignedReallocNN(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnDebug_AlignedReallocNNZ(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnDebug_AlignedOffsetRealloc(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNZ(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNN(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p) void) DnDebug_AlignedFree(void *p, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) void) DnDebug_AlignedFreeNN(void *p, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a) size_t) DnDebug_AlignedMallocMSize(void const *p, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) size_t) DnDebug_AlignedMallocMSizeNN(void const *p, size_t a, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o) size_t) DnDebug_AlignedOffsetMallocMSize(void const *p, size_t a, size_t o, char const *ac);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) size_t) DnDebug_AlignedOffsetMallocMSizeNN(void const *p, size_t a, size_t o, char const *ac);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnPrivate_Debug_AlignedMalloc(size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnPrivate_Debug_AlignedMallocNZ(size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnPrivate_Debug_AlignedOffsetMalloc(size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnPrivate_Debug_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnPrivate_Debug_AlignedCalloc(size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnPrivate_Debug_AlignedCallocNZ(size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnPrivate_Debug_AlignedOffsetCalloc(size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnPrivate_Debug_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnPrivate_Debug_AlignedRealloc(void *p, size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnPrivate_Debug_AlignedReallocNZ(void *p, size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnPrivate_Debug_AlignedReallocNN(void *p, size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnPrivate_Debug_AlignedReallocNNZ(void *p, size_t s, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetRealloc(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocNN(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p) void) DnPrivate_Debug_AlignedFree(void *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) void) DnPrivate_Debug_AlignedFreeNN(void *p, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a) size_t) DnPrivate_Debug_AlignedMallocMSize(void const *p, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) size_t) DnPrivate_Debug_AlignedMallocMSizeNN(void const *p, size_t a, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o) size_t) DnPrivate_Debug_AlignedOffsetMallocMSize(void const *p, size_t a, size_t o, char const *ac, void const *addr);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) size_t) DnPrivate_Debug_AlignedOffsetMallocMSizeNN(void const *p, size_t a, size_t o, char const *ac, void const *addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnDebug_AlignedMallocf(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnDebug_AlignedMallocfNZ(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnDebug_AlignedOffsetMallocf(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetMallocfNZ(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnDebug_AlignedCallocf(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnDebug_AlignedCallocfNZ(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnDebug_AlignedOffsetCallocf(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetCallocfNZ(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnDebug_AlignedReallocf(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnDebug_AlignedReallocfNZ(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnDebug_AlignedReallocfNN(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnDebug_AlignedReallocfNNZ(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnDebug_AlignedOffsetReallocf(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocfNZ(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnDebug_AlignedOffsetReallocfNN(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocfNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnDebug_VAlignedMallocf(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnDebug_VAlignedMallocfNZ(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnDebug_VAlignedOffsetMallocf(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnDebug_VAlignedOffsetMallocfNZ(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnDebug_VAlignedCallocf(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnDebug_VAlignedCallocfNZ(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnDebug_VAlignedOffsetCallocf(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnDebug_VAlignedOffsetCallocfNZ(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnDebug_VAlignedReallocf(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnDebug_VAlignedReallocfNZ(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnDebug_VAlignedReallocfNN(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnDebug_VAlignedReallocfNNZ(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnDebug_VAlignedOffsetReallocf(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnDebug_VAlignedOffsetReallocfNZ(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnDebug_VAlignedOffsetReallocfNN(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnDebug_VAlignedOffsetReallocfNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnPrivate_Debug_AlignedMallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnPrivate_Debug_AlignedMallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnPrivate_Debug_AlignedOffsetMallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnPrivate_Debug_AlignedOffsetMallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnPrivate_Debug_AlignedCallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnPrivate_Debug_AlignedCallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnPrivate_Debug_AlignedOffsetCallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnPrivate_Debug_AlignedOffsetCallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnPrivate_Debug_AlignedReallocf(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnPrivate_Debug_AlignedReallocfNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnPrivate_Debug_AlignedReallocfNN(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnPrivate_Debug_AlignedReallocfNNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocf(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocfNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocfNN(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnPrivate_Debug_AlignedOffsetReallocfNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnPrivate_Debug_VAlignedMallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnPrivate_Debug_VAlignedMallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnPrivate_Debug_VAlignedOffsetMallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnPrivate_Debug_VAlignedOffsetMallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnPrivate_Debug_VAlignedCallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnPrivate_Debug_VAlignedCallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnPrivate_Debug_VAlignedOffsetCallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnPrivate_Debug_VAlignedOffsetCallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnPrivate_Debug_VAlignedReallocf(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnPrivate_Debug_VAlignedReallocfNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnPrivate_Debug_VAlignedReallocfNN(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnPrivate_Debug_VAlignedReallocfNNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnPrivate_Debug_VAlignedOffsetReallocf(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnPrivate_Debug_VAlignedOffsetReallocfNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnPrivate_Debug_VAlignedOffsetReallocfNN(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnPrivate_Debug_VAlignedOffsetReallocfNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Allocation Breakpoint API Functions
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID && \
    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_DATA_DECL(size_t,DnPrivateGlobal_AllocationBreakpoint);
DEBUG_NEW_DATA_DECL(int,DnPrivateGlobal_AllocationBreakpointIsSet);
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
DEBUG_NEW_FUNC_DECL(void) DnGetAllocBreakpointSet(size_t id);
DEBUG_NEW_FUNC_DECL(void) DnGetAllocBreakpointClear(void);
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t) DnGetAllocID(void);
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region C / C++ Initialization Hooks
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#if DEBUG_NEW_MAIN_HOOK
int _after_debug_new_main(int argc, char* argv[]);
int main(int argc, char* argv[]) {
 int app_exit_code;
 DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_InitInternal(DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL);
#ifdef __cplusplus
 try {
  app_exit_code = _after_debug_new_main(argc,argv);
 } catch (...) {
  DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_QuitInternal(DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL);
  throw;
 }
#else /* !__cplusplus */
 app_exit_code = _after_debug_new_main(argc,argv);
#endif /* !__cplusplus */
 DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_QuitInternal(DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL);
 return app_exit_code;
}
#undef main
#define main _after_debug_new_main
#elif defined(__cplusplus)
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL
#if DEBUG_NEW_CONFIG_LINK_STATICALLY
static class DnPrivateStaticIncludeCounter {
 public:
  inline DnPrivateStaticIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT {
   DnPrivate_InitInternal(DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
  }
  inline ~DnPrivateStaticIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT {
   DnPrivate_QuitInternal(DEBUG_NEW_PRIVATE_ACCESS_QUIT_INTERNAL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
  }
} DnPrivate_StaticIncludeCounter;
#else /* DEBUG_NEW_CONFIG_LINK_STATICALLY */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_IncludeCounter_IncCount(void);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_IncludeCounter_DecCount(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */
static class DnPrivateIncludeCounter { public:
 inline DnPrivateIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT { DnPrivate_IncludeCounter_IncCount(); }
 inline ~DnPrivateIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT { DnPrivate_IncludeCounter_DecCount(); }
} DnPrivate_IncludeCounter;
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#else
#define Dn_Init() (DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_InitInternal(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("Dn_Init()")))
#define Dn_Quit() (DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_QuitInternal(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("Dn_Quit()")))
#endif
#endif /* DEBUG_NEW_CONFIG_HAVE_AUTO_INIT */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Debug New debug-API macro implemenation
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#ifdef __cplusplus
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
class Dn_NewPreprocessor {
 private: char const *m_ac; public:
 template<class T> inline DEBUG_NEW_ATTRIBUTE_MALLOC DEBUG_NEW_ATTRIBUTE_USE_RESULT
 DEBUG_NEW_ANNOTATION_RETURNS(p) T *operator ->* (T *p) const DEBUG_NEW_CXX11_NOEXCEPT {
   DnPrivate_ProcessCXXPointer((void *)p,this->m_ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); return p; }
 inline DEBUG_NEW_CXX11_CONSTEXPR Dn_NewPreprocessor(char const *ac) DEBUG_NEW_CXX11_NOEXCEPT: m_ac(ac) {}
};
DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
class Dn_NewPreprocessor { public: 
 template<class T> inline DEBUG_NEW_CXX11_CONSTEXPR
 DEBUG_NEW_ANNOTATION_RETURNS(p) T *operator ->* (T *p) const DEBUG_NEW_CXX11_NOEXCEPT { return p; }
 inline DEBUG_NEW_CXX11_CONSTEXPR Dn_NewPreprocessor(char const *) DEBUG_NEW_CXX11_NOEXCEPT {}
};
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#endif /* __cplusplus */


//////////////////////////////////////////////////////////////////////////
// <<Public debug_new api macros>>
//  - Most of these are used as the debug implementations of the
//    functions overwritten by debug_new.
//  - There functions are always available and are always following
//    the core debug_new configuration, meaning that these macros always
//    link against the debug-functions, if debug_new is enabled, even
//    if the overwritten api is temporarily disabled with <debug_new_disable.inl>
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
# define DEBUG_NEW_CXX_PLACEMENT            DEBUG_NEW_CXX_NAMESPACE_ROOT DEBUG_NEW_CONFIG_CXX_NAMESPACE_DETAIL
# define DEBUG_NEW_CXX_NEW                  DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_NewPreprocessor(DEBUG_NEW_PRIVATE_EMPTY_ACCESS)->* new
#endif /* __cplusplus */

// v custom access low-level memory macros
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,ac)    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,ac)
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,ac) DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,ac)
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE(p,ac)      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,p,ac)
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN(p,ac)   DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_FreeNN(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,p,ac)
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC         DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Realloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocNN
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocNNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Calloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_CallocNZ
#define DEBUG_NEW_API_MALLOC(s)                     DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc(" #s ")"))
#define DEBUG_NEW_API_MALLOC_NZ(s)                  DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_nz(" #s ")"))
#define DEBUG_NEW_API_FREE(p)                       DEBUG_NEW_API_CUSTOM_ACCESS_FREE(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("free(" #p ")"))
#define DEBUG_NEW_API_FREE_NN(p)                    DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("free_nn(" #p ")"))
#define DEBUG_NEW_API_REALLOC(p,s)                  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc(" #p "," #s ")"))
#define DEBUG_NEW_API_REALLOC_NN(p,s)               DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc_nn(" #p "," #s ")"))
#define DEBUG_NEW_API_REALLOC_NZ(p,s)               DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc_nz(" #p "," #s ")"))
#define DEBUG_NEW_API_REALLOC_NNZ(p,s)              DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc_nnz(" #p "," #s ")"))
#define DEBUG_NEW_API_CALLOC(n,s)                   DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("calloc(" #n "," #s ")"))
#define DEBUG_NEW_API_CALLOC_NZ(n,s)                DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("calloc_nz(" #n "," #s ")"))
#define DEBUG_NEW_API_STRDUP(s)                     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_StrDup(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strdup(" #s ")"))
#define DEBUG_NEW_API_STRDUP_NZ(s)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_StrDupNZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strdup_nz(" #s ")"))
#define DEBUG_NEW_API_WCSDUP(s)                     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_WcsDup(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("wcsdup(" #s ")"))
#define DEBUG_NEW_API_WCSDUP_NZ(s)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_WcsDupNZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("wcsdup_nz(" #s ")"))
#define DEBUG_NEW_API_MBSDUP(s)                     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MbsDup(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mbsdup(" #s ")"))
#define DEBUG_NEW_API_MBSDUP_NZ(s)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MbsDupNZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mbsdup_nz(" #s ")"))
#define DEBUG_NEW_API_STRNDUP(s,n)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_StrNDup(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strndup(" #s "," #n ")"))
#define DEBUG_NEW_API_STRNDUP_NZ(s,n)               DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_StrNDupNZ(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strndup(" #s "," #n ")"))
#define DEBUG_NEW_API_WCSNDUP(s,n)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_WcsNDup(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("wcsndup(" #s "," #n ")"))
#define DEBUG_NEW_API_WCSNDUP_NZ(s,n)               DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_WcsNDupNZ(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("wcsndup_nz(" #s "," #n ")"))
#define DEBUG_NEW_API_MBSNDUP(s,n)                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MbsNDup(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mbsndup(" #s "," #n ")"))
#define DEBUG_NEW_API_MBSNDUP_NZ(s,n)               DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MbsNDupNZ(s,n,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mbsndup_nz(" #s "," #n ")"))
#define DEBUG_NEW_API_MALLOC_MSIZE(p)               DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocMSize(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_msize(" #p ")"))
#define DEBUG_NEW_API_MALLOC_MSIZE_C(p)             DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocMSizeC(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_msize(" #p ")"))
#define DEBUG_NEW_API_MALLOC_MSIZE_NN(p)            DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocMSizeNN(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_msize(" #p ")"))
#define DEBUG_NEW_API_MALLOC_MSIZE_C_NN(p)          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocMSizeCNN(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_msize(" #p ")"))
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
#if DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
# define DEBUG_NEW_API_MALLOCF(s,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Mallocf(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf(" #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_MALLOCF_NZ(s,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocfNZ(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf_nz(" #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF(p,s,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Reallocf(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf(" #p "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NZ(p,s,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nz(" #p "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NN(p,s,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nn(" #p "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NNZ(p,s,...)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nnz(" #p "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_CALLOCF(n,s,...)            DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Callocf(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf(" #n "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_CALLOCF_NZ(n,s,...)         DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_CallocfNZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf_nz(" #n "," #s "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_VMALLOCF(s,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VMallocf(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf(" #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VMALLOCF_NZ(s,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VMallocfNZ(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf_nz(" #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF(p,s,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocf(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf(" #p "," #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NZ(p,s,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nz(" #p "," #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NN(p,s,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nn(" #p "," #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NNZ(p,s,fmt,args) DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nnz(" #p "," #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VCALLOCF(n,s,fmt,args)      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VCallocf(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf(" #n "," #s "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VCALLOCF_NZ(n,s,fmt,args)   DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VCallocfNZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf_nz(" #n "," #s "," #fmt "," #args ")"),fmt,args)
#else /* DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
# define DEBUG_NEW_API_MALLOCF(s,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Mallocf(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf(" #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_MALLOCF_NZ(s,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_MallocfNZ(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf_nz(" #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF(p,s,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Reallocf(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf(" #p "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NZ(p,s,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nz(" #p "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NN(p,s,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nn(" #p "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_REALLOCF_NNZ(p,s,...)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_ReallocfNNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nnz(" #p "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_CALLOCF(n,s,...)            DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_Callocf(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf(" #n "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_CALLOCF_NZ(n,s,...)         DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_CallocfNZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf_nz(" #n "," #s ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_VMALLOCF(s,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VMallocf(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf(" #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VMALLOCF_NZ(s,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VMallocfNZ(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf_nz(" #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF(p,s,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocf(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf(" #p "," #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NZ(p,s,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nz(" #p "," #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NN(p,s,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nn(" #p "," #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VREALLOCF_NNZ(p,s,fmt,args) DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VReallocfNNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nnz(" #p "," #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VCALLOCF(n,s,fmt,args)      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VCallocf(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf(" #n "," #s ",...)"),fmt,args)
# define DEBUG_NEW_API_VCALLOCF_NZ(n,s,fmt,args)   DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VCallocfNZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf_nz(" #n "," #s ",...)"),fmt,args)
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
#else /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
# define DEBUG_NEW_API_MALLOCF(s,...)              DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf(" #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_MALLOCF_NZ(s,...)           DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf_nz(" #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_REALLOCF(p,s,...)           DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf(" #p "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_REALLOCF_NZ(p,s,...)        DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nz(" #p "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_REALLOCF_NN(p,s,...)        DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nn(" #p "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_REALLOCF_NNZ(p,s,...)       DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nnz(" #p "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_CALLOCF(n,s,...)            DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf(" #n "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_CALLOCF_NZ(n,s,...)         DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf_nz(" #n "," #s "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_VMALLOCF(s,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf(" #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VMALLOCF_NZ(s,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf_nz(" #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VREALLOCF(p,s,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf(" #p "," #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VREALLOCF_NZ(p,s,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nz(" #p "," #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VREALLOCF_NN(p,s,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nn(" #p "," #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VREALLOCF_NNZ(p,s,fmt,args) DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nnz(" #p "," #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VCALLOCF(n,s,fmt,args)      DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf(" #n "," #s "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VCALLOCF_NZ(n,s,fmt,args)   DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf_nz(" #n "," #s "," #fmt "," #args ")"))
#else /* DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
# define DEBUG_NEW_API_MALLOCF(s,...)              DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf(" #s ",...)"))
# define DEBUG_NEW_API_MALLOCF_NZ(s,...)           DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mallocf_nz(" #s ",...)"))
# define DEBUG_NEW_API_REALLOCF(p,s,...)           DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf(" #p "," #s ",...)"))
# define DEBUG_NEW_API_REALLOCF_NZ(p,s,...)        DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nz(" #p "," #s ",...)"))
# define DEBUG_NEW_API_REALLOCF_NN(p,s,...)        DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nn(" #p "," #s ",...)"))
# define DEBUG_NEW_API_REALLOCF_NNZ(p,s,...)       DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("reallocf_nnz(" #p "," #s ",...)"))
# define DEBUG_NEW_API_CALLOCF(n,s,...)            DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf(" #n "," #s ",...)"))
# define DEBUG_NEW_API_CALLOCF_NZ(n,s,...)         DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("callocf_nz(" #n "," #s ",...)"))
# define DEBUG_NEW_API_VMALLOCF(s,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf(" #s ",...)"))
# define DEBUG_NEW_API_VMALLOCF_NZ(s,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vmallocf_nz(" #s ",...)"))
# define DEBUG_NEW_API_VREALLOCF(p,s,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf(" #p "," #s ",...)"))
# define DEBUG_NEW_API_VREALLOCF_NZ(p,s,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nz(" #p "," #s ",...)"))
# define DEBUG_NEW_API_VREALLOCF_NN(p,s,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nn(" #p "," #s ",...)"))
# define DEBUG_NEW_API_VREALLOCF_NNZ(p,s,fmt,args) DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vreallocf_nnz(" #p "," #s ",...)"))
# define DEBUG_NEW_API_VCALLOCF(n,s,fmt,args)      DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf(" #n "," #s ",...)"))
# define DEBUG_NEW_API_VCALLOCF_NZ(n,s,fmt,args)   DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("vcallocf_nz(" #n "," #s ",...)"))
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
#endif /* !DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 1 */

#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC             DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMalloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC            DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedRealloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN         DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ         DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocNN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocNNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC             DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCalloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMalloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ   DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetRealloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocNN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocNNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC      DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetCalloc
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ   DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetCallocNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE               DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedFree
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN            DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedFreeNN
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE(p,a)                DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocMSize(p,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc_msize(" #p "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN(p,a)             DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocMSizeNN(p,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc_msize_nn(" #p "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE(p,a,o)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMallocMSize(p,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_malloc_msize(" #p "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN(p,a,o)    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMallocMSizeNN(p,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_malloc_msize_nn(" #p "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_MALLOC(s,a)                      DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc(" #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_MALLOC_NZ(s,a)                   DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc_nz(" #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_REALLOC(p,s,a)                   DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc(" #p "," #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_REALLOC_NN(p,s,a)                DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nn(" #p "," #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_REALLOC_NZ(p,s,a)                DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nz(" #p "," #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_REALLOC_NNZ(p,s,a)               DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nnz(" #p "," #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_CALLOC(s,a)                      DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_calloc(" #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_CALLOC_NZ(s,a)                   DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_calloc_nz(" #s "," #a ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC(s,a,o)             DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc(" #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)          DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_malloc_nz(" #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC(p,s,a,o)          DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc(" #p "," #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nn(" #p "," #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nz(" #p "," #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)      DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_realloc_nnz(" #p "," #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC(s,a,o)             DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_calloc(" #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)          DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_calloc_nz(" #s "," #a "," #o ")"))
#define DEBUG_NEW_API_ALIGNED_FREE(p)                          DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_free(" #p ")"))
#define DEBUG_NEW_API_ALIGNED_FREE_NN(p)                       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_free_nn(" #p ")"))
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
#if DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
# define DEBUG_NEW_API_ALIGNED_MALLOCF(s,a,...)                       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf(" #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ(s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf_nz(" #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF(p,s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocf(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf(" #p "," #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ(p,s,a,...)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nz(" #p "," #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NN(p,s,a,...)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nn(" #p "," #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ(p,s,a,...)                DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nnz(" #p "," #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_CALLOCF(s,a,...)                       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf(" #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ(s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf_nz(" #s "," #a "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_VALIGNED_MALLOCF(s,a,fmt,args)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf(" #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf_nz(" #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF(p,s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocf(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf(" #p "," #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ(p,s,a,fmt,args)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nz(" #p "," #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NN(p,s,a,fmt,args)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nn(" #p "," #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ(p,s,a,fmt,args)          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nnz(" #p "," #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_CALLOCF(s,a,fmt,args)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf(" #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf_nz(" #s "," #a "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF(s,a,o,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf(" #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ(s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetMallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf_nz(" #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF(p,s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocf(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocfNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nz(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocfNN(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nn(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,...)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetReallocfNNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nnz(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF(s,a,o,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetCallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf(" #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ(s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedOffsetCallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf_nz(" #s "," #a "," #o "," #__VA_ARGS__ ")"),__VA_ARGS__)
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF(s,a,o,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetMallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf(" #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetMallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf_nz(" #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF(p,s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetReallocf(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf(" #p "," #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetReallocfNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nz(" #p "," #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetReallocfNN(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nn(" #p "," #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,fmt,args) DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetReallocfNNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nnz(" #p "," #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF(s,a,o,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetCallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf(" #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedOffsetCallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf_nz(" #s "," #a "," #o "," #fmt "," #args ")"),fmt,args)
#else /* DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
# define DEBUG_NEW_API_ALIGNED_MALLOCF(s,a,...)                       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf(" #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ(s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf_nz(" #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF(p,s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocf(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf(" #p "," #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ(p,s,a,...)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nz(" #p "," #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NN(p,s,a,...)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nn(" #p "," #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ(p,s,a,...)                DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nnz(" #p "," #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_CALLOCF(s,a,...)                       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf(" #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ(s,a,...)                    DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf_nz(" #s "," #a ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_VALIGNED_MALLOCF(s,a,fmt,args)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf(" #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf_nz(" #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF(p,s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocf(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf(" #p "," #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ(p,s,a,fmt,args)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nz(" #p "," #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NN(p,s,a,fmt,args)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nn(" #p "," #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ(p,s,a,fmt,args)          DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nnz(" #p "," #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_CALLOCF(s,a,fmt,args)                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocf(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf(" #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocfNZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf_nz(" #s "," #a ",...)"),fmt,args)
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF(s,a,o,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf(" #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ(s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedMallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf_nz(" #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF(p,s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocf(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf(" #p "," #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nz(" #p "," #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,...)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNN(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nn(" #p "," #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,...)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedReallocfNNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nnz(" #p "," #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF(s,a,o,...)              DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf(" #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ(s,a,o,...)           DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_AlignedCallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf_nz(" #s "," #a "," #o ",...)"),__VA_ARGS__)
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF(s,a,o,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf(" #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedMallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf_nz(" #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF(p,s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocf(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf(" #p "," #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nz(" #p "," #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,fmt,args)  DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNN(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nn(" #p "," #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,fmt,args) DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedReallocfNNZ(p,s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nnz(" #p "," #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF(s,a,o,fmt,args)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocf(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf(" #s "," #a "," #o ",...)"),fmt,args)
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_CXX_NAMESPACE_ROOT DnDebug_VAlignedCallocfNZ(s,a,o,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf_nz(" #s "," #a "," #o ",...)"),fmt,args)
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
#else /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
# define DEBUG_NEW_API_ALIGNED_MALLOCF(s,a,...)                       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf(" #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ(s,a,...)                    DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf_nz(" #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF(p,s,a,...)                    DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf(" #p "," #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ(p,s,a,...)                 DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nz(" #p "," #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NN(p,s,a,...)                 DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nn(" #p "," #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ(p,s,a,...)                DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nnz(" #p "," #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_CALLOCF(s,a,...)                       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf(" #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ(s,a,...)                    DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf_nz(" #s "," #a "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_VALIGNED_MALLOCF(s,a,fmt,args)                 DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf(" #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf_nz(" #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF(p,s,a,fmt,args)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf(" #p "," #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ(p,s,a,fmt,args)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nz(" #p "," #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NN(p,s,a,fmt,args)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nn(" #p "," #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ(p,s,a,fmt,args)          DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nnz(" #p "," #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_CALLOCF(s,a,fmt,args)                 DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf(" #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ(s,a,fmt,args)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf_nz(" #s "," #a "," #fmt "," #args ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF(s,a,o,...)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf(" #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ(s,a,o,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf_nz(" #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF(p,s,a,o,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,...)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nz(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,...)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nn(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,...)       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nnz(" #p "," #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF(s,a,o,...)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf(" #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ(s,a,o,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf_nz(" #s "," #a "," #o "," #__VA_ARGS__ ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF(s,a,o,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf(" #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf_nz(" #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF(p,s,a,o,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf(" #p "," #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nz(" #p "," #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nn(" #p "," #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,fmt,args) DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nnz(" #p "," #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF(s,a,o,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf(" #s "," #a "," #o "," #fmt "," #args ")"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ(s,a,o,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf_nz(" #s "," #a "," #o "," #fmt "," #args ")"))
#else /* DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
# define DEBUG_NEW_API_ALIGNED_MALLOCF(s,a,...)                     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf(" #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ(s,a,...)                  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_mallocf_nz(" #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF(p,s,a,...)                  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ(p,s,a,...)               DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nz(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NN(p,s,a,...)               DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nn(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ(p,s,a,...)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_reallocf_nnz(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_CALLOCF(s,a,...)                     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf(" #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ(s,a,...)                  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_callocf_nz(" #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_MALLOCF(s,a,fmt,args)               DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf(" #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ(s,a,fmt,args)            DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_mallocf_nz(" #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF(p,s,a,fmt,args)            DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ(p,s,a,fmt,args)         DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nz(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NN(p,s,a,fmt,args)         DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nn(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ(p,s,a,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_reallocf_nnz(" #p "," #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_CALLOCF(s,a,fmt,args)               DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf(" #s "," #a ",...)"))
# define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ(s,a,fmt,args)            DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_callocf_nz(" #s "," #a ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF(s,a,...)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ(s,a,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_mallocf_nz(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF(p,s,a,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ(p,s,a,...)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nz(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN(p,s,a,...)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nn(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,...)       DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_reallocf_nnz(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF(s,a,...)              DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ(s,a,...)           DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("aligned_offset_callocf_nz(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF(s,a,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ(s,a,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_mallocf_nz(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF(p,s,a,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ(p,s,a,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nz(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN(p,s,a,fmt,args)  DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nn(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,fmt,args) DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_reallocf_nnz(" #p "," #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF(s,a,fmt,args)        DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf(" #s "," #a "," #o ",...)"))
# define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ(s,a,fmt,args)     DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("valigned_offset_callocf_nz(" #s "," #a "," #o ",...)"))
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
#endif /* !DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 1 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#define DEBUG_NEW_API_MSTATE_PUSH()              DEBUG_NEW_CXX_NAMESPACE_ROOT DnState_PushThread(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_push()"))
#define DEBUG_NEW_API_MSTATE_POP()               DEBUG_NEW_CXX_NAMESPACE_ROOT DnState_PopThread(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_pop()"))
#define DEBUG_NEW_API_MSTATE_GLOBAL_PUSH()       DEBUG_NEW_CXX_NAMESPACE_ROOT DnState_PushGlobal(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_global_push()"))
#define DEBUG_NEW_API_MSTATE_GLOBAL_POP()        DEBUG_NEW_CXX_NAMESPACE_ROOT DnState_PopGlobal(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_global_pop()"))
#define DEBUG_NEW_API_POINTER_GET_FILE(p)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetFile(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_FILE(" #p ")"))
#define DEBUG_NEW_API_POINTER_GET_LINE(p)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetLine(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_LINE(" #p ")"))
#define DEBUG_NEW_API_POINTER_GET_LINE_STR(p)    DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetLineStr(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_LINE_STR(" #p ")"))
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
#define DEBUG_NEW_API_POINTER_GET_FUNC(p)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetFunc(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_FUNC(" #p ")"))
#else
#define DEBUG_NEW_API_POINTER_GET_FUNC(p)        ((char const *)"<function>")
#endif
#define DEBUG_NEW_API_POINTER_GET_EXPR(p)        DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetExpr(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_EXPR(" #p ")"))
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
#define DEBUG_NEW_API_POINTER_GET_MSG(p)         DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetMesg(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_EXPR(" #p ")"))
#else /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#define DEBUG_NEW_API_POINTER_GET_MSG(p)         ((char const *)NULL)
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 1 */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
#define DEBUG_NEW_API_POINTER_GET_ID(p)          DEBUG_NEW_CXX_NAMESPACE_ROOT DnPointer_GetID(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_POINTER_GET_ID(" #p ")"))
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#define DEBUG_NEW_API_POINTER_GET_ID(p)          ((size_t)0)
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#define DEBUG_NEW_API_CHECK_PTR(p)               DEBUG_NEW_CXX_NAMESPACE_ROOT DnCheck_Pointer(p,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_CHECK_PTR(" #p ")"))
#define DEBUG_NEW_API_PRINTFUNC_PUSH(func)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnThread_PushPrintFunc(func,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_PRINTFUNC_PUSH(" #func ")"))
#define DEBUG_NEW_API_PRINTFUNC_POP()            DEBUG_NEW_CXX_NAMESPACE_ROOT DnThread_PopPrintFunc(DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_PRINTFUNC_POP()"))
#define DEBUG_NEW_API_PRINTFUNC_STACK_SIZE       DEBUG_NEW_CXX_NAMESPACE_ROOT DnThread_GetPrintFuncStackSize
#define DEBUG_NEW_API_RAW_PRINT                  DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrint
#define DEBUG_NEW_API_RAW_PRINTF                 DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrintf
#define DEBUG_NEW_API_RAW_VPRINTF                DEBUG_NEW_CXX_NAMESPACE_ROOT DnVPrintf
#define DEBUG_NEW_API_RAW_PRINT_LOCATION()       DEBUG_NEW_API_RAW_PRINT(__FILE__ "(" DEBUG_NEW_PP_LINE_STR() ") : ")
#define DEBUG_NEW_API_PRINT(msg)                (DEBUG_NEW_API_RAW_PRINT_LOCATION(),DEBUG_NEW_API_RAW_PRINT(msg))
#define DEBUG_NEW_API_PRINTF(...)               (DEBUG_NEW_API_RAW_PRINT_LOCATION(),DEBUG_NEW_API_RAW_PRINTF(__VA_ARGS__))
#define DEBUG_NEW_API_VPRINTF(fmt,args)         (DEBUG_NEW_API_RAW_PRINT_LOCATION(),DEBUG_NEW_API_RAW_VPRINTF(fmt,args))

//////////////////////////////////////////////////////////////////////////
// <<Allocation breakpoints>>
//  - Set's up a breakpoint for a specific allocation:
//    """
//     DEBUG_NEW_API_ALLOC_BREAKPOINT(DEBUG_NEW_API_ALLOC_ID()+42);
//
//     // Some code, where the 42nd allocation leaks:
//     // debug_new will break into any running debugger and
//     // you can investigate the stack and local vars, too see,
//     // where exactly the problem lies.
//     ...
//
//     DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR(); 
//    """
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID && \
    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
#define DEBUG_NEW_API_ALLOC_BREAKPOINT           DEBUG_NEW_CXX_NAMESPACE_ROOT DnGetAllocBreakpointSet
#define DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR     DEBUG_NEW_CXX_NAMESPACE_ROOT DnGetAllocBreakpointClear
#define DEBUG_NEW_API_ALLOC_ID                   DEBUG_NEW_CXX_NAMESPACE_ROOT DnGetAllocID
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */
#define DEBUG_NEW_API_ALLOC_BREAKPOINT(id)       DEBUG_NEW_PRIVATE_EMPTY_EXPR
#define DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR(id) DEBUG_NEW_PRIVATE_EMPTY_EXPR
#define DEBUG_NEW_API_ALLOC_ID()                 0
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

#endif /* DEBUG_NEW_CONFIG_ENABLED */


//////////////////////////////////////////////////////////////////////////
// Fallback implementation linkage (link the debug api agains the native api)
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Native --> Debug fallback implementation
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#ifdef __cplusplus
#ifndef DEBUG_NEW_CXX_PLACEMENT
#define DEBUG_NEW_CXX_PLACEMENT  /* nothing */
#endif /* !DEBUG_NEW_CXX_PLACEMENT */
#ifndef DEBUG_NEW_CXX_NEW
#define DEBUG_NEW_CXX_NEW        new
#endif /* !DEBUG_NEW_CXX_NEW */
#endif /* __cplusplus */
#ifndef DEBUG_NEW_API_MSTATE_PUSH
#define DEBUG_NEW_API_MSTATE_PUSH()                 1
#endif /* !DEBUG_NEW_API_MSTATE_PUSH */
#ifndef DEBUG_NEW_API_MSTATE_POP
#define DEBUG_NEW_API_MSTATE_POP()                  0
#endif /* !DEBUG_NEW_API_MSTATE_POP */
#ifndef DEBUG_NEW_API_MSTATE_GLOBAL_PUSH
#define DEBUG_NEW_API_MSTATE_GLOBAL_PUSH()          1
#endif /* !DEBUG_NEW_API_MSTATE_GLOBAL_PUSH */
#ifndef DEBUG_NEW_API_MSTATE_GLOBAL_POP
#define DEBUG_NEW_API_MSTATE_GLOBAL_POP()           0
#endif /* !DEBUG_NEW_API_MSTATE_GLOBAL_POP */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,ac)    DEBUG_NEW_API_NATIVE_MALLOC(s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(s,ac) DEBUG_NEW_API_NATIVE_MALLOC_NZ(s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_FREE
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE(p,ac)      DEBUG_NEW_API_NATIVE_FREE(p)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_FREE */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN(p,ac)   DEBUG_NEW_API_NATIVE_FREE_NN(p)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,ac) DEBUG_NEW_API_NATIVE_REALLOC(p,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ(p,s,ac) DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN(p,s,ac) DEBUG_NEW_API_NATIVE_REALLOC_NN(p,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ(p,s,ac) DEBUG_NEW_API_NATIVE_REALLOC_NZ(p,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC(n,s,ac)  DEBUG_NEW_API_NATIVE_CALLOC(n,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,s,ac) DEBUG_NEW_API_NATIVE_CALLOC_NZ(n,s)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ */
#ifndef DEBUG_NEW_API_MALLOC
#define DEBUG_NEW_API_MALLOC                        DEBUG_NEW_API_NATIVE_MALLOC
#endif /* !DEBUG_NEW_API_MALLOC */
#ifndef DEBUG_NEW_API_MALLOC_NZ
#define DEBUG_NEW_API_MALLOC_NZ                     DEBUG_NEW_API_NATIVE_MALLOC_NZ
#endif /* !DEBUG_NEW_API_MALLOC_NZ */
#ifndef DEBUG_NEW_API_FREE
#define DEBUG_NEW_API_FREE                          DEBUG_NEW_API_NATIVE_FREE
#endif /* !DEBUG_NEW_API_FREE */
#ifndef DEBUG_NEW_API_FREE_NN
#define DEBUG_NEW_API_FREE_NN                       DEBUG_NEW_API_NATIVE_FREE_NN
#endif /* !DEBUG_NEW_API_FREE_NN */
#ifndef DEBUG_NEW_API_REALLOC
#define DEBUG_NEW_API_REALLOC                       DEBUG_NEW_API_NATIVE_REALLOC
#endif /* !DEBUG_NEW_API_REALLOC */
#ifndef DEBUG_NEW_API_REALLOC_NNZ
#define DEBUG_NEW_API_REALLOC_NNZ                   DEBUG_NEW_API_NATIVE_REALLOC_NNZ
#endif /* !DEBUG_NEW_API_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_REALLOC_NN
#define DEBUG_NEW_API_REALLOC_NN                    DEBUG_NEW_API_NATIVE_REALLOC_NN
#endif /* !DEBUG_NEW_API_REALLOC_NN */
#ifndef DEBUG_NEW_API_REALLOC_NZ
#define DEBUG_NEW_API_REALLOC_NZ                    DEBUG_NEW_API_NATIVE_REALLOC_NZ
#endif /* !DEBUG_NEW_API_REALLOC_NZ */
#ifndef DEBUG_NEW_API_CALLOC
#define DEBUG_NEW_API_CALLOC                        DEBUG_NEW_API_NATIVE_CALLOC
#endif /* !DEBUG_NEW_API_CALLOC */
#ifndef DEBUG_NEW_API_CALLOC_NZ
#define DEBUG_NEW_API_CALLOC_NZ                     DEBUG_NEW_API_NATIVE_CALLOC_NZ
#endif /* !DEBUG_NEW_API_CALLOC_NZ */
#ifdef DEBUG_NEW_API_NATIVE_MALLOC_MSIZE
#ifndef DEBUG_NEW_API_MALLOC_MSIZE
#define DEBUG_NEW_API_MALLOC_MSIZE                  DEBUG_NEW_API_NATIVE_MALLOC_MSIZE
#endif /* !DEBUG_NEW_API_MALLOC_MSIZE */
#ifndef DEBUG_NEW_API_MALLOC_MSIZE_C
#define DEBUG_NEW_API_MALLOC_MSIZE_C                DEBUG_NEW_API_NATIVE_MALLOC_MSIZE
#endif /* !DEBUG_NEW_API_MALLOC_MSIZE_C */
#ifndef DEBUG_NEW_API_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_MALLOC_MSIZE_NN               DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN
#endif /* !DEBUG_NEW_API_MALLOC_MSIZE_NN */
#ifndef DEBUG_NEW_API_MALLOC_MSIZE_C_NN
#define DEBUG_NEW_API_MALLOC_MSIZE_C_NN             DEBUG_NEW_API_NATIVE_MALLOC_MSIZE_NN
#endif /* !DEBUG_NEW_API_MALLOC_MSIZE_C_NN */
#endif /* DEBUG_NEW_API_NATIVE_MALLOC_MSIZE */
#ifndef DEBUG_NEW_API_STRDUP
#define DEBUG_NEW_API_STRDUP                        DEBUG_NEW_API_NATIVE_STRDUP
#endif /* !DEBUG_NEW_API_STRDUP */
#ifndef DEBUG_NEW_API_STRDUP_NZ
#define DEBUG_NEW_API_STRDUP_NZ                     DEBUG_NEW_API_NATIVE_STRDUP_NZ
#endif /* !DEBUG_NEW_API_STRDUP_NZ */
#ifndef DEBUG_NEW_API_STRNDUP
#define DEBUG_NEW_API_STRNDUP                       DEBUG_NEW_API_NATIVE_STRNDUP
#endif /* !DEBUG_NEW_API_STRNDUP */
#ifndef DEBUG_NEW_API_STRNDUP_NZ
#define DEBUG_NEW_API_STRNDUP_NZ                    DEBUG_NEW_API_NATIVE_STRNDUP_NZ
#endif /* !DEBUG_NEW_API_STRNDUP_NZ */
#ifndef DEBUG_NEW_API_WCSDUP
#define DEBUG_NEW_API_WCSDUP                        DEBUG_NEW_API_NATIVE_WCSDUP
#endif /* !DEBUG_NEW_API_WCSDUP */
#ifndef DEBUG_NEW_API_WCSDUP_NZ
#define DEBUG_NEW_API_WCSDUP_NZ                     DEBUG_NEW_API_NATIVE_WCSDUP_NZ
#endif /* !DEBUG_NEW_API_WCSDUP_NZ */
#ifndef DEBUG_NEW_API_WCSNDUP
#define DEBUG_NEW_API_WCSNDUP                       DEBUG_NEW_API_NATIVE_WCSNDUP
#endif /* !DEBUG_NEW_API_WCSNDUP */
#ifndef DEBUG_NEW_API_WCSNDUP_NZ
#define DEBUG_NEW_API_WCSNDUP_NZ                    DEBUG_NEW_API_NATIVE_WCSNDUP_NZ
#endif /* !DEBUG_NEW_API_WCSNDUP_NZ */
#ifndef DEBUG_NEW_API_MBSDUP
#define DEBUG_NEW_API_MBSDUP                        DEBUG_NEW_API_NATIVE_MBSDUP
#endif /* !DEBUG_NEW_API_MBSDUP */
#ifndef DEBUG_NEW_API_MBSDUP_NZ
#define DEBUG_NEW_API_MBSDUP_NZ                     DEBUG_NEW_API_NATIVE_MBSDUP_NZ
#endif /* !DEBUG_NEW_API_MBSDUP_NZ */
#ifndef DEBUG_NEW_API_MBSNDUP
#define DEBUG_NEW_API_MBSNDUP                       DEBUG_NEW_API_NATIVE_MBSNDUP
#endif /* !DEBUG_NEW_API_MBSNDUP */
#ifndef DEBUG_NEW_API_MBSNDUP_NZ
#define DEBUG_NEW_API_MBSNDUP_NZ                    DEBUG_NEW_API_NATIVE_MBSNDUP_NZ
#endif /* !DEBUG_NEW_API_MBSNDUP_NZ */
#ifndef DEBUG_NEW_API_MALLOCF
#define DEBUG_NEW_API_MALLOCF(s,...)                DEBUG_NEW_API_NATIVE_MALLOC(s)
#endif /* !DEBUG_NEW_API_MALLOCF */
#ifndef DEBUG_NEW_API_MALLOCF_NZ
#define DEBUG_NEW_API_MALLOCF_NZ(s,...)             DEBUG_NEW_API_NATIVE_MALLOC_NZ(s)
#endif /* !DEBUG_NEW_API_MALLOCF_NZ */
#ifndef DEBUG_NEW_API_REALLOCF
#define DEBUG_NEW_API_REALLOCF(p,s,...)             DEBUG_NEW_API_NATIVE_REALLOC(p,s)
#endif /* !DEBUG_NEW_API_REALLOCF */
#ifndef DEBUG_NEW_API_REALLOCF_NZ
#define DEBUG_NEW_API_REALLOCF_NZ(p,s,...)          DEBUG_NEW_API_NATIVE_REALLOC_NZ(p,s)
#endif /* !DEBUG_NEW_API_REALLOCF_NZ */
#ifndef DEBUG_NEW_API_REALLOCF_NN
#define DEBUG_NEW_API_REALLOCF_NN(p,s,...)          DEBUG_NEW_API_NATIVE_REALLOC_NN(p,s)
#endif /* !DEBUG_NEW_API_REALLOCF_NN */
#ifndef DEBUG_NEW_API_REALLOCF_NNZ
#define DEBUG_NEW_API_REALLOCF_NNZ(p,s,...)         DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s)
#endif /* !DEBUG_NEW_API_REALLOCF_NNZ */
#ifndef DEBUG_NEW_API_CALLOCF
#define DEBUG_NEW_API_CALLOCF(n,s,...)              DEBUG_NEW_API_NATIVE_CALLOC(n,s)
#endif /* !DEBUG_NEW_API_CALLOCF */
#ifndef DEBUG_NEW_API_CALLOCF_NZ
#define DEBUG_NEW_API_CALLOCF_NZ(n,s,...)           DEBUG_NEW_API_NATIVE_CALLOC_NZ(n,s)
#endif /* !DEBUG_NEW_API_CALLOCF_NZ */
#ifndef DEBUG_NEW_API_VMALLOCF
#define DEBUG_NEW_API_VMALLOCF(s,fmt,args)          DEBUG_NEW_API_NATIVE_MALLOC(s)
#endif /* !DEBUG_NEW_API_VMALLOCF */
#ifndef DEBUG_NEW_API_VMALLOCF_NZ
#define DEBUG_NEW_API_VMALLOCF_NZ(s,fmt,args)       DEBUG_NEW_API_NATIVE_MALLOC_NZ(s)
#endif /* !DEBUG_NEW_API_VMALLOCF_NZ */
#ifndef DEBUG_NEW_API_VREALLOCF
#define DEBUG_NEW_API_VREALLOCF(p,s,fmt,args)       DEBUG_NEW_API_NATIVE_REALLOC(p,s)
#endif /* !DEBUG_NEW_API_VREALLOCF */
#ifndef DEBUG_NEW_API_VREALLOCF_NZ
#define DEBUG_NEW_API_VREALLOCF_NZ(p,s,fmt,args)    DEBUG_NEW_API_NATIVE_REALLOC_NZ(p,s)
#endif /* !DEBUG_NEW_API_VREALLOCF_NZ */
#ifndef DEBUG_NEW_API_VREALLOCF_NN
#define DEBUG_NEW_API_VREALLOCF_NN(p,s,fmt,args)    DEBUG_NEW_API_NATIVE_REALLOC_NN(p,s)
#endif /* !DEBUG_NEW_API_VREALLOCF_NN */
#ifndef DEBUG_NEW_API_VREALLOCF_NNZ
#define DEBUG_NEW_API_VREALLOCF_NNZ(p,s,fmt,args)   DEBUG_NEW_API_NATIVE_REALLOC_NNZ(p,s)
#endif /* !DEBUG_NEW_API_VREALLOCF_NNZ */
#ifndef DEBUG_NEW_API_VCALLOCF
#define DEBUG_NEW_API_VCALLOCF(n,s,fmt,args)        DEBUG_NEW_API_NATIVE_CALLOC(n,s)
#endif /* !DEBUG_NEW_API_VCALLOCF */
#ifndef DEBUG_NEW_API_VCALLOCF_NZ
#define DEBUG_NEW_API_VCALLOCF_NZ(n,s,fmt,args)     DEBUG_NEW_API_NATIVE_CALLOC_NZ(n,s)
#endif /* !DEBUG_NEW_API_VCALLOCF_NZ */

#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC(s,a,ac)                 DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(s,a,ac)              DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC(p,s,a,ac)              DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC(p,s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(p,s,a,ac)           DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN(p,s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(p,s,a,ac)           DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ(p,s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(p,s,a,ac)          DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC(s,a,ac)                 DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC(s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(s,a,ac)              DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(s,a,o,ac)        DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(s,a,o,ac)     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(p,s,a,o,ac)     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC(p,s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o,ac)  DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o,ac)  DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o,ac) DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(s,a,o,ac)        DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(s,a,o,ac)     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE(p,ac)                     DEBUG_NEW_API_NATIVE_ALIGNED_FREE(p)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE */
#ifndef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN(p,ac)                  DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN(p)
#endif /* !DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE                                 DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE */
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN                              DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE                          DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE */
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN                       DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#endif /* DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOC
#define DEBUG_NEW_API_ALIGNED_MALLOC                                       DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_MALLOC_NZ                                    DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOC
#define DEBUG_NEW_API_ALIGNED_REALLOC                                      DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOC_NN
#define DEBUG_NEW_API_ALIGNED_REALLOC_NN                                   DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOC_NN */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_REALLOC_NZ                                   DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOC_NNZ
#define DEBUG_NEW_API_ALIGNED_REALLOC_NNZ                                  DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_ALIGNED_CALLOC
#define DEBUG_NEW_API_ALIGNED_CALLOC                                       DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_CALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_CALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_CALLOC_NZ                                    DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_CALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC                                DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ                             DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC                               DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN                            DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ                            DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ                           DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC                                DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ                             DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_FREE
#define DEBUG_NEW_API_ALIGNED_FREE                                         DEBUG_NEW_API_NATIVE_ALIGNED_FREE
#endif /* !DEBUG_NEW_API_ALIGNED_FREE */
#ifndef DEBUG_NEW_API_ALIGNED_FREE_NN
#define DEBUG_NEW_API_ALIGNED_FREE_NN                                      DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN
#endif /* !DEBUG_NEW_API_ALIGNED_FREE_NN */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOCF
#define DEBUG_NEW_API_ALIGNED_MALLOCF(s,a,...)                             DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_MALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ(s,a,...)                          DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_MALLOCF_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOCF
#define DEBUG_NEW_API_ALIGNED_REALLOCF(p,s,a,...)                          DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC(p,s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ(p,s,a,...)                       DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN(p,s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOCF_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOCF_NN
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NN(p,s,a,...)                       DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ(p,s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOCF_NN */
#ifndef DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ(p,s,a,...)                      DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ */
#ifndef DEBUG_NEW_API_ALIGNED_CALLOCF
#define DEBUG_NEW_API_ALIGNED_CALLOCF(s,a,...)                             DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC(s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_CALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_CALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ(s,a,...)                          DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_ALIGNED_CALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_MALLOCF
#define DEBUG_NEW_API_VALIGNED_MALLOCF(s,a,fmt,args)                       DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_MALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_MALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ(s,a,fmt,args)                    DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_MALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_REALLOCF
#define DEBUG_NEW_API_VALIGNED_REALLOCF(p,s,a,fmt,args)                    DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC(p,s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_REALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_REALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ(p,s,a,fmt,args)                 DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN(p,s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_REALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_REALLOCF_NN
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NN(p,s,a,fmt,args)                 DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ(p,s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_REALLOCF_NN */
#ifndef DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ(p,s,a,fmt,args)                DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ */
#ifndef DEBUG_NEW_API_VALIGNED_CALLOCF
#define DEBUG_NEW_API_VALIGNED_CALLOCF(s,a,fmt,args)                       DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC(s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_CALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_CALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ(s,a,fmt,args)                    DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a)
#endif /* !DEBUG_NEW_API_VALIGNED_CALLOCF_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF(s,a,o,...)                    DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ(s,a,o,...)                 DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF(p,s,a,o,...)                 DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC(p,s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,...)              DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,...)              DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,...)             DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF(s,a,o,...)                    DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF */
#ifndef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ(s,a,o,...)                 DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF(s,a,o,fmt,args)              DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ(s,a,o,fmt,args)           DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF(p,s,a,o,fmt,args)           DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC(p,s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ(p,s,a,o,fmt,args)        DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN(p,s,a,o,fmt,args)        DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ(p,s,a,o,fmt,args)       DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF(s,a,o,fmt,args)              DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC(s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF */
#ifndef DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ(s,a,o,fmt,args)           DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
#endif /* !DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

#ifndef DEBUG_NEW_API_POINTER_GET_FILE
#define DEBUG_NEW_API_POINTER_GET_FILE(p)           ((char const*)NULL)
#endif /* !DEBUG_NEW_API_POINTER_GET_FILE */
#ifndef DEBUG_NEW_API_POINTER_GET_LINE
#define DEBUG_NEW_API_POINTER_GET_LINE(p)           ((int)-1)
#endif /* !DEBUG_NEW_API_POINTER_GET_LINE */
#ifndef DEBUG_NEW_API_POINTER_GET_LINE_STR
#define DEBUG_NEW_API_POINTER_GET_LINE_STR(p)       ((char const*)NULL)
#endif /* !DEBUG_NEW_API_POINTER_GET_LINE_STR */
#ifndef DEBUG_NEW_API_POINTER_GET_FUNC
#define DEBUG_NEW_API_POINTER_GET_FUNC(p)           ((char const*)NULL)
#endif /* !DEBUG_NEW_API_POINTER_GET_FUNC */
#ifndef DEBUG_NEW_API_POINTER_GET_EXPR
#define DEBUG_NEW_API_POINTER_GET_EXPR(p)           ((char const*)NULL)
#endif /* !DEBUG_NEW_API_POINTER_GET_EXPR */
#ifndef DEBUG_NEW_API_POINTER_GET_MSG
#define DEBUG_NEW_API_POINTER_GET_MSG(p)            ((char const*)NULL)
#endif /* !DEBUG_NEW_API_POINTER_GET_MSG */
#ifndef DEBUG_NEW_API_POINTER_GET_ID
#define DEBUG_NEW_API_POINTER_GET_ID(p)             ((size_t)0)
#endif /* !DEBUG_NEW_API_POINTER_GET_ID */
#ifndef DEBUG_NEW_API_CHECK_PTR
#define DEBUG_NEW_API_CHECK_PTR(p)                  1
#endif /* !DEBUG_NEW_API_CHECK_PTR */
#ifndef DEBUG_NEW_API_PRINTFUNC_PUSH
#define DEBUG_NEW_API_PRINTFUNC_PUSH(func)          1
#endif /* !DEBUG_NEW_API_PRINTFUNC_PUSH */
#ifndef DEBUG_NEW_API_PRINTFUNC_POP
#define DEBUG_NEW_API_PRINTFUNC_POP()               1
#endif /* !DEBUG_NEW_API_PRINTFUNC_POP */
#ifndef DEBUG_NEW_API_PRINTFUNC_STACK_SIZE
#define DEBUG_NEW_API_PRINTFUNC_STACK_SIZE()        0
#endif /* !DEBUG_NEW_API_PRINTFUNC_STACK_SIZE */
#ifndef DEBUG_NEW_API_RAW_PRINT
#define DEBUG_NEW_API_RAW_PRINT(msg)                DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_RAW_PRINT */
#ifndef DEBUG_NEW_API_RAW_PRINTF
#define DEBUG_NEW_API_RAW_PRINTF(...)               DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_RAW_PRINTF */
#ifndef DEBUG_NEW_API_RAW_VPRINTF
#define DEBUG_NEW_API_RAW_VPRINTF(fmt,args)         DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_RAW_VPRINTF */
#ifndef DEBUG_NEW_API_RAW_PRINT_LOCATION
#define DEBUG_NEW_API_RAW_PRINT_LOCATION()          DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_RAW_PRINT_LOCATION */
#ifndef DEBUG_NEW_API_PRINT
#define DEBUG_NEW_API_PRINT(msg)                    DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_PRINT */
#ifndef DEBUG_NEW_API_PRINTF
#define DEBUG_NEW_API_PRINTF(...)                   DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_PRINTF */
#ifndef DEBUG_NEW_API_VPRINTF
#define DEBUG_NEW_API_VPRINTF(fmt,args)             DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_VPRINTF */
#ifndef DEBUG_NEW_API_ALLOC_BREAKPOINT
#define DEBUG_NEW_API_ALLOC_BREAKPOINT(id)          DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_ALLOC_BREAKPOINT */
#ifndef DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR
#define DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR()      DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR */
#ifndef DEBUG_NEW_API_ALLOC_ID
#define DEBUG_NEW_API_ALLOC_ID()                    0
#endif /* !DEBUG_NEW_API_ALLOC_ID */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */

// Replace the the debug api with a fake, but nice looking documentation API
// Used, to go easy on intellisense or other documentation generators, by removing all the access stuff
// NOTE: This portion isn't *real* code, that would ever get into any sort of build!
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Documentation API Overrides (Sal + Intellisense)
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_DOC_ONLY
#ifdef DEBUG_NEW_CXX_PLACEMENT
#undef DEBUG_NEW_CXX_NEW
#if defined(__cplusplus) // should always be true...
// A bit more guarding, to only allow "placement" infront of pointers... (It's something)
struct DEBUG_NEW_DOC_API_CXX_PLACEMENT {
 template<class T> inline DEBUG_NEW_CXX11_CONSTEXPR DEBUG_NEW_ANNOTATION_RETURNS(p)
 T *operator ->* (T *p) const DEBUG_NEW_CXX11_NOEXCEPT { return p; }
 //template<class T> inline DEBUG_NEW_CXX11_CONSTEXPR DEBUG_NEW_ANNOTATION_RETURNS(p)
 //T const *operator ->* (T const *p) const DEBUG_NEW_CXX11_NOEXCEPT { return p; }
};
#undef DEBUG_NEW_CXX_PLACEMENT
#define DEBUG_NEW_CXX_PLACEMENT  ::DEBUG_NEW_DOC_API_CXX_PLACEMENT()->*
#endif
#define DEBUG_NEW_CXX_NEW        new
#endif /* DEBUG_NEW_CXX_PLACEMENT */

#undef DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_FREE
#undef DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN
#undef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN
#undef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ
#undef DEBUG_NEW_API_MALLOC
#undef DEBUG_NEW_API_MALLOC_NZ
#undef DEBUG_NEW_API_FREE
#undef DEBUG_NEW_API_FREE_NN
#undef DEBUG_NEW_API_REALLOC
#undef DEBUG_NEW_API_REALLOC_NN
#undef DEBUG_NEW_API_REALLOC_NZ
#undef DEBUG_NEW_API_REALLOC_NNZ
#undef DEBUG_NEW_API_CALLOC
#undef DEBUG_NEW_API_CALLOC_NZ
#undef DEBUG_NEW_API_STRDUP
#undef DEBUG_NEW_API_STRDUP_NZ
#undef DEBUG_NEW_API_WCSDUP
#undef DEBUG_NEW_API_WCSDUP_NZ
#undef DEBUG_NEW_API_MBSDUP
#undef DEBUG_NEW_API_MBSDUP_NZ
#undef DEBUG_NEW_API_STRNDUP
#undef DEBUG_NEW_API_STRNDUP_NZ
#undef DEBUG_NEW_API_WCSNDUP
#undef DEBUG_NEW_API_WCSNDUP_NZ
#undef DEBUG_NEW_API_MBSNDUP
#undef DEBUG_NEW_API_MBSNDUP_NZ
#undef DEBUG_NEW_API_MALLOC_MSIZE
#undef DEBUG_NEW_API_MALLOC_MSIZE_C
#undef DEBUG_NEW_API_MALLOC_MSIZE_NN
#undef DEBUG_NEW_API_MALLOC_MSIZE_C_NN
#undef DEBUG_NEW_API_MALLOCF
#undef DEBUG_NEW_API_MALLOCF_NZ
#undef DEBUG_NEW_API_REALLOCF
#undef DEBUG_NEW_API_REALLOCF_NZ
#undef DEBUG_NEW_API_REALLOCF_NN
#undef DEBUG_NEW_API_REALLOCF_NNZ
#undef DEBUG_NEW_API_CALLOCF
#undef DEBUG_NEW_API_CALLOCF_NZ
#undef DEBUG_NEW_API_VMALLOCF
#undef DEBUG_NEW_API_VMALLOCF_NZ
#undef DEBUG_NEW_API_VREALLOCF
#undef DEBUG_NEW_API_VREALLOCF_NZ
#undef DEBUG_NEW_API_VREALLOCF_NN
#undef DEBUG_NEW_API_VREALLOCF_NNZ
#undef DEBUG_NEW_API_VCALLOCF
#undef DEBUG_NEW_API_VCALLOCF_NZ
#undef DEBUG_NEW_API_MSTATE_PUSH
#undef DEBUG_NEW_API_MSTATE_POP
#undef DEBUG_NEW_API_MSTATE_GLOBAL_PUSH
#undef DEBUG_NEW_API_MSTATE_GLOBAL_POP
#undef DEBUG_NEW_API_POINTER_GET_FILE
#undef DEBUG_NEW_API_POINTER_GET_LINE
#undef DEBUG_NEW_API_POINTER_GET_LINE_STR
#undef DEBUG_NEW_API_POINTER_GET_FUNC
#undef DEBUG_NEW_API_POINTER_GET_EXPR
#undef DEBUG_NEW_API_POINTER_GET_MSG
#undef DEBUG_NEW_API_POINTER_GET_ID
#undef DEBUG_NEW_API_CHECK_PTR
#undef DEBUG_NEW_API_PRINTFUNC_PUSH
#undef DEBUG_NEW_API_PRINTFUNC_POP
#undef DEBUG_NEW_API_PRINTFUNC_STACK_SIZE
#undef DEBUG_NEW_API_RAW_PRINT
#undef DEBUG_NEW_API_RAW_PRINTF
#undef DEBUG_NEW_API_RAW_VPRINTF
#undef DEBUG_NEW_API_RAW_PRINT_LOCATION
#undef DEBUG_NEW_API_PRINT
#undef DEBUG_NEW_API_PRINTF
#undef DEBUG_NEW_API_VPRINTF
#undef DEBUG_NEW_API_ALLOC_BREAKPOINT
#undef DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR
#undef DEBUG_NEW_API_ALLOC_ID

DEBUG_NEW_ANNOTATION_MALLOC(s)        void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_MALLOC(size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_MALLOC_NZ(size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_FREE(p)          void DEBUG_NEW_DOC_API_CUSTOM_ACCESS_FREE(void *p, char const *ac);
DEBUG_NEW_ANNOTATION_FREE_NN(p)       void DEBUG_NEW_DOC_API_CUSTOM_ACCESS_FREE_NN(void *p, char const *ac);
DEBUG_NEW_ANNOTATION_REALLOC(p,s)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC(void *p, size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NN(void *p, size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NZ(void *p, size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NNZ(void *p, size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_CALLOC(n,s)      void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_CALLOC(size_t n, size_t s, char const *ac);
DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s)   void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_CALLOC_NZ(size_t n, size_t s, char const *ac);
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC      DEBUG_NEW_DOC_API_CUSTOM_ACCESS_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ   DEBUG_NEW_DOC_API_CUSTOM_ACCESS_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE        DEBUG_NEW_DOC_API_CUSTOM_ACCESS_FREE
#define DEBUG_NEW_API_CUSTOM_ACCESS_FREE_NN     DEBUG_NEW_DOC_API_CUSTOM_ACCESS_FREE_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC     DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NN  DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NZ  DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC_NNZ DEBUG_NEW_DOC_API_CUSTOM_ACCESS_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC      DEBUG_NEW_DOC_API_CUSTOM_ACCESS_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ   DEBUG_NEW_DOC_API_CUSTOM_ACCESS_CALLOC_NZ

DEBUG_NEW_ANNOTATION_MALLOC(s)        void *DEBUG_NEW_DOC_API_MALLOC(size_t s);
DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)     void *DEBUG_NEW_DOC_API_MALLOC_NZ(size_t s);
DEBUG_NEW_ANNOTATION_FREE(p)          void DEBUG_NEW_DOC_API_FREE(void *p);
DEBUG_NEW_ANNOTATION_FREE_NN(p)       void DEBUG_NEW_DOC_API_FREE_NN(void *p);
DEBUG_NEW_ANNOTATION_REALLOC(p,s)     void *DEBUG_NEW_DOC_API_REALLOC(void *p, size_t s);
DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)  void *DEBUG_NEW_DOC_API_REALLOC_NN(void *p, size_t s);
DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)  void *DEBUG_NEW_DOC_API_REALLOC_NZ(void *p, size_t s);
DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *DEBUG_NEW_DOC_API_REALLOC_NNZ(void *p, size_t s);
DEBUG_NEW_ANNOTATION_CALLOC(n,s)      void *DEBUG_NEW_DOC_API_CALLOC(size_t n, size_t s);
DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s)   void *DEBUG_NEW_DOC_API_CALLOC_NZ(size_t n, size_t s);
#define DEBUG_NEW_API_MALLOC      DEBUG_NEW_DOC_API_MALLOC
#define DEBUG_NEW_API_MALLOC_NZ   DEBUG_NEW_DOC_API_MALLOC_NZ
#define DEBUG_NEW_API_FREE        DEBUG_NEW_DOC_API_FREE
#define DEBUG_NEW_API_FREE_NN     DEBUG_NEW_DOC_API_FREE_NN
#define DEBUG_NEW_API_REALLOC     DEBUG_NEW_DOC_API_REALLOC
#define DEBUG_NEW_API_REALLOC_NN  DEBUG_NEW_DOC_API_REALLOC_NN
#define DEBUG_NEW_API_REALLOC_NZ  DEBUG_NEW_DOC_API_REALLOC_NZ
#define DEBUG_NEW_API_REALLOC_NNZ DEBUG_NEW_DOC_API_REALLOC_NNZ
#define DEBUG_NEW_API_CALLOC      DEBUG_NEW_DOC_API_CALLOC
#define DEBUG_NEW_API_CALLOC_NZ   DEBUG_NEW_DOC_API_CALLOC_NZ

DEBUG_NEW_ANNOTATION_STRDUP(s)       char *DEBUG_NEW_DOC_API_STRDUP(char const *s);
DEBUG_NEW_ANNOTATION_STRDUP_NZ(s)    char *DEBUG_NEW_DOC_API_STRDUP_NZ(char const *s);
DEBUG_NEW_ANNOTATION_STRDUP(s)       DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t *DEBUG_NEW_DOC_API_WCSDUP(DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t const *s);
DEBUG_NEW_ANNOTATION_STRDUP_NZ(s)    DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t *DEBUG_NEW_DOC_API_WCSDUP_NZ(DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t const *s);
DEBUG_NEW_ANNOTATION_STRDUP(s)       unsigned char *DEBUG_NEW_DOC_API_MBSDUP(unsigned char const *s);
DEBUG_NEW_ANNOTATION_STRDUP_NZ(s)    unsigned char *DEBUG_NEW_DOC_API_MBSDUP_NZ(unsigned char const *s);
DEBUG_NEW_ANNOTATION_STRNDUP(s,n)    char *DEBUG_NEW_DOC_API_STRNDUP(char const *s, size_t n);
DEBUG_NEW_ANNOTATION_STRNDUP_NZ(s,n) char *DEBUG_NEW_DOC_API_STRNDUP_NZ(char const *s, size_t n);
DEBUG_NEW_ANNOTATION_STRNDUP(s,n)    DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t *DEBUG_NEW_DOC_API_WCSNDUP(DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t const *s, size_t n);
DEBUG_NEW_ANNOTATION_STRNDUP_NZ(s,n) DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t *DEBUG_NEW_DOC_API_WCSNDUP_NZ(DEBUG_NEW_CXX_NAMESPACE_ROOT DnWchar_t const *s, size_t n);
DEBUG_NEW_ANNOTATION_STRNDUP(s,n)    unsigned char *DEBUG_NEW_DOC_API_MBSNDUP(unsigned char const *s, size_t n);
DEBUG_NEW_ANNOTATION_STRNDUP_NZ(s,n) unsigned char *DEBUG_NEW_DOC_API_MBSNDUP_NZ(unsigned char const *s, size_t n);
#define DEBUG_NEW_API_STRDUP      DEBUG_NEW_DOC_API_STRDUP
#define DEBUG_NEW_API_STRDUP_NZ   DEBUG_NEW_DOC_API_STRDUP_NZ
#define DEBUG_NEW_API_WCSDUP      DEBUG_NEW_DOC_API_WCSDUP
#define DEBUG_NEW_API_WCSDUP_NZ   DEBUG_NEW_DOC_API_WCSDUP_NZ
#define DEBUG_NEW_API_MBSDUP      DEBUG_NEW_DOC_API_MBSDUP
#define DEBUG_NEW_API_MBSDUP_NZ   DEBUG_NEW_DOC_API_MBSDUP_NZ
#define DEBUG_NEW_API_STRNDUP     DEBUG_NEW_DOC_API_STRNDUP
#define DEBUG_NEW_API_STRNDUP_NZ  DEBUG_NEW_DOC_API_STRNDUP_NZ
#define DEBUG_NEW_API_WCSNDUP     DEBUG_NEW_DOC_API_WCSNDUP
#define DEBUG_NEW_API_WCSNDUP_NZ  DEBUG_NEW_DOC_API_WCSNDUP_NZ
#define DEBUG_NEW_API_MBSNDUP     DEBUG_NEW_DOC_API_MBSNDUP
#define DEBUG_NEW_API_MBSNDUP_NZ  DEBUG_NEW_DOC_API_MBSNDUP_NZ

DEBUG_NEW_ANNOTATION_MSIZE(p)    size_t DEBUG_NEW_DOC_API_MALLOC_MSIZE(void *p);
DEBUG_NEW_ANNOTATION_MSIZE(p)    size_t DEBUG_NEW_DOC_API_MALLOC_MSIZE_C(void const *p);
DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t DEBUG_NEW_DOC_API_MALLOC_MSIZE_NN(void *p);
DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t DEBUG_NEW_DOC_API_MALLOC_MSIZE_C_NN(void const *p);
#define DEBUG_NEW_API_MALLOC_MSIZE      DEBUG_NEW_DOC_API_MALLOC_MSIZE
#define DEBUG_NEW_API_MALLOC_MSIZE_C    DEBUG_NEW_DOC_API_MALLOC_MSIZE_C
#define DEBUG_NEW_API_MALLOC_MSIZE_NN   DEBUG_NEW_DOC_API_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_MALLOC_MSIZE_C_NN DEBUG_NEW_DOC_API_MALLOC_MSIZE_C_NN

DEBUG_NEW_ANNOTATION_MALLOC(s)        void *DEBUG_NEW_DOC_API_MALLOCF(size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)     void *DEBUG_NEW_DOC_API_MALLOCF_NZ(size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_REALLOC(p,s)     void *DEBUG_NEW_DOC_API_REALLOCF(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)  void *DEBUG_NEW_DOC_API_REALLOCF_NN(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)  void *DEBUG_NEW_DOC_API_REALLOCF_NZ(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *DEBUG_NEW_DOC_API_REALLOCF_NNZ(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_CALLOC(n,s)      void *DEBUG_NEW_DOC_API_CALLOCF(size_t n, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s)   void *DEBUG_NEW_DOC_API_CALLOCF_NZ(size_t n, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_MALLOC(s)        void *DEBUG_NEW_DOC_API_VMALLOCF(size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_MALLOC_NZ(s)     void *DEBUG_NEW_DOC_API_VMALLOCF_NZ(size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_REALLOC(p,s)     void *DEBUG_NEW_DOC_API_VREALLOCF(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s)  void *DEBUG_NEW_DOC_API_VREALLOCF_NN(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s)  void *DEBUG_NEW_DOC_API_VREALLOCF_NZ(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *DEBUG_NEW_DOC_API_VREALLOCF_NNZ(void *p, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_CALLOC(n,s)      void *DEBUG_NEW_DOC_API_VCALLOCF(size_t n, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s)   void *DEBUG_NEW_DOC_API_VCALLOCF_NZ(size_t n, size_t s, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#define DEBUG_NEW_API_MALLOCF       DEBUG_NEW_DOC_API_MALLOCF
#define DEBUG_NEW_API_MALLOCF_NZ    DEBUG_NEW_DOC_API_MALLOCF_NZ
#define DEBUG_NEW_API_REALLOCF      DEBUG_NEW_DOC_API_REALLOCF
#define DEBUG_NEW_API_REALLOCF_NZ   DEBUG_NEW_DOC_API_REALLOCF_NZ
#define DEBUG_NEW_API_REALLOCF_NN   DEBUG_NEW_DOC_API_REALLOCF_NN
#define DEBUG_NEW_API_REALLOCF_NNZ  DEBUG_NEW_DOC_API_REALLOCF_NNZ
#define DEBUG_NEW_API_CALLOCF       DEBUG_NEW_DOC_API_CALLOCF
#define DEBUG_NEW_API_CALLOCF_NZ    DEBUG_NEW_DOC_API_CALLOCF_NZ
#define DEBUG_NEW_API_VMALLOCF      DEBUG_NEW_DOC_API_VMALLOCF
#define DEBUG_NEW_API_VMALLOCF_NZ   DEBUG_NEW_DOC_API_VMALLOCF_NZ
#define DEBUG_NEW_API_VREALLOCF     DEBUG_NEW_DOC_API_VREALLOCF
#define DEBUG_NEW_API_VREALLOCF_NZ  DEBUG_NEW_DOC_API_VREALLOCF_NZ
#define DEBUG_NEW_API_VREALLOCF_NN  DEBUG_NEW_DOC_API_VREALLOCF_NN
#define DEBUG_NEW_API_VREALLOCF_NNZ DEBUG_NEW_DOC_API_VREALLOCF_NNZ
#define DEBUG_NEW_API_VCALLOCF      DEBUG_NEW_DOC_API_VCALLOCF
#define DEBUG_NEW_API_VCALLOCF_NZ   DEBUG_NEW_DOC_API_VCALLOCF_NZ

#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC
#undef DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ
#undef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#undef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#undef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#undef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#undef DEBUG_NEW_API_ALIGNED_MALLOCF
#undef DEBUG_NEW_API_ALIGNED_MALLOCF_NZ
#undef DEBUG_NEW_API_ALIGNED_REALLOCF
#undef DEBUG_NEW_API_ALIGNED_REALLOCF_NZ
#undef DEBUG_NEW_API_ALIGNED_REALLOCF_NN
#undef DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ
#undef DEBUG_NEW_API_ALIGNED_CALLOCF
#undef DEBUG_NEW_API_ALIGNED_CALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_MALLOCF
#undef DEBUG_NEW_API_VALIGNED_MALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_REALLOCF
#undef DEBUG_NEW_API_VALIGNED_REALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_REALLOCF_NN
#undef DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ
#undef DEBUG_NEW_API_VALIGNED_CALLOCF
#undef DEBUG_NEW_API_VALIGNED_CALLOCF_NZ
#undef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF
#undef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ
#undef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF
#undef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ
#undef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN
#undef DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ
#undef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF
#undef DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF
#undef DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF
#undef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ
#undef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN
#undef DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ
#undef DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF
#undef DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ

DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)        void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_MALLOC(size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ(size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p,a)          void DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_FREE(void *p, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p,a)       void DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_FREE_NN(void *p, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ(void *p, size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)        void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_CALLOC(size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ(size_t s, size_t a, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)  void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ(void *p, size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC(size_t s, size_t a, size_t o, char const *ac);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ(size_t s, size_t a, size_t o, char const *ac);
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC             DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ          DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE               DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_FREE
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_FREE_NN            DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_FREE_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC            DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN         DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ         DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ        DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC             DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ          DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_CALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC      DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ   DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_MALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC     DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN  DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NN
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ  DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_REALLOC_NNZ
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC      DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ   DEBUG_NEW_DOC_API_CUSTOM_ACCESS_ALIGNED_OFFSET_CALLOC_NZ

DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)        void *DEBUG_NEW_DOC_API_ALIGNED_MALLOC(size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_MALLOC_NZ(size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p)            void DEBUG_NEW_DOC_API_ALIGNED_FREE(void *p);
DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p)         void DEBUG_NEW_DOC_API_ALIGNED_FREE_NN(void *p);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_REALLOC(void *p, size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a)  void *DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NN(void *p, size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a)  void *DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NZ(void *p, size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NNZ(void *p, size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)        void *DEBUG_NEW_DOC_API_ALIGNED_CALLOC(size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_CALLOC_NZ(size_t s, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC(size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_NZ(size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC(void *p, size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)  void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NN(void *p, size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)  void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NZ(void *p, size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NNZ(void *p, size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOC(size_t s, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOC_NZ(size_t s, size_t a, size_t o);
#define DEBUG_NEW_API_ALIGNED_MALLOC      DEBUG_NEW_DOC_API_ALIGNED_MALLOC
#define DEBUG_NEW_API_ALIGNED_MALLOC_NZ   DEBUG_NEW_DOC_API_ALIGNED_MALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_FREE        DEBUG_NEW_DOC_API_ALIGNED_FREE
#define DEBUG_NEW_API_ALIGNED_FREE_NN     DEBUG_NEW_DOC_API_ALIGNED_FREE_NN
#define DEBUG_NEW_API_ALIGNED_REALLOC     DEBUG_NEW_DOC_API_ALIGNED_REALLOC
#define DEBUG_NEW_API_ALIGNED_REALLOC_NN  DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NN
#define DEBUG_NEW_API_ALIGNED_REALLOC_NZ  DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_REALLOC_NNZ DEBUG_NEW_DOC_API_ALIGNED_REALLOC_NNZ
#define DEBUG_NEW_API_ALIGNED_CALLOC      DEBUG_NEW_DOC_API_ALIGNED_CALLOC
#define DEBUG_NEW_API_ALIGNED_CALLOC_NZ   DEBUG_NEW_DOC_API_ALIGNED_CALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC      DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ   DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC     DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN  DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ  DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOC_NNZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC      DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOC
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ   DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOC_NZ

DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a)    size_t DEBUG_NEW_DOC_API_ALIGNED_MALLOC_MSIZE(void const *p, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) size_t DEBUG_NEW_DOC_API_ALIGNED_MALLOC_MSIZE_NN(void const *p, size_t a);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o)    size_t DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_MSIZE(void const *p, size_t a, size_t o);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) size_t DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN(void const *p, size_t a, size_t o);
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE           DEBUG_NEW_DOC_API_ALIGNED_MALLOC_MSIZE
#define DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN        DEBUG_NEW_DOC_API_ALIGNED_MALLOC_MSIZE_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE    DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_MSIZE
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN

DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)        void *DEBUG_NEW_DOC_API_ALIGNED_MALLOCF(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_MALLOCF_NZ(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_REALLOCF(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a)  void *DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NN(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a)  void *DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NZ(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NNZ(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)        void *DEBUG_NEW_DOC_API_ALIGNED_CALLOCF(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_ALIGNED_CALLOCF_NZ(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a)        void *DEBUG_NEW_DOC_API_VALIGNED_MALLOCF(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_VALIGNED_MALLOCF_NZ(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a)     void *DEBUG_NEW_DOC_API_VALIGNED_REALLOCF(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a)  void *DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NN(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a)  void *DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NZ(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NNZ(void *p, size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a)        void *DEBUG_NEW_DOC_API_VALIGNED_CALLOCF(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a)     void *DEBUG_NEW_DOC_API_VALIGNED_CALLOCF_NZ(size_t s, size_t a, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#define DEBUG_NEW_API_ALIGNED_MALLOCF       DEBUG_NEW_DOC_API_ALIGNED_MALLOCF
#define DEBUG_NEW_API_ALIGNED_MALLOCF_NZ    DEBUG_NEW_DOC_API_ALIGNED_MALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_REALLOCF      DEBUG_NEW_DOC_API_ALIGNED_REALLOCF
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NZ   DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NN   DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NN
#define DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ  DEBUG_NEW_DOC_API_ALIGNED_REALLOCF_NNZ
#define DEBUG_NEW_API_ALIGNED_CALLOCF       DEBUG_NEW_DOC_API_ALIGNED_CALLOCF
#define DEBUG_NEW_API_ALIGNED_CALLOCF_NZ    DEBUG_NEW_DOC_API_ALIGNED_CALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_MALLOCF      DEBUG_NEW_DOC_API_VALIGNED_MALLOCF
#define DEBUG_NEW_API_VALIGNED_MALLOCF_NZ   DEBUG_NEW_DOC_API_VALIGNED_MALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_REALLOCF     DEBUG_NEW_DOC_API_VALIGNED_REALLOCF
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NZ  DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NN  DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NN
#define DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ DEBUG_NEW_DOC_API_VALIGNED_REALLOCF_NNZ
#define DEBUG_NEW_API_VALIGNED_CALLOCF      DEBUG_NEW_DOC_API_VALIGNED_CALLOCF
#define DEBUG_NEW_API_VALIGNED_CALLOCF_NZ   DEBUG_NEW_DOC_API_VALIGNED_CALLOCF_NZ

DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOCF(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOCF_NZ(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)  void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NN(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)  void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NZ(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NNZ(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOCF(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOCF_NZ(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_MALLOCF(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_MALLOCF_NZ(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o)     void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)  void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NN(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)  void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NZ(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NNZ(void *p, size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o)        void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_CALLOCF(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)     void *DEBUG_NEW_DOC_API_VALIGNED_OFFSET_CALLOCF_NZ(size_t s, size_t a, size_t o, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args);
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF       DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ    DEBUG_NEW_DOC_API_ALIGNED_OFFSET_MALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF      DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ   DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN   DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NN
#define DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ  DEBUG_NEW_DOC_API_ALIGNED_OFFSET_REALLOCF_NNZ
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF       DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOCF
#define DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ    DEBUG_NEW_DOC_API_ALIGNED_OFFSET_CALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF      DEBUG_NEW_DOC_API_VALIGNED_OFFSET_MALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ   DEBUG_NEW_DOC_API_VALIGNED_OFFSET_MALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF     DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ  DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN  DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NN
#define DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ DEBUG_NEW_DOC_API_VALIGNED_OFFSET_REALLOCF_NNZ
#define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF      DEBUG_NEW_DOC_API_VALIGNED_OFFSET_CALLOCF
#define DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ   DEBUG_NEW_DOC_API_VALIGNED_OFFSET_CALLOCF_NZ

#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_MSTATE_PUSH(void);
/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_MSTATE_POP(void);
/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_MSTATE_GLOBAL_PUSH(void);
/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_MSTATE_GLOBAL_POP(void);
#define DEBUG_NEW_API_MSTATE_PUSH        DEBUG_NEW_DOC_API_MSTATE_PUSH
#define DEBUG_NEW_API_MSTATE_POP         DEBUG_NEW_DOC_API_MSTATE_POP
#define DEBUG_NEW_API_MSTATE_GLOBAL_PUSH DEBUG_NEW_DOC_API_MSTATE_GLOBAL_PUSH
#define DEBUG_NEW_API_MSTATE_GLOBAL_POP  DEBUG_NEW_DOC_API_MSTATE_GLOBAL_POP

DEBUG_NEW_ATTRIBUTE_USE_RESULT char const *DEBUG_NEW_DOC_API_POINTER_GET_FILE(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT int DEBUG_NEW_DOC_API_POINTER_GET_LINE(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT char const *DEBUG_NEW_DOC_API_POINTER_GET_LINE_STR(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT char const *DEBUG_NEW_DOC_API_POINTER_GET_FUNC(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT char const *DEBUG_NEW_DOC_API_POINTER_GET_EXPR(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT char const *DEBUG_NEW_DOC_API_POINTER_GET_MSG(void const *p);
DEBUG_NEW_ATTRIBUTE_USE_RESULT size_t DEBUG_NEW_DOC_API_POINTER_GET_ID(void const *p);
#define DEBUG_NEW_API_POINTER_GET_FILE DEBUG_NEW_DOC_API_POINTER_GET_FILE
#define DEBUG_NEW_API_POINTER_GET_LINE DEBUG_NEW_DOC_API_POINTER_GET_LINE
#define DEBUG_NEW_API_POINTER_GET_LINE_STR DEBUG_NEW_DOC_API_POINTER_GET_LINE_STR
#define DEBUG_NEW_API_POINTER_GET_FUNC DEBUG_NEW_DOC_API_POINTER_GET_FUNC
#define DEBUG_NEW_API_POINTER_GET_EXPR DEBUG_NEW_DOC_API_POINTER_GET_EXPR
#define DEBUG_NEW_API_POINTER_GET_MSG DEBUG_NEW_DOC_API_POINTER_GET_MSG
#define DEBUG_NEW_API_POINTER_GET_ID DEBUG_NEW_DOC_API_POINTER_GET_ID

DEBUG_NEW_ATTRIBUTE_USE_RESULT int DEBUG_NEW_DOC_API_CHECK_PTR(void const *p);
#define DEBUG_NEW_API_CHECK_PTR DEBUG_NEW_DOC_API_CHECK_PTR

/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_PRINTFUNC_PUSH(void (*func)(char const *msg));
/*DEBUG_NEW_ATTRIBUTE_USE_RESULT*/ int DEBUG_NEW_DOC_API_PRINTFUNC_POP(void);
  DEBUG_NEW_ATTRIBUTE_USE_RESULT   unsigned int DEBUG_NEW_DOC_API_PRINTFUNC_STACK_SIZE(void);
#define DEBUG_NEW_API_PRINTFUNC_PUSH          DEBUG_NEW_DOC_API_PRINTFUNC_PUSH
#define DEBUG_NEW_API_PRINTFUNC_POP           DEBUG_NEW_DOC_API_PRINTFUNC_POP
#define DEBUG_NEW_API_PRINTFUNC_STACK_SIZE    DEBUG_NEW_DOC_API_PRINTFUNC_STACK_SIZE

void DEBUG_NEW_DOC_API_RAW_PRINT(char const *msg);
void DEBUG_NEW_DOC_API_RAW_PRINTF(DEBUG_NEW_ANNOTATION_PRINTF_STR char const *msg, ...);
void DEBUG_NEW_DOC_API_RAW_VPRINTF(DEBUG_NEW_ANNOTATION_PRINTF_STR char const *msg, va_list args);
#define DEBUG_NEW_API_RAW_PRINT   DEBUG_NEW_DOC_API_RAW_PRINT
#define DEBUG_NEW_API_RAW_PRINTF  DEBUG_NEW_DOC_API_RAW_PRINTF
#define DEBUG_NEW_API_RAW_VPRINTF DEBUG_NEW_DOC_API_RAW_VPRINTF

void DEBUG_NEW_DOC_API_RAW_PRINT_LOCATION(void);
#define DEBUG_NEW_API_RAW_PRINT_LOCATION   DEBUG_NEW_DOC_API_RAW_PRINT_LOCATION

void DEBUG_NEW_DOC_API_PRINT(char const *msg);
void DEBUG_NEW_DOC_API_PRINTF(DEBUG_NEW_ANNOTATION_PRINTF_STR char const *msg, ...);
void DEBUG_NEW_DOC_API_VPRINTF(DEBUG_NEW_ANNOTATION_PRINTF_STR char const *msg, va_list args);
#define DEBUG_NEW_API_PRINT   DEBUG_NEW_DOC_API_PRINT
#define DEBUG_NEW_API_PRINTF  DEBUG_NEW_DOC_API_PRINTF
#define DEBUG_NEW_API_VPRINTF DEBUG_NEW_DOC_API_VPRINTF

void DEBUG_NEW_DOC_API_ALLOC_BREAKPOINT(size_t id);
void DEBUG_NEW_DOC_API_ALLOC_BREAKPOINT_CLEAR(void);
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t DEBUG_NEW_DOC_API_ALLOC_ID(void);
#define DEBUG_NEW_API_ALLOC_BREAKPOINT       DEBUG_NEW_DOC_API_ALLOC_BREAKPOINT
#define DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR DEBUG_NEW_DOC_API_ALLOC_BREAKPOINT_CLEAR
#define DEBUG_NEW_API_ALLOC_ID               DEBUG_NEW_DOC_API_ALLOC_ID
#endif /* DEBUG_NEW_CONFIG_DOC_ONLY */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW>>
//  - "DEBUG_NEW" was the original debug allocation macro.
//    A lot of source files are implemented to "#define new DEBUG_NEW"
//    at the beginning, to prevent name collisions
//    with header files that may overwrite "operator new".
//    And while debug_new implements <debug_new_disable.inl> / <debug_new_enable.inl>
//    for that, we still want to provide that compatibility.
//////////////////////////////////////////////////////////////////////////
#if !defined(DEBUG_NEW) && defined(DEBUG_NEW_CXX_NEW)
#define DEBUG_NEW   DEBUG_NEW_CXX_NEW
#endif /* !DEBUG_NEW && DEBUG_NEW_CXX_NEW */


//////////////////////////////////////////////////////////////////////////
// <<DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS>>
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Allocation helper macros
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 1

// <<"T *DEBUG_NEW_API_ALLOC(typename T)">>
//   - Allocate memory, to fit <1> instance of "T"
//     and return it casted to "T *"
#define DEBUG_NEW_API_ALLOC(T)  DEBUG_NEW_REINTERPRET_CAST(T*)(DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ(sizeof(T),DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_ALLOC(" #T ")")))

// <<"T *DEBUG_NEW_API_ALLOCN(size_t n, typename T)">>
//   - Allocate memory, to fit "n" instance of "T"
//     and return it casted to "T *"
#define DEBUG_NEW_API_ALLOCN(n,T)  DEBUG_NEW_REINTERPRET_CAST(T*)(DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC_NZ((n)*sizeof(T),DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_ALLOCN(" #n "," #T ")")))

// <<"T *DEBUG_NEW_API_CALLOCN(size_t n, typename T)">>
//   - same as "DEBUG_NEW_API_ALLOCN(n,T)", but uses calloc instead
#define DEBUG_NEW_API_CALLOCN(n,T)  DEBUG_NEW_REINTERPRET_CAST(T*)(DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC_NZ(n,sizeof(T),DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_CALLOCN(" #n "," #T ")")))

#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
// <<"void DEBUG_NEW_API_GETMEM(T *&p, size_t s)">>
//   - Pascal-style memory allocation
//   - Allocate "s" bytes memory and store
//     the pointer in "p".
//   - If the allocation failed, "_OUT_OF_MEMORY()" is called
#define DEBUG_NEW_API_GETMEM(p,s) do{if((DEBUG_NEW_LVALUE_CAST(void*)(p)=DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC(s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_GETMEM(" #p "," #s ")")))==NULL){_OUT_OF_MEMORY();}}while(0)
#endif

#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
// <<"void DEBUG_NEW_API_REALLOCMEM(T *&p, size_t s)">>
//   - Pascal-style memory allocation
//   - Reallocate "p", to "s" bytes and update the pointer "p"
//   - If the allocation failed, "_OUT_OF_MEMORY()" is called
//     NOTE: "p" is not modified, if the allocation fails
#define DEBUG_NEW_API_REALLOCMEM(p,s) do{void *dn_realloc_new_block;if((dn_realloc_new_block=DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC(p,s,DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_GETMEM(" #p "," #s ")")))==NULL){_OUT_OF_MEMORY();}else DEBUG_NEW_LVALUE_CAST(void*)(p)=dn_realloc_new_block;}while(0)
#endif

// <<"void DEBUG_NEW_API_FREEMEM(void *p)">>
//   - Pascal-style memory deallocation
//   - overload to free(p)
//   - NOTE: p may be any pointer!
#define DEBUG_NEW_API_FREEMEM(p) DEBUG_NEW_API_CUSTOM_ACCESS_FREE((void *)(p),DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_FREEMEM(" #p ")"))

// <<"template<typename T> T *&DEBUG_NEW_API_FREE_AND_NULL(T *&p)">>
//   - free any pointer "p" and set it to 0
//   - equivalent to "free((void *)p); p = 0;"
//   - returns the lvalue for "p", now pointing to "0"
#define DEBUG_NEW_API_FREE_AND_NULL(p) (DEBUG_NEW_API_CUSTOM_ACCESS_FREE((void *)(p),DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_FREE_AND_NULL(" #p ")")),DEBUG_NEW_LVALUE_CAST(void*)(p)=NULL)

#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 1 */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */



//////////////////////////////////////////////////////////////////////////
// deprecated methods / macros
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Deprecation APIs
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
#if DEBUG_NEW_CONFIG_ENABLED
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
DEBUG_NEW_FUNC_DECL(void) Dn_Print(char const *msg); // deprecated; use "DnPrint"
DEBUG_NEW_FUNC_DECL(void) Dn_Printf(char const *msg, ...); // deprecated; use "DnPrintf"
DEBUG_NEW_FUNC_DECL(void) Dn_VPrintf(char const *msg, va_list args); // deprecated; use "DnVPrintf"
#ifdef __cplusplus
}
#endif /* __cplusplus */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#endif /* DEBUG_NEW_CONFIG_ENABLED */

#if DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE && defined(__cplusplus)
// Deprecated, old c++ namespace structure (based on default config)
namespace __debug {
 using namespace ::debug_new;
 namespace __hidden__ {using namespace ::debug_new::detail;}
}
#endif /* DEBUG_NEW_CONFIG_HAVE_CXX_NAMESPACE && __cplusplus */

#define malloc_ex  mallocf
#define realloc_ex reallocf
#define calloc_ex  callocf

#define __alloc            DEBUG_NEW_API_ALLOC
#define __allocn           DEBUG_NEW_API_ALLOCN
#define __callocn          DEBUG_NEW_API_CALLOCN
#define __GetMem           DEBUG_NEW_API_GETMEM
#define __ReallocMem       DEBUG_NEW_API_REALLOCMEM
#define __FreeMem          DEBUG_NEW_API_FREEMEM
// v removed, as it has no way of handling out-of-memory
#define __realloc_new(p,s) (DEBUG_NEW_LVALUE_CAST(void*)(p)=realloc((void*)(p),s))
#define __free_and_null    DEBUG_NEW_API_FREE_AND_NULL
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 2
#define _alloc         __alloc
#define _allocn        __allocn
#define _callocn       __callocn
#define _GetMem        __GetMem
#define _ReallocMem    __ReallocMem
#define _FreeMem       __FreeMem
#define _realloc_new   __realloc_new
#define _free_and_null __free_and_null
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 3
#define alloc          __alloc
#define allocn         __allocn
#define callocn        __callocn
#define GetMem         __GetMem
#define ReallocMem     __ReallocMem
#define FreeMem        __FreeMem
#define realloc_new    __realloc_new
#define free_and_null  __free_and_null
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 3 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 2 */

#define _DEBUG_NEW_CALL_NONE             DEBUG_NEW_ALLOC_CALL_TYPE_NONE
#define _DEBUG_NEW_CALL_MALLOC           DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC
#define _DEBUG_NEW_CALL_REALLOC          DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC
#define _DEBUG_NEW_CALL_CALLOC           DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC
#define _DEBUG_NEW_CALL_FREE             DEBUG_NEW_ALLOC_CALL_TYPE_FREE
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
# define _DEBUG_NEW_CALL_OP_NEW          DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
# define _DEBUG_NEW_CALL_OP_ARRAY_NEW    DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
# define _DEBUG_NEW_CALL_OP_DELETE       DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
# define _DEBUG_NEW_CALL_OP_ARRAY_DELETE DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */

#define __noval                              DEBUG_NEW_PRIVATE_EMPTY_EXPR
#define _DEBUG_NEW_BUILD_ACCESS              DEBUG_NEW_PRIVATE_MAKE_ACCESS
#define _DEBUG_NEW_BUILD_ACCESS2             DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS

#define _DEBUG_NEW_ACCESS_MALLOC(s)                    DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc(" #s ")")
#define _DEBUG_NEW_ACCESS_REALLOC(p,s)                 DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc(" #p "," #s ")")
#define _DEBUG_NEW_ACCESS_CALLOC(n,s)                  DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("calloc(" #n "," #s ")")
#define _DEBUG_NEW_ACCESS_FREE(p)                      DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("free(" #p ")")
#define _DEBUG_NEW_ACCESS_STRDUP(s)                    DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strdup(" #s ")")
#define _DEBUG_NEW_ACCESS__STRDUP(s)                   DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("_strdup(" #s ")")
#define _DEBUG_NEW_ACCESS_STRNDUP(s,n)                 DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("strndup(" #s "," #n ")")
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
# define _DEBUG_NEW_ACCESS___MALLOC_SIZE(p)            DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__malloc_msize(" #p ")")
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */
#if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
# define _DEBUG_NEW_ACCESS_MALLOC_USABLE_SIZE(p)       DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_usable_size(" #p ")")
#endif /* DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE */
#if DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
# define _DEBUG_NEW_ACCESS__MSIZE(p)                   DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("_msize(" #p ")")
#endif /* DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE */
#if DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
# define _DEBUG_NEW_ACCESS_MALLOC_SIZE(p)              DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_size(" #p ")")
#endif /* DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE */
#define _DEBUG_NEW_ACCESS_ANY                          DEBUG_NEW_PRIVATE_EMPTY_ACCESS
#define _DEBUG_NEW_ACCESS_THREAD_STATE_PUSH            DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_push()")
#define _DEBUG_NEW_ACCESS_THREAD_STATE_POP             DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("mstate_pop()")
#define _DEBUG_NEW_ACCESS_INIT                         DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("Dn_Init()")
#define _DEBUG_NEW_ACCESS_QUIT                         DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("Dn_Quit()")
#define _DEBUG_NEW_ACCESS_CHECK_POINTER(p)             DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_CHECK_PTR(" #p ")")
#define _DEBUG_NEW_ACCESS_THREAD_PUSH_PRINT_FUNC(func) DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_PRINTFUNC_PUSH(" #func ")")
#define _DEBUG_NEW_ACCESS_THREAD_POP_PRINT_FUNC        DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("DEBUG_NEW_API_PRINTFUNC_POP()")
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC
#if DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS
# define _DEBUG_NEW_ACCESS_MALLOC_EX(s,...)           DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("malloc_ex(" #s "," #__VA_ARGS__ ")")
# define _DEBUG_NEW_ACCESS_REALLOC_EX(p,s,...)        DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("realloc_ex(" #p "," #s "," #__VA_ARGS__ ")")
# define _DEBUG_NEW_ACCESS_CALLOC_EX(n,s,...)         DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("calloc_ex(" #n "," #s "," #__VA_ARGS__ ")")
#else /* DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
# define _DEBUG_NEW_ACCESS_MALLOC_EX(s,...)           _DEBUG_NEW_ACCESS_MALLOC(s)
# define _DEBUG_NEW_ACCESS_REALLOC_EX(p,s,...)        _DEBUG_NEW_ACCESS_REALLOC(p,s)
# define _DEBUG_NEW_ACCESS_CALLOC_EX(n,s,...)         _DEBUG_NEW_ACCESS_CALLOC(n,s)
#endif /* !DEBUG_NEW_CONFIG_HAVE_ADVANCED_ALLOC_OWN_ACCESS */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS >= 1
# define _DEBUG_NEW_ACCESS___ALLOC(T)                  DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__alloc(" #T ")")
# define _DEBUG_NEW_ACCESS___ALLOCN(n,T)               DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__allocn(" #n "," #T ")")
# define _DEBUG_NEW_ACCESS___CALLOCN(n,T)              DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__callocn(" #n "," #T ")")
# define _DEBUG_NEW_ACCESS___GETMEM(p,s)               DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__GetMem(" #p "," #s ")")
# define _DEBUG_NEW_ACCESS___REALLOCMEM(p,s)           DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__ReallocMem(" #p "," #s ")")
# define _DEBUG_NEW_ACCESS___FREEMEM(p)                DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__FreeMem(" #p ")")
# define _DEBUG_NEW_ACCESS___FREE_AND_NULL(p)          DEBUG_NEW_PRIVATE_FAST_MAKE_ACCESS("__free_and_null(" #p ")")
#endif // DEBUG_NEW_CONFIG_HAVE_ALLOC_HELPERS
#if DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#define _DEBUG_NEW_ACCESS_INIT_INTERNAL DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL
#define _DEBUG_NEW_ACCESS_QUIT_INTERNAL DEBUG_NEW_PRIVATE_ACCESS_QUIT_INTERNAL
#endif // DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE

#ifdef DEBUG_NEW_CXX_PLACEMENT
#define __debug_placement           DEBUG_NEW_CXX_PLACEMENT
#endif
#ifdef DEBUG_NEW_CXX_NEW
#define __debug_new                 DEBUG_NEW_CXX_NEW
#endif
#define __debug_ca_malloc           DEBUG_NEW_API_CUSTOM_ACCESS_MALLOC
#define __debug_ca_free             DEBUG_NEW_API_CUSTOM_ACCESS_FREE
#define __debug_ca_realloc          DEBUG_NEW_API_CUSTOM_ACCESS_REALLOC
#define __debug_ca_calloc           DEBUG_NEW_API_CUSTOM_ACCESS_CALLOC
#define __debug_malloc              DEBUG_NEW_API_MALLOC
#define __debug_free                DEBUG_NEW_API_FREE
#define __debug_realloc             DEBUG_NEW_API_REALLOC
#define __debug_calloc              DEBUG_NEW_API_CALLOC
#define __debug_strdup              DEBUG_NEW_API_STRDUP
#define __debug_strndup             DEBUG_NEW_API_STRNDUP
#define __debug__strdup             DEBUG_NEW_API_STRDUP
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
# define __debug___malloc_size      DEBUG_NEW_API_MALLOC_MSIZE_C
#endif // DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
# define __debug_malloc_usable_size DEBUG_NEW_API_MALLOC_MSIZE
#endif // DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#if DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
# define __debug__msize             DEBUG_NEW_API_MALLOC_MSIZE
#endif // DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#if DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
# define __debug_malloc_size        DEBUG_NEW_API_MALLOC_MSIZE_C
#endif // DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#ifdef _MSC_VER
# define __debug_wcsdup             DEBUG_NEW_API_WCSDUP
# define __debug__wcsdup            DEBUG_NEW_API_WCSDUP
# define __debug__mbsdup            DEBUG_NEW_API_MBSDUP
#endif
#define __debug_malloc_ex           DEBUG_NEW_API_MALLOCF
#define __debug_realloc_ex          DEBUG_NEW_API_REALLOCF
#define __debug_calloc_ex           DEBUG_NEW_API_CALLOCF
#define __debug_mstate_push         DEBUG_NEW_API_MSTATE_PUSH
#define __debug_mstate_pop          DEBUG_NEW_API_MSTATE_POP
#define __debug_mstate_global_push  DEBUG_NEW_API_MSTATE_GLOBAL_PUSH
#define __debug_mstate_global_pop   DEBUG_NEW_API_MSTATE_GLOBAL_POP
#define __debug_check_ptr           DEBUG_NEW_API_CHECK_PTR
#define __debug_printfunc_push      DEBUG_NEW_API_PRINTFUNC_PUSH
#define __debug_printfunc_pop       DEBUG_NEW_API_PRINTFUNC_POP
#define __debug_printfunc_size      DEBUG_NEW_API_PRINTFUNC_STACK_SIZE
#define __debug_print_raw           DEBUG_NEW_API_RAW_PRINT
#define __debug_printf_raw          DEBUG_NEW_API_RAW_PRINTF
#define __debug_vprintf_raw         DEBUG_NEW_API_RAW_VPRINTF
#define __debug_print_location      DEBUG_NEW_API_RAW_PRINT_LOCATION
#define __debug_print               DEBUG_NEW_API_PRINT
#define __debug_printf              DEBUG_NEW_API_PRINTF
#define __debug_vprintf             DEBUG_NEW_API_VPRINTF

#define __debug_alloc_breakpoint       DEBUG_NEW_API_ALLOC_BREAKPOINT
#define __debug_alloc_breakpoint_clear DEBUG_NEW_API_ALLOC_BREAKPOINT_CLEAR
#define __debug_alloc_id               DEBUG_NEW_API_ALLOC_ID

#ifdef DEBUG_NEW_OS_THREAD_ID_T
#define _OS_THREAD_ID   DEBUG_NEW_OS_THREAD_ID_T
#endif /* DEBUG_NEW_OS_THREAD_ID_T */
#ifdef DEBUG_NEW_OS_THREAD_SELF
#define _OS_THREAD_SELF DEBUG_NEW_OS_THREAD_SELF
#endif /* DEBUG_NEW_OS_THREAD_SELF */

#ifdef DEBUG_NEW_OS_MUTEX_T
#define _OS_MUTEX_T         DEBUG_NEW_OS_MUTEX_T
#define _OS_MUTEX_INIT      DEBUG_NEW_OS_MUTEX_INIT
#define _OS_MUTEX_QUIT      DEBUG_NEW_OS_MUTEX_QUIT
#define _OS_MUTEX_ENTER     DEBUG_NEW_OS_MUTEX_ENTER
#define _OS_MUTEX_LEAVE     DEBUG_NEW_OS_MUTEX_LEAVE
#define _OS_MUTEX_TRY_ENTER DEBUG_NEW_OS_MUTEX_TRY_ENTER
#endif /* DEBUG_NEW_OS_MUTEX_T */

#ifdef DEBUG_NEW_OS_TLS_T
#define _OS_TLS_T        DEBUG_NEW_OS_TLS_T
#define _OS_TLS_INIT     DEBUG_NEW_OS_TLS_INIT
#define _OS_TLS_QUIT     DEBUG_NEW_OS_TLS_QUIT
#define _OS_TLS_GET_FAST DEBUG_NEW_OS_TLS_GET_FAST
#define _OS_TLS_GET      DEBUG_NEW_OS_TLS_GET
#define _OS_TLS_SET      DEBUG_NEW_OS_TLS_SET
#endif /* DEBUG_NEW_OS_TLS_T */

#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// Link debug_new against the requested state
#ifdef DEBUG_NEW_PRIVATE_DISABLED
// ** small hack, to allow <debug_new.h> being included between
//    <debug_new_disable.inl> and <debug_new_enable.inl>
#define DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED 1
#include "debug_new_disable.inl"
#else
#define DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED 1
#include "debug_new_enable.inl"
#if defined(DEBUG_NEW_PRIVATE_DISABLED)
#error "Failed, to enable debug_new"
#endif
#endif

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region C++ Operators
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if !defined(DEBUG_NEW_CONFIG_BUILDING_SOURCE)

#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
//////////////////////////////////////////////////////////////////////////
// _OUT_OF_MEMORY
//  - Deprecated; don't use this
//  - This is a remainder of the ancient past, where debug_new was
//    written in that weird c/c++ hybrid and actually threw exceptions
//    it its internal functions failed to allocate memory.
//    This is what is left from those times and no longer has any
//    real meaning.
//  - Now-a-days you are free to implement you own out-of-memory
//    handling that can do whatever you what it to do!
#ifndef _OUT_OF_MEMORY
#if defined(DEBUG_NEW_EXTERNAL_THROW_BAD_ALLOC)
extern DEBUG_NEW_ATTRIBUTE_NORETURN void throw_bad_alloc(void);
#define _OUT_OF_MEMORY throw_bad_alloc
#elif defined(__cplusplus)
#if defined(__debug_throw)
# define _OUT_OF_MEMORY() __debug_throw(::std::bad_alloc())
#else
# define _OUT_OF_MEMORY() throw(::std::bad_alloc())
#endif
#ifndef DEBUG_NEW_WITHOUT_HEADERS
// NOTE: This right here is a perfect example, of how to use 
//       the enable/disable #includes of debug_new!
#include "debug_new_disable.inl"
#include <exception>
#include "debug_new_enable.inl"
#endif /* !DEBUG_NEW_WITHOUT_HEADERS */
#else
# define _OUT_OF_MEMORY() DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif
#endif /* !_OUT_OF_MEMORY */
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED */

#if defined(__cplusplus)
#if DEBUG_NEW_CONFIG_ENABLED
#ifndef __INTELLISENSE__
#include "debug_new_disable.inl"
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4986) /* 'X': exception specification does not match previous declaration */
# pragma warning(disable:4559) /* 'operator new' : redefinition; the function gains __declspec(restrict) */
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28183)
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28196)
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28252)
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28253)
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28301)
#endif /* _MSC_VER */
//////////////////////////////////////////////////////////////////////////
// Operator integration
//////////////////////////////////////////////////////////////////////////
#ifdef _OUT_OF_MEMORY
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new(/*non-zero*/size_t size, char const *,int) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new[](/*non-zero*/size_t size, char const *,int) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new(/*non-zero*/size_t size) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new[](/*non-zero*/size_t size) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
#else /* _OUT_OF_MEMORY */
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new(/*non-zero*/size_t size, char const *,int) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) throw(::std::bad_alloc()); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new[](/*non-zero*/size_t size, char const *,int) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) throw(::std::bad_alloc()); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new(/*non-zero*/size_t size) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) throw(::std::bad_alloc()); return ptr; }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new[](/*non-zero*/size_t size) { void *ptr = DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); if (!ptr) throw(::std::bad_alloc()); return ptr; }
#endif /* !_OUT_OF_MEMORY */
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new(/*non-zero*/size_t size, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { return DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_MALLOC_NZ(size) void *operator new[](/*non-zero*/size_t size, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { return DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_MallocNZ(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW,size,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete(void *ptr) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete[](void *ptr) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete(void *ptr, char const *,int) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete[](void *ptr, char const *,int) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete(void *ptr, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
inline DEBUG_NEW_ATTRIBUTE_NOINLINE DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete[](void *ptr, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE,ptr,NULL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
#ifdef _MSC_VER
# pragma warning(pop)
#endif /* _MSC_VER */
#include "debug_new_enable.inl"
#endif /* !__INTELLISENSE__ */
#elif DEBUG_NEW_CONFIG_HAVE_OVERWRITE_NO_DEBUG_CXX_OPERATORS
//#include "debug_new_disable.inl" // No need; debug_new isn't enabled!
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4559) /* Redefinition adds __declspec(restrict) */
# pragma warning(disable:4986) /* 'X': exception specification does not match previous declaration */
#endif /* _MSC_VER */
//////////////////////////////////////////////////////////////////////////
// non-debug new / delete operators
//////////////////////////////////////////////////////////////////////////
#ifdef _OUT_OF_MEMORY
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new[] (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) _OUT_OF_MEMORY(); return ptr; }
#elif defined(__debug_throw)
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) __debug_throw(std::bad_alloc()); return ptr; }
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new[] (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) __debug_throw(std::bad_alloc()); return ptr; }
#else
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) throw std::bad_alloc(); return ptr; }
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new[] (size_t size) { void *ptr = DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); if (!ptr) throw std::bad_alloc(); return ptr; }
#endif
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new (size_t size, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { return DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); }
inline DEBUG_NEW_ANNOTATION_MALLOC_BASE(size) void *operator new[](size_t size, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { return DEBUG_NEW_API_NATIVE_MALLOC_NZ(size); }
inline DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete (void *ptr) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_FREE(ptr); }
inline DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete[] (void *ptr) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_FREE(ptr); }
inline DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete (void *ptr, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_FREE(ptr); }
inline DEBUG_NEW_ANNOTATION_FREE_BASE(ptr) void operator delete[] (void *ptr, const std::nothrow_t&) DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_FREE(ptr); }
#ifdef _MSC_VER
# pragma warning(pop)
#endif /* _MSC_VER */
//#include "debug_new_enable.inl" // No need; debug_new isn't enabled!
#endif
#endif /* __cplusplus */

#if DEBUG_NEW_CONFIG_LINK_STATICALLY
#include "debug_new.c"
#endif /* DEBUG_NEW_CONFIG_LINK_STATICALLY */
#endif /* !DEBUG_NEW_CONFIG_BUILDING_SOURCE */
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


//////////////////////////////////////////////////////////////////////////
// Native API Initialization / Finalization, when debug_new is disabled.
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma region Native API Initialization / Finalization
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */
#if !DEBUG_NEW_CONFIG_ENABLED && \
   (defined(DEBUG_NEW_API_NATIVE_INITIALIZE) ||\
    defined(DEBUG_NEW_API_NATIVE_FINALIZE))
// Hook-up the automatic initialization for the non-debug native API
// >> The Win32 HeapAlloc API needs to be initialization manually,
//    to store the handle returned by GetProcessHeap().
// >> This has to be done before anything else, to get allocations to work properly
#if DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#if DEBUG_NEW_MAIN_HOOK
// We can use the main-hook, to link into!
int _after_debug_new_main(int argc, char* argv[]);
int main(int argc, char* argv[]) {
#ifdef DEBUG_NEW_API_NATIVE_FINALIZE
 int app_exit_code;
#ifdef DEBUG_NEW_API_NATIVE_INITIALIZE
 DEBUG_NEW_API_NATIVE_INITIALIZE();
#endif /* DEBUG_NEW_API_NATIVE_INITIALIZE */
#ifdef __cplusplus
 try {
  app_exit_code = _after_debug_new_main(argc,argv);
 } catch (...) {
  DEBUG_NEW_API_NATIVE_FINALIZE();
  throw;
 }
#else /* !__cplusplus */
 app_exit_code = _after_debug_new_main(argc,argv);
#endif /* !__cplusplus */
 DEBUG_NEW_API_NATIVE_FINALIZE();
 return app_exit_code;
#else /* DEBUG_NEW_API_NATIVE_FINALIZE */
 DEBUG_NEW_API_NATIVE_INITIALIZE();
 return _after_debug_new_main(argc,argv);
#endif /* !DEBUG_NEW_API_NATIVE_FINALIZE */
}
#undef main
#define main _after_debug_new_main
#elif defined(__cplusplus)
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL
#if DEBUG_NEW_CONFIG_LINK_STATICALLY
static class DnPrivateStaticNativeAPIIncludeCounter {
 public:
#ifdef DEBUG_NEW_API_NATIVE_INITIALIZE
  inline DnPrivateStaticNativeAPIIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_INITIALIZE(); }
#endif /* DEBUG_NEW_API_NATIVE_INITIALIZE */
#ifdef DEBUG_NEW_API_NATIVE_FINALIZE
  inline ~DnPrivateStaticNativeAPIIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT { DEBUG_NEW_API_NATIVE_FINALIZE(); }
#endif /* DEBUG_NEW_API_NATIVE_FINALIZE */
} DnPrivate_StaticNativeAPIIncludeCounter;
#else /* DEBUG_NEW_CONFIG_LINK_STATICALLY */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
DEBUG_NEW_DATA_DECL(int,DnPrivateNativeAPIIncludeCounter_Count);
#ifdef __cplusplus
}
#endif /* __cplusplus */
static class DnPrivateNativeAPIIncludeCounter {
 public:
#ifdef DEBUG_NEW_API_NATIVE_INITIALIZE
  inline DnPrivateNativeAPIIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT {
   if (++DnPrivateNativeAPIIncludeCounter_Count == 1) { DEBUG_NEW_API_NATIVE_INITIALIZE(); }
  }
#endif /* DEBUG_NEW_API_NATIVE_INITIALIZE */
#ifdef DEBUG_NEW_API_NATIVE_FINALIZE
  inline ~DnPrivateNativeAPIIncludeCounter() DEBUG_NEW_CXX11_NOEXCEPT {
   if (--DnPrivateNativeAPIIncludeCounter_Count == 0) { DEBUG_NEW_API_NATIVE_FINALIZE(); }
  }
#endif /* DEBUG_NEW_API_NATIVE_FINALIZE */
} DnPrivate_NativeAPIIncludeCounter;
#ifdef DEBUG_NEW_CONFIG_BUILDING_SOURCE
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int DnPrivateNativeAPIIncludeCounter_Count = 0;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* DEBUG_NEW_CONFIG_BUILDING_SOURCE */
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#else
// The user has to do this!
#ifdef DEBUG_NEW_API_NATIVE_INITIALIZE
#define Dn_Init DEBUG_NEW_API_NATIVE_INITIALIZE
#endif /* DEBUG_NEW_API_NATIVE_INITIALIZE */
#ifdef DEBUG_NEW_API_NATIVE_FINALIZE
#define Dn_Quit DEBUG_NEW_API_NATIVE_FINALIZE
#endif /* DEBUG_NEW_API_NATIVE_FINALIZE */
#endif
#endif /* DEBUG_NEW_CONFIG_HAVE_AUTO_INIT */
#endif
#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION
#pragma endregion 
#endif /* DEBUG_NEW_COMPILER_HAVE_PRAGMA_REGION */


#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !_DEBUG_NEW_H */
#endif /* !GUARD_DEBUG_NEW_H */
