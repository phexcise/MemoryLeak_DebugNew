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
#ifndef GUARD_DEBUG_NEW_C
#define GUARD_DEBUG_NEW_C 1
#undef DEBUG_NEW_CONFIG_BUILDING_SOURCE
#define DEBUG_NEW_CONFIG_BUILDING_SOURCE 1

#include "debug_new_conf.inl"

#if DEBUG_NEW_COMPILER_HAVE_PRAGMA_ONCE && \
   (DEBUG_NEW_CONFIG_LINK_STATICALLY || !(\
    defined(__GNUC__) || defined(__clang__)))
// Some sompilers warn about #pragma once in *main* files
// But <debug_new.c> isn't a main file, if you are linking it statically!
#pragma once
#endif /* ... */

#if !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !defined(_DEBUG_NEW_C)
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !defined(_DEBUG_NEW_C)
#define _DEBUG_NEW_C 1
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !_DEBUG_NEW_C */
#if !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !defined(_DEBUG_NEW_CPP)
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !defined(_DEBUG_NEW_CPP)
#define _DEBUG_NEW_CPP 1
#endif /* DEBUG_NEW_CONFIG_HAVE_DEPRECATED && !_DEBUG_NEW_CPP */

#ifdef DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC
#if !DEBUG_NEW_CONFIG_LINK_STATICALLY
DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT
DEBUG_NEW_DATA_DECL(void *,DnPrivateGlobal_WinProcessHeap);
DEBUG_NEW_DATA_IMPL(void *,DnPrivateGlobal_WinProcessHeap = (void *)0); // Don't write as NULL!
DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */
#endif /* DEBUG_NEW_CONFIG_USE_WIN32_HEAPALLOC */

#if defined(DEBUG_NEW_CONFIG_HAVE_LIMITED_API)
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#define DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API 1
#else 
#define DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API 0
#endif
#undef DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */

/* v we must always work with the off-limit api */
#define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 1
#include "debug_new.h"

#if DEBUG_NEW_CONFIG_ENABLED

//////////////////////////////////////////////////////////////////////////
// v Disable debug_new for the duration of its implementation
//   (We don't want to get runtime-stack_overflows, because debug_new
//    is trying to track its own allocation calls...)
#include "debug_new_disable.inl"

#ifndef DEBUG_NEW_WITHOUT_HEADERS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4820) /* 'xxx' : 'n' bytes padding added after data member 'xxx::yyy' */
#endif /* _MSC_VER */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#if defined(DEBUG_NEW_PLATFORM_WINDOWS) && \
    defined(DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H) && \
    DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
#undef DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4820) /* 'xxx' : 'n' bytes padding added after data member 'xxx::yyy' */
#pragma warning(disable:4668) /* 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */
#pragma warning(disable:4255) /* 'xxx' : no function prototype given: converting '()' to '(void)' */
#endif /* _MSC_VER */
#include <Windows.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* !DEBUG_NEW_PRIVATE_HEADER_NEED_WINDOWS_H */
#endif /* !DEBUG_NEW_WITHOUT_HEADERS */

#ifndef DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING
#ifdef DEBUG_NEW_PLATFORM_WINDOWS
#define DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING(s) (\
 IsDebuggerPresent()?OutputDebugStringA(s):DEBUG_NEW_PRIVATE_EMPTY_EXPR)
#else /* DEBUG_NEW_PLATFORM_WINDOWS */
# define DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING(s) DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_PLATFORM_WINDOWS */
#endif /* !DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING */


#ifndef DEBUG_NEW_PRIVATE_OFFSETOF
#if defined(offsetof)
# define DEBUG_NEW_PRIVATE_OFFSETOF       offsetof
#elif defined(__GNUC__) || __has_builtin(__builtin_offsetof)
# define DEBUG_NEW_PRIVATE_OFFSETOF       __builtin_offsetof
#else
# define DEBUG_NEW_PRIVATE_OFFSETOF(s,m) (size_t)&(((s *)NULL)->m)
#endif
#endif // !DEBUG_NEW_PRIVATE_OFFSETOF

#ifndef DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT
#if defined(_MSC_VER) || defined(__MINGW32__)
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT    "%Iu"
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG    /* nothing */
#elif defined(__GNUC__)
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT    "%zu"
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG    /* nothing */
#else
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT    "%lu"
# define DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(x) DEBUG_NEW_STATIC_CAST(unsigned long)(x)
#endif
#endif /* !DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT */

#ifndef DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT
#if defined(_MSC_VER) || defined(__MINGW32__)
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT    "%Id"
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_ARG    /* nothing */
#elif defined(__GNUC__)
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT    "%zd"
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_ARG    /* nothing */
#else
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT    "%ld"
# define DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_ARG(x) DEBUG_NEW_STATIC_CAST(long)(x)
#endif
#endif /* !DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT */

//////////////////////////////////////////////////////////////////////////
// Begin implementation of <debug_new.c>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) /* conditional expression is constant */
#endif


#ifndef DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC
#ifdef DEBUG_NEW_PLATFORM_64BIT // v logical maximum of a 64-bit pointer
# define DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC(s) ((s) < 0xFFFFFFFFFFFFFFFFull)
#else // DEBUG_NEW_PLATFORM_64BIT
# if defined(DEBUG_NEW_PLATFORM_WINDOWS) && !defined(_WIN64) && !defined(WIN64) // v no process on x86 windows can exceed 2GB
#  define DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC(s) ((s) <= (2ul*1024ul*1024ul*1024ul))
# else // v logical maximum of a 32-bit pointer
#  define DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC(s) ((s) < 0xFFFFFFFFull)
# endif
#endif // !DEBUG_NEW_PLATFORM_64BIT
#endif // !DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC

DEBUG_NEW_CXX_NAMESPACE_ENTER_ROOT

#if defined(__cplusplus) || \
    defined(DEBUG_NEW_CONFIG_LINK_DLLEXPORT)
// NOTE: still define these if we're exporting from a dll/so
#if DEBUG_NEW_CONFIG_HAVE_AUTO_INIT
#if !DEBUG_NEW_CONFIG_LINK_STATICALLY
DEBUG_NEW_CXX_NAMESPACE_ENTER_DETAIL
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
DEBUG_NEW_FUNC_DECL(void) DnPrivate_IncludeCounter_IncCount(void);
DEBUG_NEW_FUNC_DECL(void) DnPrivate_IncludeCounter_DecCount(void);

static int include_counter_total_count = 0;
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_IncludeCounter_IncCount(void) {
 if (++include_counter_total_count == 1)
  DnPrivate_InitInternal(DEBUG_NEW_PRIVATE_ACCESS_INIT_INTERNAL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_IncludeCounter_DecCount(void) {
 if (--include_counter_total_count == 0)
  DnPrivate_QuitInternal(DEBUG_NEW_PRIVATE_ACCESS_QUIT_INTERNAL,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
#ifdef __cplusplus
}
#endif /* __cplusplus */
DEBUG_NEW_CXX_NAMESPACE_LEAVE_DETAIL
#endif /* !DEBUG_NEW_CONFIG_LINK_STATICALLY */
#endif /* DEBUG_NEW_CONFIG_HAVE_AUTO_INIT */
#endif /* __cplusplus */


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//////////////////////////////////////////////////////////////////////////
// debug Debug_new
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_HAVE_ASSERT
DEBUG_NEW_FUNC_IMPL(void) Dn_ReportAssertionFailed(char const *ac) {
 char buffer[2048];
 char const *file = DnAccess_GetFile(ac);
 char const *line = DnAccess_GetLine(ac);
 char const *expr = DnAccess_GetExpr(ac);
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
 char const *func = DnAccess_GetFunc(ac);
 sprintf(buffer,"\n%s(%s) : Assertion failed in %s:\n>> %s\n",file,line,func,expr);
#else /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
 sprintf(buffer,"\n%s(%s) : Assertion failed:\n>> %s\n",file,line,expr);
#endif /* !DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
 DnPrivate_DefaultPrintFunc_Impl(buffer);
}
#endif /* DEBUG_NEW_CONFIG_HAVE_ASSERT */



//////////////////////////////////////////////////////////////////////////
// Internal scope
//////////////////////////////////////////////////////////////////////////
#define DEBUG_NEW_PRIVATE_PLATFORM_MEM_ALIGNMENT (sizeof(size_t)*2)
#define DEBUG_NEW_PRIVATE_ALIGN(s)               (((s)+DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT-1) & ~(DEBUG_NEW_CONFIG_ALLOCATION_ALIGNMENT-1))
#define DEBUG_NEW_PRIVATE_ALIGNED_POINTER_SIZE   DEBUG_NEW_PRIVATE_ALIGN(sizeof(Dn_MemPtr_t))

static char const DnPrivateSource_EmptyString[] = "";
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *)
DnAccess_GetFile(char const *ac) {
 return ac ? ac : DnPrivateSource_EmptyString;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *)
DnAccess_GetLine(char const *ac) {
 return ac ? ac+(strlen(ac)+1) : DnPrivateSource_EmptyString;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *)
DnAccess_GetExpr(char const *ac) {
 char const *result;
 if (!ac) return DnPrivateSource_EmptyString;
 result = DnAccess_GetLine(ac);
 return result+(1+strlen(result));
}
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT DEBUG_NEW_ATTRIBUTE_CONST char const *)
DnAccess_GetFunc(char const *ac) {
 char const *result;
 if (!ac) return DnPrivateSource_EmptyString;
 result = DnAccess_GetExpr(ac);
 return result+(1+strlen(result));
}
#endif /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */

//////////////////////////////////////////////////////////////////////////
// global vars
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
DEBUG_NEW_DATA_IMPL(unsigned int,DnGlobal_InitRecursion = 0);
#endif /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
DEBUG_NEW_DATA_IMPL(Dn_PrintFunc_t,DnGlobal_DefaultPrintFunc = DnPrivate_DefaultPrintFunc_Impl);
DEBUG_NEW_DATA_IMPL(Dn_MemPtr_t *,DnGlobal_AllocChain = NULL);
DEBUG_NEW_DATA_IMPL(Dn_State_t *,DnGlobal_GlobalState = NULL);
DEBUG_NEW_DATA_IMPL(Dn_Thread_t *,DnGlobal_Threads = NULL);
DEBUG_NEW_DATA_IMPL(DEBUG_NEW_OS_TLS_T,DnPrivateGlobal_CurrentThread);
#ifdef DEBUG_NEW_OS_MUTEX_T
DEBUG_NEW_DATA_IMPL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_AllocChainLock);
DEBUG_NEW_DATA_IMPL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_ThreadsLock);
DEBUG_NEW_DATA_IMPL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_PrintLock);
DEBUG_NEW_DATA_IMPL(DEBUG_NEW_OS_MUTEX_T,DnPrivateGlobal_GlobalStateLock);
#endif // DEBUG_NEW_OS_MUTEX_T
#if DEBUG_NEW_CONFIG_HAVE_LOG
DEBUG_NEW_DATA_IMPL(void *,DnPrivateGlobal_LogFile = NULL);
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_OPEN()      ((DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile = DEBUG_NEW_STATIC_CAST(void *)(fopen(DEBUG_NEW_CONFIG_LOG_FILENAME,"a"))) != NULL)
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_INIT()      ((DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile = NULL),DEBUG_NEW_PRIVATE_EMPTY_EXPR)
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_QUIT()      ((DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile) ? (fclose(DEBUG_NEW_REINTERPRET_CAST(FILE *)(DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile)),(DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile = 0),DEBUG_NEW_PRIVATE_EMPTY_EXPR) : DEBUG_NEW_PRIVATE_EMPTY_EXPR)
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_DO_WRITE(s) (fwrite((void const *)(s),1,strlen((char const *)(s)),DEBUG_NEW_REINTERPRET_CAST(FILE *)(DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile)),DEBUG_NEW_PRIVATE_EMPTY_EXPR)
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_WRITE(s)    ((DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_LogFile) ? DEBUG_NEW_PRIVATE_SOURCE_LOG_DO_WRITE(s) : (DEBUG_NEW_PRIVATE_SOURCE_LOG_OPEN() ? DEBUG_NEW_PRIVATE_SOURCE_LOG_DO_WRITE(s) : DEBUG_NEW_PRIVATE_EMPTY_EXPR))
#else // DEBUG_NEW_CONFIG_HAVE_LOG
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_OPEN()      DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_INIT()      DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_QUIT()      DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_DO_WRITE(s) DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_LOG_WRITE(s)    DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif // !DEBUG_NEW_CONFIG_HAVE_LOG
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
DEBUG_NEW_DATA_IMPL(size_t,DnPrivateGlobal_NextAllocID = 0);
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_INIT()     DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_NextAllocID = 0
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_QUIT()     DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_GET_NEXT() DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_NextAllocID++
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_INIT()     DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_QUIT()     DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_GET_NEXT() 0
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */


#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID &&\
    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
DEBUG_NEW_DATA_IMPL(size_t,DnPrivateGlobal_AllocationBreakpoint = 0);
DEBUG_NEW_DATA_IMPL(int,DnPrivateGlobal_AllocationBreakpointIsSet = 0);
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK() (\
 (DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_AllocationBreakpointIsSet &&\
 (DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_NextAllocID == DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_AllocationBreakpoint))\
 ? (DEBUG_NEW_BREAKPOINT(),1) : 0)
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_INIT()  DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_AllocationBreakpointIsSet = 0
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_QUIT()  DEBUG_NEW_PRIVATE_EMPTY_EXPR
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK() DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_INIT()  DEBUG_NEW_PRIVATE_EMPTY_EXPR
# define DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_QUIT()  DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */





//////////////////////////////////////////////////////////////////////////
// Init / Quit
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) Dn_InitInternal(char const *ac) {
 DnPrivate_InitInternal(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}

#if !DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION && !DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE
static unsigned char debug_new_initialized = 0;
#define DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_INITIALIZED() (DEBUG_NEW_CXX_NAMESPACE_ROOT debug_new_initialized = 1)
#define DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_FINALIZED()   (DEBUG_NEW_CXX_NAMESPACE_ROOT debug_new_initialized = 0)
#else
#define DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_INITIALIZED() DEBUG_NEW_PRIVATE_EMPTY_EXPR
#define DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_FINALIZED()   DEBUG_NEW_PRIVATE_EMPTY_EXPR
#endif

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) Dn_Initialized(void) {
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
 return DnGlobal_InitRecursion != 0;
#elif defined(DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE) && (DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE-0)
 return DnGlobal_GlobalState != NULL;
#else /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
 return debug_new_initialized != 0;
#endif /* !DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_InitInternal(char const *ac, void const *addr) {
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
 if (++DnGlobal_InitRecursion != 1) return;
#else /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
 DN_ASSERTE(!Dn_Initialized() && "Debug_New is already initialized",{return;});
#endif /* !DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
#ifdef DEBUG_NEW_API_NATIVE_INITIALIZE
 DEBUG_NEW_API_NATIVE_INITIALIZE();
#endif /* DEBUG_NEW_API_NATIVE_INITIALIZE */
 DEBUG_NEW_PRIVATE_SOURCE_LOG_INIT();
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_INIT();
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_INIT();
 DN_ASSERT(!DnGlobal_AllocChain  && "DnGlobal_AllocChain is already initialized");
 DN_ASSERT(!DnGlobal_GlobalState && "DnGlobal_GlobalState is already initialized");
 DN_ASSERT(!DnGlobal_Threads     && "DnGlobal_Threads is already initialized");
 DEBUG_NEW_OS_TLS_INIT(DnPrivateGlobal_CurrentThread);
 DEBUG_NEW_OS_MUTEX_INIT(DnPrivateGlobal_AllocChainLock);
 DEBUG_NEW_OS_MUTEX_INIT(DnPrivateGlobal_ThreadsLock);
 DEBUG_NEW_OS_MUTEX_INIT(DnPrivateGlobal_PrintLock);
 DEBUG_NEW_OS_MUTEX_INIT(DnPrivateGlobal_GlobalStateLock);
#if defined(DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE) && (DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE-0)
 DnPrivate_State_PushGlobal(ac,addr); 
 //^ push the global 0-memory state
#endif // DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE
 DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_INITIALIZED();
}
DEBUG_NEW_FUNC_IMPL(void) Dn_QuitInternal(char const *ac) {
 DnPrivate_QuitInternal(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_QuitInternal(char const *ac, void const *addr) {
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION
 if (--DnGlobal_InitRecursion != 0) return;
#else /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
 DN_ASSERTE(Dn_Initialized() && "Debug_New is not initialized",{return;});
#endif /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION */
 // v pop any remaining global states
 //   (this includes the report-memoryleaks-on-shutdown state,
 //    if "DEBUG_NEW_CONFIG_HAVE_AUTO_GLOBAL_STATE" is enabled)
 while (DnGlobal_GlobalState)
  DnPrivate_State_PopGlobal(ac,addr); 
#if DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN
 {Dn_MemPtr_t *next,*allocs = DnGlobal_AllocChain;
  // ** free all non-freed pointers
  //    (depending on the configuration, 
  //     there probably are none)
  while (allocs) {
   next = allocs->p_global_prev;
   DnPrivate_FixMemoryLeak(allocs,ac,addr);
   allocs = next;
  }
  DnGlobal_AllocChain = NULL;
 }
#endif /* DEBUG_NEW_CONFIG_HAVE_FREE_LEAKS_ON_SHUTDOWN */
 
 // ** free all non-popped threads
 {Dn_Thread_t *thread_next,*threads = DnGlobal_Threads;
  while (threads) {
   thread_next = threads->t_prev;
   DnPrivate_Thread_Free(threads,ac,addr);
   threads = thread_next;
  }
  DnGlobal_Threads = NULL;
 }
 DEBUG_NEW_OS_MUTEX_QUIT(DnPrivateGlobal_GlobalStateLock);
 DEBUG_NEW_OS_MUTEX_QUIT(DnPrivateGlobal_PrintLock);
 DEBUG_NEW_OS_MUTEX_QUIT(DnPrivateGlobal_ThreadsLock);
 DEBUG_NEW_OS_MUTEX_QUIT(DnPrivateGlobal_AllocChainLock);
 DEBUG_NEW_OS_TLS_QUIT(DnPrivateGlobal_CurrentThread);
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_QUIT();
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_QUIT();
 DEBUG_NEW_PRIVATE_SOURCE_LOG_QUIT();
#ifdef DEBUG_NEW_API_NATIVE_FINALIZE
 DEBUG_NEW_API_NATIVE_FINALIZE();
#endif /* DEBUG_NEW_API_NATIVE_FINALIZE */
 DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_FINALIZED();
}

#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION && DEBUG_NEW_CONFIG_HAVE_DEBUG
# define DEBUG_NEW_PRIVATE_SOURCE_CHECK_INIT(err_ret) \
 DN_ASSERTE(DnGlobal_InitRecursion != 0 && "Debug_new is not initialized",{return err_ret;})
#else /* DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION && DEBUG_NEW_CONFIG_HAVE_DEBUG */
# define DEBUG_NEW_PRIVATE_SOURCE_CHECK_INIT(err_ret) do{}while(0)
#endif /* !DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION || !DEBUG_NEW_CONFIG_HAVE_DEBUG */


//////////////////////////////////////////////////////////////////////////
// print locks
//////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_NEW_OS_MUTEX_T
# define DEBUG_NEW_PRINT_LOCK_ENTER DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_PrintLock); {
# define DEBUG_NEW_PRINT_LOCK_LEAVE } DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_PrintLock);
#else // DEBUG_NEW_OS_MUTEX_T
# define DEBUG_NEW_PRINT_LOCK_ENTER {
# define DEBUG_NEW_PRINT_LOCK_LEAVE }
#endif // !DEBUG_NEW_OS_MUTEX_T


//////////////////////////////////////////////////////////////////////////
// Print function helpers
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_DefaultPrintFunc_Impl(char const *msg) {
 DEBUG_NEW_PRIVATE_SOURCE_LOG_WRITE(msg);
 DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING(msg);
 fputs(msg,stderr);
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t)
Dn_GetDefaultPrintFunc(void) {
 Dn_PrintFunc_t result;
 DEBUG_NEW_PRINT_LOCK_ENTER
 result = DnGlobal_DefaultPrintFunc;
 DEBUG_NEW_PRINT_LOCK_LEAVE
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t)
DnPrivate_GetDefaultPrintFuncAlreadyLocked(void) {
 return DnGlobal_DefaultPrintFunc;
}
DEBUG_NEW_FUNC_IMPL(void) Dn_SetDefaultPrintFunc(Dn_PrintFunc_t func) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_SetDefaultPrintFuncAlreadyLocked(func);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_SetDefaultPrintFuncAlreadyLocked(Dn_PrintFunc_t func) {
 // v setting the default print function as 0 restores the default print function
 DnGlobal_DefaultPrintFunc = func ? func : DnPrivate_DefaultPrintFunc_Impl;
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_PrintFunc_t)
DnThread_GetPrintFunc(void) {
 Dn_PrintFunc_t result;
 Dn_Thread_t *thread = DnThread_Get();
 if (!thread) return DnPrivate_DefaultPrintFunc_Impl;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
 if (thread->t_print) {
  result = thread->t_print->ps_func;
  DN_ASSERTE(result && 
   "The current thread is listing a print "
   "function, but that function is NULL",{
    result = DnPrivate_DefaultPrintFunc_Impl;});
 } else {
  result = DnPrivate_DefaultPrintFunc_Impl;
  // use default print function implementation
 }
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 return result;
}
DEBUG_NEW_FUNC_IMPL(int)
DnThread_PushPrintFunc(Dn_PrintFunc_t func, char const *ac) {
 return DnPrivate_Thread_PushPrintFunc2(func,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnPrivate_Thread_PushPrintFunc2(Dn_PrintFunc_t func, char const *ac, void const *addr) {
 Dn_Thread_t *thread;
 Dn_PrintFuncStack_t *stack;
 if (!func) {
  // TODO: log warning
  return 0;
 }
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 if ((thread = DnPrivate_Thread_GetAlreadyLocked(ac,addr)) == NULL) goto err0;
 if ((stack = (Dn_PrintFuncStack_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(
  sizeof(Dn_PrintFuncStack_t))) == NULL) goto err0;
 stack->ps_access = ac;
 stack->ps_addr = addr;
 stack->ps_func = func;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock); 
 stack->ps_prev = thread->t_print;
 thread->t_print = stack;
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return 1;
err0:
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return 0;
}
#if DEBUG_NEW_CONFIG_HAVE_LIMITED_API
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnPrivate_Thread_PopPrintFuncInternalAlreadyLocked(Dn_Thread_t *thread, char const *ac, void const *addr) {
 Dn_PrintFuncStack_t *old;
 DN_ASSERTE(thread && "Invalid Thread",{return 0;});
 if (!thread->t_print) {
  DnPrivate_HandleNoPopPrintFunc(thread,ac,addr);
  return 0;
 }
 old = thread->t_print;
 thread->t_print = thread->t_print->ps_prev;
 DEBUG_NEW_API_NATIVE_FREE_NN(old);
 return 1;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnPrivate_Thread_PopPrintFuncInternal(Dn_Thread_t *thread, char const *ac, void const *addr) {
 int result;
 DN_ASSERTE(thread && "Invalid Thread",{return 0;});
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock); 
 result = DnPrivate_Thread_PopPrintFuncInternalAlreadyLocked(thread,ac,addr);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 return result;
}
#endif /* DEBUG_NEW_CONFIG_HAVE_LIMITED_API */
DEBUG_NEW_FUNC_IMPL(int) DnThread_PopPrintFunc(char const *ac) {
 return DnPrivate_Thread_PopPrintFunc2(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int) DnPrivate_Thread_PopPrintFunc2(char const *ac, void const *addr) {
 Dn_Thread_t *thread;
 Dn_PrintFuncStack_t *old;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 thread = DnThread_Get();
 if (!thread || !thread->t_print) {
  DnPrivate_HandleNoPopPrintFunc(thread,ac,addr);
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
  return 0;
 }
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
 old = thread->t_print;
 thread->t_print = thread->t_print->ps_prev;
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 DEBUG_NEW_API_NATIVE_FREE_NN(old);
 if (!thread->t_print && !thread->t_state && !thread->t_allocs) {
  DnPrivate_Thread_RemInternalAlreadyLocked(thread,ac,addr);
  // we have to kill our own thread here,
  // to prevent other threads from doing so
  // without setting "DnPrivateGlobal_CurrentThread" of
  // use to NULL
  DEBUG_NEW_OS_TLS_SET(DnPrivateGlobal_CurrentThread,0);
 }
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return 1;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT unsigned int) DnThread_GetPrintFuncStackSize(void) {
 unsigned int result = 0;
 Dn_Thread_t *thread;
 Dn_PrintFuncStack_t *stack;
 if ((thread = DnThread_Get()) != NULL) {
  DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
  stack = thread->t_print;
  while (stack) { ++result; stack = stack->ps_prev; }
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 }
 return result;
}





//////////////////////////////////////////////////////////////////////////
// print helpers
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrint_FileAndLine(char const *ac, void const *addr) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_FileAndLineAlreadyLocked(DnThread_GetPrintFunc(),ac,addr);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_FileAndLineAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac, void const *addr) {
 char const *file,*line;
 if (ac) {
  file = DnAccess_GetFile(ac);
  line = DnAccess_GetLine(ac);
  if (*file && *line)
   DnPrivate_PrintfAlreadyLocked(print_func,DEBUG_NEW_PRIVATE_FILE_AND_LINE_FORMAT,file,line);
 } else {
  // TODO: use "addr2line", to display more detailed location information
  DnPrivate_PrintfAlreadyLocked(print_func,"Code at 0x%p : ",addr);
 }
}
DEBUG_NEW_FUNC_IMPL(void) DnPrint_Thread(DEBUG_NEW_OS_THREAD_ID_T const thread) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_ThreadAlreadyLocked(DnThread_GetPrintFunc(),thread);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_ThreadAlreadyLocked(Dn_PrintFunc_t print_func, DEBUG_NEW_OS_THREAD_ID_T const thread) {
 DnPrivate_PrintfAlreadyLocked(print_func,"Thread %lu : ",(unsigned long)thread);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrint_ExprInFunc(char const *ac) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_ExprInFuncAlreadyLocked(DnThread_GetPrintFunc(),ac);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_ExprInFuncAlreadyLocked(Dn_PrintFunc_t print_func, char const *ac) {
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
 char const *func;
#endif /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
 if (ac) {
  DnPrivate_PrintAlreadyLocked(print_func,DnAccess_GetExpr(ac));
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
  func = DnAccess_GetFunc(ac);
  if (*func) DnPrivate_PrintfAlreadyLocked(print_func," in %s",func);
#endif /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
 }
}
DEBUG_NEW_FUNC_IMPL(void) DnPrint_ObjectAt(Dn_StatePtr_t const *state) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_ObjectAtAlreadyLocked(DnThread_GetPrintFunc(),state);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_ObjectAtAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state) {
 char const *expr;
 DN_ASSERTE(state && "Invalid state",{return;});
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
 DnPrivate_PrintfAlreadyLocked(print_func,
  "object " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT
  " at 0x%p (size: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " byte",
  DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(state->p_alloc_id),
  state->p_self,DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(state->p_size));
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
 DnPrivate_PrintfAlreadyLocked(print_func,
  "object at 0x%p (size: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " byte",
  DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(state->p_self,state->p_size));
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
 if (state->p_size > DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD)
  DnPrivate_Print_SizeApproximationAlreadyLocked(print_func,state->p_size);
 DnPrivate_PrintAlreadyLocked(print_func,"); ");
 expr = DnAccess_GetExpr(state->p_access);
 if (*expr) DnPrivate_PrintfAlreadyLocked(print_func,"%s",expr);
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW) || \
   defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW)
 else switch (state->p_type) {
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW:
   DnPrivate_PrintfAlreadyLocked(print_func,"new");
   break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW:
   DnPrivate_PrintfAlreadyLocked(print_func,"new[]");
   break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
 }
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW || DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
 if (state->p_alloc_info) DnPrivate_PrintfAlreadyLocked(print_func," (with\"%s\")",state->p_alloc_info);
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
 DnPrivate_PrintfAlreadyLocked(print_func," in %s",DnAccess_GetFunc(state->p_access));
#endif /* DEBUG_NEW_PRIVATE_HAVE_PP_FUNC */
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
}


DEBUG_NEW_FUNC_IMPL(void) DnPrint_SizeApproximation(size_t s) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_SizeApproximationAlreadyLocked(DnThread_GetPrintFunc(),s);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_SizeApproximationAlreadyLocked(Dn_PrintFunc_t print_func, size_t s) {
 double s2;
#define DEBUG_NEW_PRIVATE_SOURCE_GB_SIZE (1024*1024*1024)
#define DEBUG_NEW_PRIVATE_SOURCE_MB_SIZE (1024*1024)
#define DEBUG_NEW_PRIVATE_SOURCE_KB_SIZE (1024)
 if (s >= DEBUG_NEW_PRIVATE_SOURCE_GB_SIZE) { /* Gigabyte */
  s2 = DEBUG_NEW_STATIC_CAST(double)(s)/DEBUG_NEW_PRIVATE_SOURCE_GB_SIZE;
  DnPrivate_PrintfAlreadyLocked(print_func," ~%.2f Gb",s2);
 } else if (s >= DEBUG_NEW_PRIVATE_SOURCE_MB_SIZE) { /* Megabyte */
  s2 = DEBUG_NEW_STATIC_CAST(double)(s)/DEBUG_NEW_PRIVATE_SOURCE_MB_SIZE;
  DnPrivate_PrintfAlreadyLocked(print_func," ~%.2f Mb",s2);
 } else if (s >= DEBUG_NEW_PRIVATE_SOURCE_KB_SIZE) { /* Kilobyte */
  s2 = DEBUG_NEW_STATIC_CAST(double)(s)/DEBUG_NEW_PRIVATE_SOURCE_KB_SIZE;
  DnPrivate_PrintfAlreadyLocked(print_func," ~%.2f Kb",s2);
 }
#undef DEBUG_NEW_PRIVATE_SOURCE_KB_SIZE
#undef DEBUG_NEW_PRIVATE_SOURCE_MB_SIZE
#undef DEBUG_NEW_PRIVATE_SOURCE_GB_SIZE
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_AccessMethod(
 char const *ac, void const *addr, Dn_AllocType_t method) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_AccessMethodAlreadyLocked(DnThread_GetPrintFunc(),ac,addr,method);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_AccessMethodAlreadyLocked(
 Dn_PrintFunc_t print_func, char const *ac, void const *addr, Dn_AllocType_t method) {
 char const *expr;
 DEBUG_NEW_UNREFERENCED(addr);
 switch (method) {
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE:
   DnPrivate_PrintAlreadyLocked(print_func,"delete");
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE:
   DnPrivate_PrintAlreadyLocked(print_func,"delete[]");
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
  case DEBUG_NEW_ALLOC_CALL_TYPE_FREE:
   DnPrivate_PrintAlreadyLocked(print_func,"free(...)");
   break;
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE:
   DnPrivate_PrintAlreadyLocked(print_func,"aligned_free(...)");
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE */
  default:
   expr = DnAccess_GetExpr(ac);
   if (*expr) DnPrivate_PrintAlreadyLocked(print_func,expr);
   else      DnPrivate_PrintfAlreadyLocked(print_func,"<unknown: %d>",DEBUG_NEW_STATIC_CAST(int)(method));
   break;
 }
}


//////////////////////////////////////////////////////////////////////////
// Handlers
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleShouldBePlacementCall(void *ptr, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,"this\"new\" statement for 0x%p should be\"placement new\"",ptr);
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleOutOfMemory(size_t alloc_s, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Out of memory when allocating " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " bytes; ",
  DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(alloc_s));
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
#if DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleTooBigAlloc(size_t alloc_s, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Too big allocation requested: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " bytes; ",
  DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(alloc_s));
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
#endif // DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
DEBUG_NEW_FUNC_DECL(void) DnPrivate_HandleNotXXXViolation(
 char const *call_overview, char const *func_name,
 char const *alt_name, char const *violation_value, 
 char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,
  "%s : Undefined behavior : %s cannot be used as parameter, "
  "when calling %s (call %s instead)",call_overview,violation_value,func_name,alt_name);
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleStrDupNull(char const *func_name, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,"%s(0) : Calling %s with NULL causes undefined behavior; ",func_name,func_name);
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleInvalidMagic(Dn_MemPtr_t const *rec, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 /* v byte offset from allocation address */
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Invalid pointer magic for pointer 0x%p (magic offset %-5ld); ",
  addr,DEBUG_NEW_STATIC_CAST(long)(
  DEBUG_NEW_REINTERPRET_CAST(char const *)(rec->p_state.p_self)-
  DEBUG_NEW_REINTERPRET_CAST(char const *)(&rec->p_magic)));
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleUnalignedPointer(void const *p, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_PrintfAlreadyLocked(print_func,"warning : memory unaligned at 0x%p\n",p);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleTailCorruption(Dn_MemPtr_t const *p, char const *ac, void const *addr, void const *corruption_address) {
 Dn_PrintFunc_t print_func;
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 /* v byte offset from allocation address */
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Tail corruption at 0x%p (offset %lu); ",
  corruption_address,DEBUG_NEW_STATIC_CAST(unsigned long)(
  DEBUG_NEW_REINTERPRET_CAST(char const *)(corruption_address)-
  DEBUG_NEW_REINTERPRET_CAST(char const *)(p->p_state.p_self)));
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,":\n");
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,p->p_state.p_access,addr);
 DnPrivate_PrintAlreadyLocked(print_func,"in ");
 DnPrivate_Print_ObjectAtAlreadyLocked(print_func,&p->p_state);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleNoPopState(char const *ac, void const *addr, int is_global) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintfAlreadyLocked(print_func,"Can\'t pop %s state! no states available (", is_global ? "global" : "thread); ");
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleNoPopPrintFunc(const Dn_Thread_t *thread, char const *ac, void const *addr) {
 Dn_PrintFunc_t print_func;
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,thread ? thread->t_thread_id : DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintAlreadyLocked(print_func,"Can\'t pop print func! no functions available; ");
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleInvalidDeallocKind(char const *ac, void const *addr, Dn_MemPtr_t const *p, Dn_AllocType_t should_be, Dn_AllocType_t is) {
 Dn_PrintFunc_t print_func;
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 DEBUG_NEW_PRINT_LOCK_ENTER
 print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintAlreadyLocked(print_func,"Invalid free method: using\"");
 DnPrivate_Print_AccessMethodAlreadyLocked(print_func,ac,addr,is);
 DnPrivate_PrintAlreadyLocked(print_func,"\" instead of\"");
 DnPrivate_Print_AccessMethodAlreadyLocked(print_func,ac,addr,should_be);
 DnPrivate_PrintAlreadyLocked(print_func,"\"\n");
 if (p) {
  // print info on the object in question
  DnPrivate_Print_FileAndLineAlreadyLocked(print_func,p->p_state.p_access,p->p_state.p_address);
  DnPrivate_Print_ThreadAlreadyLocked(print_func,p->p_state.p_thread);
  DnPrivate_PrintAlreadyLocked(print_func,"Allocated ");
  DnPrivate_Print_ObjectAtAlreadyLocked(print_func,&p->p_state);
 }
 DEBUG_NEW_PRINT_LOCK_LEAVE
}

#if !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_HandleFreeNullPointer(char const *ac, void const *addr) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 Dn_PrintFunc_t print_func = DnThread_GetPrintFunc();
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,DEBUG_NEW_OS_THREAD_SELF());
 DnPrivate_PrintAlreadyLocked(print_func,"Tried to free 0-pointer\n");
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL */




DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_InvalidMemoryLeak(Dn_StatePtr_t const *state) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Dump_InvalidMemoryLeakAlreadyLocked(DnThread_GetPrintFunc(),state);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_InvalidMemoryLeakAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state) {
 if (!state) return;
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,state->p_access,state->p_address);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,state->p_thread);
 DnPrivate_PrintAlreadyLocked(print_func,"Leaked ");
 DnPrivate_Print_ObjectAtAlreadyLocked(print_func,state);
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_InvalidMemoryFree(Dn_StatePtr_t const *state) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Dump_InvalidMemoryFreeAlreadyLocked(DnThread_GetPrintFunc(),state);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_InvalidMemoryFreeAlreadyLocked(Dn_PrintFunc_t print_func, Dn_StatePtr_t const *state) {
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 if (!state) return;
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,state->p_access,state->p_address);
 DnPrivate_Print_ThreadAlreadyLocked(print_func,state->p_thread);
 DnPrivate_PrintAlreadyLocked(print_func,"Freed ");
 DnPrivate_Print_ObjectAtAlreadyLocked(print_func,state);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_LeaksFirstMessage(Dn_State_t const *state, char const *ac, void const *addr) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Print_LeaksFirstMessageAlreadyLocked(DnThread_GetPrintFunc(),state,ac,addr);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_LeaksFirstMessageAlreadyLocked(Dn_PrintFunc_t print_func, Dn_State_t const *state, char const *ac, void const *addr) {
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 DN_ASSERTE(state && "Invalid state",{return;});
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,state->s_access,state->s_access_addr);
 DnPrivate_PrintAlreadyLocked(print_func,"+++ pushed +++ ");
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,state->s_access);
 DnPrivate_PrintAlreadyLocked(print_func,"\n");
 DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ac,addr);
 DnPrivate_PrintAlreadyLocked(print_func,"--- popped --- ");
 DnPrivate_Print_ExprInFuncAlreadyLocked(print_func,ac);
 DnPrivate_PrintAlreadyLocked(print_func,":\n");
 DnPrivate_PrintAlreadyLocked(print_func,">>> Begin logging of leaks / frees (most recent leak / free first):\n");
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_LeakStatistics(char const *ac, void const *addr,
 unsigned int ntotal_leaks, size_t total_leak,
 unsigned int ntotal_frees, size_t total_free) {
 DEBUG_NEW_PRINT_LOCK_ENTER 
 DnPrivate_Print_LeakStatisticsAlreadyLocked(DnThread_GetPrintFunc(),ac,addr,ntotal_leaks,total_leak,ntotal_frees,total_free);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print_LeakStatisticsAlreadyLocked(
 Dn_PrintFunc_t print_func, char const *ac, void const *addr,
 unsigned int ntotal_leaks, size_t total_leak,
 unsigned int ntotal_frees, size_t total_free) {
 DEBUG_NEW_UNREFERENCED(ac);
 DEBUG_NEW_UNREFERENCED(addr);
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Total leaks: %u (total: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " byte",
  ntotal_leaks,DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(total_leak));
 if (total_leak > DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD)
  DnPrivate_Print_SizeApproximationAlreadyLocked(print_func,total_leak);
 DnPrivate_PrintAlreadyLocked(print_func,")\n");
 DnPrivate_PrintfAlreadyLocked(print_func,
  "Total frees: %u (total: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " byte",
  ntotal_frees,DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(total_free));
 if (total_free > DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD)
  DnPrivate_Print_SizeApproximationAlreadyLocked(print_func,total_free);
 DnPrivate_PrintAlreadyLocked(print_func,")\n");
 DnPrivate_PrintAlreadyLocked(print_func,">>> End logging of leaks / frees\n\n");
}




//////////////////////////////////////////////////////////////////////////
// Public dumping methods
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_HAVE_STATS
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_Stats(char const *prefix, Dn_Stat_t *stats) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Dump_StatsAlreadyLocked(DnThread_GetPrintFunc(),prefix,stats);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_StatsAlreadyLocked(Dn_PrintFunc_t print_func, char const *prefix, Dn_Stat_t *stats) {
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 DN_ASSERTE(stats && "Invalid stats",{return;});
#ifdef __cplusplus
# define DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "::"
#else /* __cplusplus */
# define DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "."
#endif /* !__cplusplus */
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "malloc            = %u;\n",prefix,DnStat_GetTotalCalls_Malloc(stats));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "realloc           = %u;\n",prefix,DnStat_GetTotalCalls_Realloc(stats));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calloc            = %u;\n",prefix,DnStat_GetTotalCalls_Calloc(stats));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "free              = %u;\n",prefix,DnStat_GetTotalCalls_Free(stats));
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "operator new      = %u;\n",prefix,DnStat_GetTotalCalls_New(stats));
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "operator new[]    = %u;\n",prefix,DnStat_GetTotalCalls_ArrayNew(stats));
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "operator delete   = %u;\n",prefix,DnStat_GetTotalCalls_Delete(stats));
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "operator delete[] = %u;\n",prefix,DnStat_GetTotalCalls_ArrayDelete(stats));
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "aligned_malloc    = %u;\n",prefix,stats->total_calls_aligned_malloc);
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "aligned_realloc   = %u;\n",prefix,stats->total_calls_aligned_realloc);
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "aligned_calloc    = %u;\n",prefix,stats->total_calls_aligned_calloc);
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "calls" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "aligned_free      = %u;\n",prefix,stats->total_calls_aligned_free);
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "alloc" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "blocks" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "total      = %u;\n",prefix,DnStat_GetTotalAllocBlocks(stats));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "alloc" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "blocks" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "max        = %u;\n",prefix,DnStat_GetMaxAllocBlocks(stats));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "alloc" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "blocks" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "total      = " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT ";\n",prefix,DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(DnStat_GetTotalAlloc(stats)));
 DnPrivate_PrintfAlreadyLocked(print_func,"%sstats" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "alloc" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "blocks" DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP "max        = " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT ";\n",prefix,DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(DnStat_GetMaxAlloc(stats)));
#undef DEBUG_NEW_PRIVATE_SOURCE_GROUP_SEP
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_ThreadStats(Dn_Thread_t *thread) {
 char prefix[64];
 if (!thread) return;
 sprintf(prefix,"mem.Thread[%lu].",(unsigned long)thread->t_thread_id);
 DnPrivate_Dump_Stats(prefix,&thread->t_stats);
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_ThreadStatsEx(DEBUG_NEW_OS_THREAD_ID_T thread_id) {
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DnPrivate_Dump_ThreadStats(DnPrivate_Thread_FindAlreadyLocked(thread_id));
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_ThreadStats(void) {
 DnPrivate_Dump_ThreadStats(DnThread_Get());
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_GlobalStats(void) {
 Dn_Stat_t stats;
 DnStat_GetGlobal(&stats);
 DnPrivate_Dump_Stats("mem.global.",&stats);
}
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */

#if DEBUG_NEW_CONFIG_HAVE_DEBUG
#define DnCheck_PointerConsistency(p) (\
 DN_ASSERT(((p)->p_thread_next?(p)->p_thread_next->p_thread_prev==(p):1)&&"Linked pointer list inconsistency: Thread (this->next->prev)") &&\
 DN_ASSERT(((p)->p_thread_prev?(p)->p_thread_prev->p_thread_next==(p):1)&&"Linked pointer list inconsistency: Thread (this->prev->next)") &&\
 DN_ASSERT(((p)->p_global_next?(p)->p_global_next->p_global_prev==(p):1)&&"Linked pointer list inconsistency: Global (this->next->prev)") &&\
 DN_ASSERT(((p)->p_global_prev?(p)->p_global_prev->p_global_next==(p):1)&&"Linked pointer list inconsistency: Global (this->prev->next)"))
#else /* DEBUG_NEW_CONFIG_HAVE_DEBUG */
#define DnCheck_PointerConsistency(p) 1
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEBUG */

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_Allocations(Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Dump_AllocationsAlreadyLocked(DnThread_GetPrintFunc(),ptr,offset_of_prev);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_AllocationsAlreadyLocked(Dn_PrintFunc_t print_func, Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev) {
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
 ) && "Invalid offset_of_prev",{return;});
 while (ptr) {
  if (!DnCheck_PointerConsistency(ptr)) return;
  DnPrivate_Print_FileAndLineAlreadyLocked(print_func,ptr->p_state.p_access,ptr->p_state.p_address);
  DnPrivate_Print_ThreadAlreadyLocked(print_func,ptr->p_state.p_thread);
  DnPrivate_Print_ObjectAtAlreadyLocked(print_func,&ptr->p_state);
  ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
   /*!tinyc!DEBUG_NEW_REINTERPRET_CAST*/((char *)ptr)+offset_of_prev);
 }
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_MostAllocations(Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev, int most_calls, int merge) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_Dump_MostAllocationsAlreadyLocked(DnThread_GetPrintFunc(),ptr,offset_of_prev,most_calls,merge);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}

typedef struct Dn_VerboseStatePtr_t {
 unsigned int    p_ncalls;  /*< amount of allocation calls. */
 Dn_AllocType_t  p_type;    /*< type of allocation. */
 size_t          p_size;    /*< size of this allocation. */
 char const     *p_access;  /*< allocation access data. */
 void const     *p_address; /*< allocation code address. */
} Dn_VerboseStatePtr_t;


#define DEBUG_NEW_LOCAL_LIST_BUF_SIZE 128
#define DEBUG_NEW_LOCAL_LIST_FREE() DEBUG_NEW_API_NATIVE_FREE_NN(sorted_list)
#define DEBUG_NEW_LOCAL_LIST_REALLOC(n)\
do{\
 sorted_list_length = (n);\
 if ((sorted_list = (Dn_VerboseStatePtr_t *)DEBUG_NEW_API_NATIVE_REALLOC_NNZ(sorted_list,sorted_list_length*sizeof(Dn_VerboseStatePtr_t))) == NULL) {\
  DEBUG_NEW_API_NATIVE_FREE_NN(sorted_list);\
  return;\
 }\
}while(0)
#define DEBUG_NEW_LOCAL_LIST_RESERVE_1()\
do{\
 if (++sorted_list_used == sorted_list_length)\
  DEBUG_NEW_LOCAL_LIST_REALLOC(sorted_list_length+DEBUG_NEW_LOCAL_LIST_BUF_SIZE);\
}while(0)
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Dump_MostAllocationsAlreadyLocked(Dn_PrintFunc_t print_func, Dn_MemPtr_t *ptr, ptrdiff_t offset_of_prev, int most_calls, int merge) {
 Dn_VerboseStatePtr_t *sorted_list,*p,shift;
 unsigned int iter,i,sorted_list_used = 0,
  sorted_list_length = DEBUG_NEW_LOCAL_LIST_BUF_SIZE;
 void const *my_addr;
 size_t shift_start,shift_end,shift_size;
 char const *expr;
 DN_ASSERTE(print_func && "Invalid print function",{return;});
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
  ) && "Invalid offset_of_prev",{return; });
 if ((sorted_list = (Dn_VerboseStatePtr_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(
  DEBUG_NEW_LOCAL_LIST_BUF_SIZE*sizeof(Dn_VerboseStatePtr_t))) == NULL) return;
 while (ptr) {
  if (!DnCheck_PointerConsistency(ptr)) {
   DEBUG_NEW_LOCAL_LIST_FREE();
   return;
  }
  my_addr = ptr->p_state.p_address;
  for (i = 0; i < sorted_list_used; ++i) {
   if (merge && sorted_list[i].p_address == my_addr) {
    sorted_list[i].p_size += ptr->p_state.p_size;
    ++sorted_list[i].p_ncalls;
    goto next_ptr;
   }
  }
  DEBUG_NEW_LOCAL_LIST_RESERVE_1();
  sorted_list[sorted_list_used-1].p_access = ptr->p_state.p_access;
  sorted_list[sorted_list_used-1].p_address = ptr->p_state.p_address;
  sorted_list[sorted_list_used-1].p_ncalls = 1;
  sorted_list[sorted_list_used-1].p_size = ptr->p_state.p_size;
  sorted_list[sorted_list_used-1].p_type = ptr->p_state.p_type;
next_ptr:
  ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
   /*!tinyc!DEBUG_NEW_REINTERPRET_CAST*/((char *)ptr)+offset_of_prev);
 }
 // ** actually sort the list
 p = sorted_list;
 iter = sorted_list_used;
 while (iter--) {
  shift_start = DEBUG_NEW_STATIC_CAST(size_t)(p-sorted_list);
  shift_end = shift_start+1;
  if (most_calls) {
   while (1) {
    if (shift_end >= sorted_list_used) break;
    if (p->p_ncalls > sorted_list[shift_end].p_ncalls) break;
    ++shift_end;
   }
  } else {
   while (1) {
    if (shift_end >= sorted_list_used) break;
    if (p->p_size > sorted_list[shift_end].p_size) break;
    ++shift_end;
   }
  }
  --shift_end;
  shift_size = shift_end-shift_start;
  if (shift_size) {
   shift = sorted_list[shift_start];
   memcpy(sorted_list+shift_start,
          sorted_list+(shift_start+1),
          shift_size*sizeof(Dn_VerboseStatePtr_t));
   sorted_list[shift_end] = shift;
  }
 }
 // ** print the list
 p = sorted_list;
 while (sorted_list_used) {
  DnPrivate_Print_FileAndLineAlreadyLocked(print_func,p->p_access,p->p_address);
  DnPrivate_PrintfAlreadyLocked(
   print_func,"%d object%s (size: " DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT " byte",
   p->p_ncalls,(p->p_ncalls == 1) ? "" : "s",DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG(p->p_size));
  if (p->p_size > DEBUG_NEW_PRIVATE_SIZE_APROX_THRESHOLD)
   DnPrivate_Print_SizeApproximationAlreadyLocked(print_func,p->p_size);
  DnPrivate_PrintAlreadyLocked(print_func,"); ");
  expr = DnAccess_GetExpr(p->p_access);
  if (*expr) DnPrivate_PrintfAlreadyLocked(print_func,"%s",expr);
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW) || \
    defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW)
  else switch (p->p_type) {
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
   case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW:
    DnPrivate_PrintfAlreadyLocked(print_func,"new");
    break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
   case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW:
    DnPrivate_PrintfAlreadyLocked(print_func,"new[]");
    break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
   default: break;
  }
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW || DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
  DnPrivate_PrintAlreadyLocked(print_func,"\n");
  ++p;
  --sorted_list_used;
 }
 DEBUG_NEW_LOCAL_LIST_FREE();
}
#undef DEBUG_NEW_LOCAL_LIST_RESERVE_1
#undef DEBUG_NEW_LOCAL_LIST_REALLOC
#undef DEBUG_NEW_LOCAL_LIST_FREE
#undef DEBUG_NEW_LOCAL_LIST_BUF_SIZE
#undef DnCheck_PointerConsistency



DEBUG_NEW_FUNC_IMPL(void) DnDump_ThreadAllosEx(DEBUG_NEW_OS_THREAD_ID_T thread_id) {
 Dn_Thread_t *thread;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 if ((thread = DnPrivate_Thread_FindAlreadyLocked(thread_id)) != NULL)
  DnPrivate_Dump_Allocations(thread->t_allocs,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_ThreadAllocs(void) {
 Dn_Thread_t *thread = DnThread_Get();
 if (thread) DnPrivate_Dump_Allocations(thread->t_allocs,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_GlobalAllocs(void) {
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
 DnPrivate_Dump_Allocations(DnGlobal_AllocChain,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
}



DEBUG_NEW_FUNC_IMPL(void) DnDump_MostThreadAllocs(int most_calls, int merge) {
 Dn_Thread_t *thread = DnThread_Get();
 if (thread) DnPrivate_Dump_MostAllocations(thread->t_allocs,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev),most_calls,merge);
}
DEBUG_NEW_FUNC_IMPL(void) DnDump_MostGlobalAllocs(int most_calls, int merge) {
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
 DnPrivate_Dump_MostAllocations(DnGlobal_AllocChain,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev),most_calls,merge);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
}







//////////////////////////////////////////////////////////////////////////
// print method implementation
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrint(char const *msg) {
 DnPrivate_Print(DnThread_GetPrintFunc(),msg);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrintf(char const *msg, ...) {
 char buffer[2048];
 va_list args;
 va_start(args,msg);
 vsprintf(buffer,msg,args);
 va_end(args);
 DnPrivate_Print(DnThread_GetPrintFunc(),buffer);
}
DEBUG_NEW_FUNC_IMPL(void) DnVPrintf(char const *msg, va_list args) {
 DnPrivate_VPrintf(DnThread_GetPrintFunc(),msg,args);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Print(Dn_PrintFunc_t func, char const *msg) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_PrintAlreadyLocked(func,msg);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Printf(Dn_PrintFunc_t func, char const *msg, ...) {
 va_list args;
 va_start(args,msg);
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_VPrintfAlreadyLocked(func,msg,args);
 DEBUG_NEW_PRINT_LOCK_LEAVE
 va_end(args);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_VPrintf(
 Dn_PrintFunc_t func, char const *msg, va_list args) {
 DEBUG_NEW_PRINT_LOCK_ENTER
 DnPrivate_VPrintfAlreadyLocked(func,msg,args);
 DEBUG_NEW_PRINT_LOCK_LEAVE
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_PrintAlreadyLocked(
 Dn_PrintFunc_t func, char const *msg) {
 DN_ASSERTE(func && "Invalid print function",{return;});
 func(msg);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_PrintfAlreadyLocked(
 Dn_PrintFunc_t func, char const *msg, ...) {
 va_list args;
 char buffer[2048];
 DN_ASSERTE(func && "Invalid print function",{return;});
 va_start(args,msg);
 vsprintf(buffer,msg,args);
 va_end(args);
 func(buffer);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_VPrintfAlreadyLocked(
 Dn_PrintFunc_t func, char const *msg, va_list args) {
 char buffer[2048];
 DN_ASSERTE(func && "Invalid print function",{return;});
 vsprintf(buffer,msg,args);
 func(buffer);
}


//////////////////////////////////////////////////////////////////////////
// Stats
//////////////////////////////////////////////////////////////////////////
#if DEBUG_NEW_CONFIG_HAVE_STATS
DEBUG_NEW_FUNC_IMPL(void) DnStat_Merge(Dn_Stat_t *left, Dn_Stat_t const *right) {
 DN_ASSERTE(left && "Invalid left stats",{return;});
 DN_ASSERTE(right && "Invalid right stats",{return;});
 left->total_calls_malloc       += right->total_calls_malloc;
 left->total_calls_realloc      += right->total_calls_realloc;
 left->total_calls_calloc       += right->total_calls_calloc;
 left->total_calls_free         += right->total_calls_free;
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
 left->total_calls_new          += right->total_calls_new;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
 left->total_calls_array_new    += right->total_calls_array_new;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
 left->total_calls_delete       += right->total_calls_delete;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
 left->total_calls_array_delete += right->total_calls_array_delete;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
 left->total_calls_aligned_malloc  += right->total_calls_aligned_malloc;
 left->total_calls_aligned_calloc  += right->total_calls_aligned_calloc;
 left->total_calls_aligned_realloc += right->total_calls_aligned_realloc;
 left->total_calls_aligned_free    += right->total_calls_aligned_free;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
 left->total_alloc_blocks       += right->total_alloc_blocks;
 left->total_alloc              += right->total_alloc;
 if (right->max_alloc_blocks > left->max_alloc_blocks)
  left->max_alloc_blocks = right->max_alloc_blocks;
 if (right->max_alloc > left->max_alloc)
  left->max_alloc = right->max_alloc;
}
DEBUG_NEW_FUNC_IMPL(void) DnStat_GetGlobal(Dn_Stat_t *stats) {
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DnPrivate_Stat_GetGlobalAlreadyLocked(stats);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Stat_GetGlobalAlreadyLocked(Dn_Stat_t *stats) {
 Dn_Thread_t *iter;
 DN_ASSERTE(stats && "Invalid stats",{return;});
 memset(stats,0,sizeof(Dn_Stat_t)); //< clear the stats
 iter = DnGlobal_Threads;
 while (iter) {
  DnStat_Merge(stats,&iter->t_stats);
  iter = iter->t_prev;
 }
}
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */


//////////////////////////////////////////////////////////////////////////
// Thread methods
//////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable:4619)
# pragma warning(disable:4703)
#endif /* _MSC_VER */
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) 
DnPrivate_Thread_Find(DEBUG_NEW_OS_THREAD_ID_T thread_id) {
 Dn_Thread_t *result;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock); 
 result = DnPrivate_Thread_FindAlreadyLocked(thread_id);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return result;
}
#ifdef _MSC_VER
# pragma warning(pop)
#endif /* _MSC_VER */
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) 
DnPrivate_Thread_FindAlreadyLocked(DEBUG_NEW_OS_THREAD_ID_T thread_id) {
 Dn_Thread_t *iter = DnGlobal_Threads;
 while (iter) {
  if (iter->t_thread_id == thread_id) return iter;
  iter = iter->t_prev;
 }
 return NULL;
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) 
DnPrivate_Thread_GetAlreadyLocked(char const *ac, void const *addr) {
 Dn_Thread_t *result;
 DEBUG_NEW_PRIVATE_SOURCE_CHECK_INIT(NULL);
 DEBUG_NEW_UNREFERENCED(ac);
 DEBUG_NEW_UNREFERENCED(addr);
 DEBUG_NEW_OS_TLS_GET_FAST(DnPrivateGlobal_CurrentThread,result);
 if (!result) { // v allocate a new thread
  if ((result = (Dn_Thread_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(
   sizeof(Dn_Thread_t))) == NULL) return NULL;
  if (!DEBUG_NEW_OS_TLS_SET(DnPrivateGlobal_CurrentThread,result)) {
   DEBUG_NEW_API_NATIVE_FREE_NN(result);
   return NULL;
  }
  DEBUG_NEW_OS_MUTEX_INIT(result->t_lock);
  result->t_thread_id = DEBUG_NEW_OS_THREAD_SELF();
  result->t_next = NULL;
  result->t_allocs = NULL;
  result->t_state = NULL;
  result->t_print = NULL;
#if DEBUG_NEW_CONFIG_HAVE_STATS
  memset(&result->t_stats,0,sizeof(Dn_Stat_t));
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
  if ((result->t_prev = DnGlobal_Threads) != NULL)
   DnGlobal_Threads->t_next = result;
  DnGlobal_Threads = result;
 }
 return result;
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_Thread_t *) DnThread_Get(void) {
 Dn_Thread_t *result;
 DEBUG_NEW_PRIVATE_SOURCE_CHECK_INIT(NULL);
 DEBUG_NEW_OS_TLS_GET_FAST(DnPrivateGlobal_CurrentThread,result);
 return result;
}
static unsigned int DnPrivateSource_State_Pop(
 Dn_State_t **state_ptr, Dn_MemPtr_t *allocs,
 char const *ac, void const *addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
 Dn_Stat_t const *stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
 ptrdiff_t offset_of_prev);
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_Free(Dn_Thread_t *thread, char const *ac, void const *addr) {
 Dn_MemPtr_t *allocs;
 DN_ASSERTE(thread && "Invalid thread",{return;});
 while (thread->t_state) {
  // ** pop all thread states, that remained open
  DnPrivateSource_State_Pop(
   &thread->t_state,thread->t_allocs,ac,addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
   &thread->t_stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
   DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
 }
 while (thread->t_print && DnPrivate_Thread_PopPrintFuncInternalAlreadyLocked(thread,ac,addr));
 allocs = thread->t_allocs;
 if (allocs) while (1) {
  // ** unlink the thread pointers from all 
  //    memory blocks allocated with this thread
  Dn_MemPtr_t *prev = allocs->p_thread_prev;
  allocs->p_thread_next = NULL;
  if (!prev) break;
  allocs->p_thread_prev = NULL;
  allocs = prev;
 }
 DEBUG_NEW_OS_MUTEX_QUIT(thread->t_lock);
 DEBUG_NEW_API_NATIVE_FREE_NN(thread);
}
#define DnPrivateSource_Thread_UnlinkAlreadyLocked(thread)\
do{\
 if ((thread) == DnGlobal_Threads) {\
  if ((DnGlobal_Threads = (thread)->t_prev) != NULL)\
   DnGlobal_Threads->t_next = NULL;\
 } else {\
  /* thread->t_next must be assigned */\
  if ((thread)->t_prev) {\
   (thread)->t_next->t_prev = (thread)->t_prev;\
   (thread)->t_prev->t_next = (thread)->t_next;\
  } else {\
   (thread)->t_next->t_prev = NULL;\
  }\
 }\
}while(0)
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_RemInternal(
 struct Dn_Thread_t *thread, char const *ac, void const *addr) {
 DN_ASSERTE(thread && "Invalid thread",{return;});
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DnPrivateSource_Thread_UnlinkAlreadyLocked(thread);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DnPrivate_Thread_Free(thread,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_RemInternalAlreadyLocked(
 struct Dn_Thread_t *thread, char const *ac, void const *addr) {
 DN_ASSERTE(thread && "Invalid thread",{return;});
 DnPrivateSource_Thread_UnlinkAlreadyLocked(thread);
 DnPrivate_Thread_Free(thread,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(void) DnThread_Rem(char const *ac, void const *addr) {
 Dn_Thread_t *thread;
 DEBUG_NEW_OS_TLS_GET_FAST(DnPrivateGlobal_CurrentThread,thread);
 if (thread) DnPrivate_Thread_RemInternal(thread,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_RemAlreadyLocked(char const *ac, void const *addr) {
 Dn_Thread_t *thread;
 DEBUG_NEW_OS_TLS_GET_FAST(DnPrivateGlobal_CurrentThread,thread);
 if (thread) DnPrivate_Thread_RemInternalAlreadyLocked(thread,ac,addr);
}
#undef DnPrivateSource_Thread_UnlinkAlreadyLocked


//////////////////////////////////////////////////////////////////////////
// Pointer adding / removal
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_PushPointer(Dn_Thread_t *thread, Dn_MemPtr_t *p) {
 DN_ASSERTE(thread && "Invalid thread",{return;});
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 p->p_thread_next = NULL;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock); 
 if (thread->t_allocs) {
  // alloc->next = p;
  // p->prev = alloc;
  (thread->t_allocs->p_thread_next = p)
   ->p_thread_prev = thread->t_allocs;
 } else {
  p->p_thread_prev = NULL;
 }
 thread->t_allocs = p;
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Thread_PopPointer(Dn_MemPtr_t *p, char const *ac, void const *addr) {
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 if (p->p_state.p_thread == DEBUG_NEW_OS_THREAD_SELF()) {
  // ** easy case: we were the ones, that allocated the pointer
  Dn_Thread_t *thread = DnThread_Get();
  DN_ASSERTE(thread && 
   "Dafuq? We allocated this pointer, but our thread died, "
   "even though it was still managing pointers?",{return;});
  DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
  if (p == thread->t_allocs) {
   if ((thread->t_allocs = p->p_thread_prev) != NULL) {
    thread->t_allocs->p_thread_next = NULL;
   } else {
    if (!thread->t_print && !thread->t_state) {
     DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
     DnPrivate_Thread_RemInternal(thread,ac,addr);
     // reason: see "DnThread_PopPrintFunc"
     DEBUG_NEW_OS_TLS_SET(DnPrivateGlobal_CurrentThread,0);
     return;
    }
   }
  } else {
   if (p->p_thread_prev) {
    p->p_thread_prev->p_thread_next = p->p_thread_next;
    if (DN_ASSERT(p->p_thread_next && "p->p_thread_next must be assigned")) {
     p->p_thread_next->p_thread_prev = p->p_thread_prev;
    }
   } else if (DN_ASSERT(p->p_thread_next && "p->p_thread_next must be assigned")) {
    p->p_thread_next->p_thread_prev = NULL;
   }
  } // ^ must be assigned, because the chain symbols the last alloc
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 } else {
  Dn_Thread_t *thread_iter;
  DEBUG_NEW_OS_THREAD_ID_T search_handle;
  // ** pop the pointer from a different thread
  DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
  thread_iter = DnGlobal_Threads;
  search_handle = p->p_state.p_thread;
  while (thread_iter) {
   if (thread_iter->t_thread_id == search_handle) {
    // ** found the original owner thread
    DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread_iter->t_lock);
    // ** pop the memory pointer from its original thread
    if (p == thread_iter->t_allocs) {
     if ((thread_iter->t_allocs = p->p_thread_prev) != NULL) {
      thread_iter->t_allocs->p_thread_next = NULL;
     } else {
      if (!thread_iter->t_print && !thread_iter->t_state) {
       // ** we just found a dead thread!
       //    we might as well delete it ourselves!
       DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread_iter->t_lock);
       DnPrivate_Thread_RemInternalAlreadyLocked(thread_iter,ac,addr);
       DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
       return;
      }
     }
    } else {
     if (p->p_thread_prev) {
      p->p_thread_prev->p_thread_next = p->p_thread_next;
      if (DN_ASSERT(p->p_thread_next && 
       "p->p_thread_next must be assigned, because this isn\'t the last record")) {
       p->p_thread_next->p_thread_prev = p->p_thread_prev;
      }
     } else if (DN_ASSERT(p->p_thread_next && 
      "p->p_thread_next must be assigned, because this isn\'t the last record")) {
      p->p_thread_next->p_thread_prev = NULL;
     }
    }
    DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread_iter->t_lock);
    DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
    return; //< no need, to keep searching...
   }
   thread_iter = thread_iter->t_prev; //< next thread
  }
  // v if we didn't find the original owner thread,
  //   it (shouldn't) really matter!
  //    technically, there shouldn't be any remaining
  //    links between this record and its surrounding
  //    thread records.
  //    Because If we are here, it should mean,
  //    that we are currently unlinking a record
  //    created by a different thread, that is now dead.
  //    When threads are found dead, they should normally
  //    unlink their pointers. But just in case something 
  //    went horribly wrong, lets manually try,
  //    to unlink this pointer from other thread pointers.
  if (p->p_thread_prev) p->p_thread_prev->p_thread_next = p->p_thread_next;
  if (p->p_thread_next) p->p_thread_next->p_thread_prev = p->p_thread_prev;
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 }
}


DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Global_PushPointer(Dn_MemPtr_t *p) {
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
 p->p_global_next = NULL;
 if (DnGlobal_AllocChain) {
  (DnGlobal_AllocChain->p_global_next = p)
   ->p_global_prev = DnGlobal_AllocChain;
 } else {
  p->p_global_prev = NULL;
 }
 DnGlobal_AllocChain = p;
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_Global_PopPointer(Dn_MemPtr_t *p) {
 DN_ASSERTE(p && "Invalid pointer record",{return;});
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_AllocChainLock); 
 if (p == DnGlobal_AllocChain) {
  if ((DnGlobal_AllocChain = p->p_global_prev) != NULL)
   DnGlobal_AllocChain->p_global_next = NULL;
 } else {
  if (p->p_global_prev) {
   p->p_global_prev->p_global_next = p->p_global_next;
   p->p_global_next->p_global_prev = p->p_global_prev;
  } else {
   p->p_global_next->p_global_prev = NULL;
  }
 } // ^ must be assigned, because the chain symbols the last alloc
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_AllocChainLock);
 //p->p_global_next = NULL;
 //p->p_global_prev = NULL;
}



//////////////////////////////////////////////////////////////////////////
// Leak fixing
//////////////////////////////////////////////////////////////////////////
// ac: access used for popping the associated thread state
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH()
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28159)
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_FixMemoryLeak(Dn_MemPtr_t *ptr, char const *ac, void const *addr) {
 if (!ptr) return;
 // v be careful, to use the right access mode for freeing the pointer
 switch (ptr->p_state.p_type) {
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW) && \
    defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE)
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW:
   DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE,ptr->p_state.p_self,ac,addr);
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW && DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW) && \
    defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE)
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW:
   DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE,ptr->p_state.p_self,ac,addr);
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW && DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
  case DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC:
   DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,ptr->p_state.p_self,ac,addr);
   break;
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_REALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_CALLOC:
   DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE,ptr->p_state.p_self,ac,addr);
   break;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
  default: break;
 }
}
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()







//////////////////////////////////////////////////////////////////////////
// Thread state push / pop
//////////////////////////////////////////////////////////////////////////
#define DEBUG_NEW_THREAD_STATE_ITEM_SIZE (sizeof((DEBUG_NEW_REINTERPRET_CAST(Dn_State_t *)(0))->s_pointer_states))
#define DEBUG_NEW_THREAD_STATE_BASE_SIZE (sizeof(Dn_State_t)-DEBUG_NEW_THREAD_STATE_ITEM_SIZE)
static int DnPrivateSource_State_Push(
 Dn_State_t **state_ptr, Dn_MemPtr_t *allocs,
 char const *ac, void const *addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
 Dn_Stat_t const *stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
 ptrdiff_t offset_of_prev) {
 Dn_State_t *state;
 DN_ASSERTE(state_ptr && "Invalid state-pointer",{return 0;});
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
 ) && "Invalid offset_of_prev",{return 0;});
 if (allocs) {
  size_t idx,npointers = 0;
  Dn_MemPtr_t *p = allocs;
  while (p) {
   ++npointers;
   p = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
    /*!tinyc!DEBUG_NEW_REINTERPRET_CAST*/((char *)p)+offset_of_prev);
  }
  if ((state = (Dn_State_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(
   DEBUG_NEW_THREAD_STATE_BASE_SIZE+DEBUG_NEW_THREAD_STATE_ITEM_SIZE*npointers)
   ) == NULL) return 0;
  state->s_npointer_states = npointers;
  p = allocs;
  idx = 0;
  while (p) {
   DN_ASSERTE(idx < npointers && "Invalid state pointer count",{break;});
   // v import the pointer state
   state->s_pointer_states[idx++] = p->p_state;
   p = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
    DEBUG_NEW_REINTERPRET_CAST(char *)(p)+offset_of_prev);
  }
 } else {
  // v allocate an empty state
  if ((state = (Dn_State_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(
   DEBUG_NEW_THREAD_STATE_BASE_SIZE)) == NULL) return 0;
  state->s_npointer_states = 0; 
 }
#if DEBUG_NEW_CONFIG_HAVE_STATS
 if (stats) {
  state->s_stats = *stats;
 } else {
  memset(&state->s_stats,0,sizeof(Dn_Stat_t));
 }
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS */
 state->s_access      = ac;
 state->s_access_addr = addr;
 state->s_prev        = *state_ptr;
 *state_ptr           = state;
 return 1;
}
#undef DEBUG_NEW_THREAD_STATE_BASE_SIZE
#undef DEBUG_NEW_THREAD_STATE_ITEM_SIZE

#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
#define DnState_Equal(l,r) ((l).p_alloc_id == (r).p_alloc_id)
#else /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#define DnState_Equal(l,r) (\
 (l).p_self == (r).p_self &&\
 (l).p_address == (r).p_address &&\
 (l).p_size == (r).p_size &&\
 (l).p_type == (r).p_type )
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */


static unsigned int DnPrivateSource_State_Pop(
 Dn_State_t **state_ptr, Dn_MemPtr_t *allocs,
 char const *ac, void const *addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
 Dn_Stat_t const *stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
 ptrdiff_t offset_of_prev) {
 DN_ASSERTE(state_ptr && "Invalid state-pointer",{return 0;});
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
 ) && "Invalid offset_of_prev",{return 0;});
 {
  unsigned int result = 0;
  Dn_State_t *state = *state_ptr;
  if (!state) {
   DnPrivate_HandleNoPopState(ac,addr,
    offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
  } else {
#if DEBUG_NEW_CONFIG_HAVE_STATS
   if (stats && (
       state->s_stats.total_alloc        != stats->total_alloc ||
       state->s_stats.total_alloc_blocks != stats->total_alloc_blocks
      )) {
    // ** something definitely went wrong!
    result = DnPrivate_Dump_StateErrors(ac,addr,allocs,state,offset_of_prev);
   } else 
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
   {
    // ** manually search and compare states
    //    NOTE: do this even if stats are enabled an they
    //          don't display any differences.
    //          Because freeing and allocating memory
    //          from outside the current block is still
    //          not allowed, even if the amount added by
    //          is the same, as originally allocated!
    size_t idx = 0; 
    Dn_MemPtr_t *ptr = allocs;
    if (ptr) {
     do {
      if (idx > state->s_npointer_states ||
          !DnState_Equal(state->s_pointer_states[idx],ptr->p_state)) {
       result = DnPrivate_Dump_StateErrors(ac,addr,allocs,state,offset_of_prev); 
       break; // ^ gotcha
      }
      ++idx;
      ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
       DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
     } while (ptr);
    } else {
     if (state->s_npointer_states != 0) {
      result = DnPrivate_Dump_StateErrors(ac,addr,allocs,state,offset_of_prev); 
      // ^ gotcha!
     }
    }
   }
   *state_ptr = state->s_prev;
   DEBUG_NEW_API_NATIVE_FREE_NN(state);
  }
  return result;
 }
}



DEBUG_NEW_FUNC_IMPL(int) DnState_PushThread(char const *ac) {
 return DnPrivate_State_PushThread(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(int) DnPrivate_State_PushThread(char const *ac, void const *addr) {
 int result;
 Dn_Thread_t *thread;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 if ((thread = DnPrivate_Thread_GetAlreadyLocked(ac,addr)) == NULL) {
  result = 0; /*< out-of-memory */
 } else {
  DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
  result = DnPrivateSource_State_Push(&thread->t_state,thread->t_allocs,ac,addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
                         &thread->t_stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
                         DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 }
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return result;
}
DEBUG_NEW_FUNC_IMPL(unsigned int) DnState_PopThread(char const *ac) {
 return DnPrivate_State_PopThread(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_State_PopThread(char const *ac, void const *addr) {
 unsigned int result;
 Dn_Thread_t *thread = DnThread_Get();
 if (!thread) { DnPrivate_HandleNoPopState(ac,addr,0); return 0; }
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(thread->t_lock);
 result = DnPrivateSource_State_Pop(&thread->t_state,thread->t_allocs,ac,addr,
#if DEBUG_NEW_CONFIG_HAVE_STATS
                        &thread->t_stats,
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
                       DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(thread->t_lock);
 if (!thread->t_print && !thread->t_state && !thread->t_allocs) {
  DnPrivate_Thread_RemInternalAlreadyLocked(thread,ac,addr);
  // reason: see "DnThread_PopPrintFunc"
  DEBUG_NEW_OS_TLS_SET(DnPrivateGlobal_CurrentThread,0);
 }
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 return result;
}


DEBUG_NEW_FUNC_IMPL(int) DnState_PushGlobal(char const *ac) {
 return DnPrivate_State_PushGlobal(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(int) DnPrivate_State_PushGlobal(char const *ac, void const *addr) {
 int result;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_GlobalStateLock);
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
#if DEBUG_NEW_CONFIG_HAVE_STATS
 {Dn_Stat_t stats;
  DnPrivate_Stat_GetGlobalAlreadyLocked(&stats);
  result = DnPrivateSource_State_Push(
   &DnGlobal_GlobalState,DnGlobal_AllocChain,
   ac,addr,&stats,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
 }
#else /* DEBUG_NEW_CONFIG_HAVE_STATS */
 result = DnPrivateSource_State_Push(
  &DnGlobal_GlobalState,DnGlobal_AllocChain,
  ac,addr,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS */
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_GlobalStateLock);
 return result;
}
DEBUG_NEW_FUNC_IMPL(unsigned int) DnState_PopGlobal(char const *ac) {
 return DnPrivate_State_PopGlobal(ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_State_PopGlobal(char const *ac, void const *addr) {
 unsigned int result;
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_GlobalStateLock);
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
#if DEBUG_NEW_CONFIG_HAVE_STATS
 {Dn_Stat_t stats;
  DnPrivate_Stat_GetGlobalAlreadyLocked(&stats);
  result = DnPrivateSource_State_Pop(
   &DnGlobal_GlobalState,DnGlobal_AllocChain,ac,addr,
   &stats,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
 }
#else /* DEBUG_NEW_CONFIG_HAVE_STATS */
 result = DnPrivateSource_State_Pop(
  &DnGlobal_GlobalState,DnGlobal_AllocChain,ac,addr,
  DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
#endif /* !DEBUG_NEW_CONFIG_HAVE_STATS */
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_GlobalStateLock);
 return result;
}



//////////////////////////////////////////////////////////////////////////
// State leak / free dumping and fixing (if specified) 
// NOTE: ac: access used for popping the associated state
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_Dump_ThreadStateErrors(char const *ac, void const *addr, Dn_MemPtr_t *allocs, Dn_State_t const *state) {
 return DnPrivate_Dump_StateErrors(ac,addr,allocs,state,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev));
}
DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_Dump_GlobalStateErrors(char const *ac, void const *addr, Dn_MemPtr_t *allocs, Dn_State_t const *state) {
 return DnPrivate_Dump_StateErrors(ac,addr,allocs,state,DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev));
}

DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_Pointer_FindFutureState(Dn_MemPtr_t const *left, Dn_StatePtr_t const *right, ptrdiff_t offset_of_prev) {
 unsigned int result;
 if (!left) return 0;
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
 ) && "Invalid offset_of_prev",{return 0;});
 DN_ASSERTE(right && "Invalid state-pointer",{return 0;});
 DN_ASSERTE(!DnState_Equal(left->p_state,*right) && 
            "initial left and right may not be equal",{return 0;});
 result = 1;
 while (1) {
  if ((left = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t *const*)(
   DEBUG_NEW_REINTERPRET_CAST(char const *)(left)+offset_of_prev)) == NULL) break;
  if (DnState_Equal(left->p_state,*right)) return result;
  ++result;
 }
 return 0;
}

DEBUG_NEW_FUNC_IMPL(unsigned int) DnPrivate_Dump_StateErrors(
 char const *ac, void const *addr,
 Dn_MemPtr_t *allocs,
 Dn_State_t const *state,
 ptrdiff_t offset_of_prev) {
 Dn_PrintFunc_t print_func = DnThread_GetPrintFunc();
 size_t idx = 0,total_leaks = 0,total_frees = 0;
 Dn_MemPtr_t *ptr = allocs;
 unsigned int n_leaks = 0,n_frees = 0,future_offset;
 int first_msg = 1;
 DN_ASSERTE((
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_thread_prev)) ||
  (offset_of_prev == DEBUG_NEW_PRIVATE_OFFSETOF(Dn_MemPtr_t,p_global_prev))
 ) && "Invalid offset_of_prev",{return 0;});
 DEBUG_NEW_PRINT_LOCK_ENTER
 while (1) {
  if (idx >= state->s_npointer_states) break;
  if (!ptr) break;
  if (!DnState_Equal(state->s_pointer_states[idx],ptr->p_state)) {
   // ** incoherence found (a pointer got freed, that was allocated outside this state)
   if (first_msg) { DnPrivate_Print_LeaksFirstMessageAlreadyLocked(print_func,state,ac,addr); first_msg = 0; }
   future_offset = DnPrivate_Pointer_FindFutureState(ptr,&state->s_pointer_states[idx],offset_of_prev);
   if (future_offset) {
    // ** the current state pointer can be found in the future of "ptr"
    //    --> until that future is met, every "ptr"
    //        is a leak and handled as su.h"
    while (1) {
     DnPrivate_Dump_InvalidMemoryLeakAlreadyLocked(print_func,&ptr->p_state);
     total_leaks += ptr->p_state.p_size;
     ++n_leaks;
#if DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
     {
      Dn_MemPtr_t *next = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
       DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
      DnPrivate_FixMemoryLeak(ptr,ac,addr); //< use the access given
      ptr = next;
     }
#else /* DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */
     ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
      DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
#endif /* !DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */
     if (!--future_offset) break;
    }
   } else {
    // ** the current state pointer cannot be found in the future of "ptr"
    //    --> "state->s_pointer_states[idx]" is a invalid free
    DnPrivate_Dump_InvalidMemoryFreeAlreadyLocked(print_func,&state->s_pointer_states[idx]);
    total_frees += state->s_pointer_states[idx].p_size;
    ++n_frees;
    ++idx;
   }
  } else {
   // ** normal behavior: state and ptr match each other
   //    --> continue normally
   ++idx;
   ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
    DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
  }
 }
 // v anything, that is still remaining is a leak
 while (ptr) {
  if (first_msg) { DnPrivate_Print_LeaksFirstMessageAlreadyLocked(print_func,state,ac,addr); first_msg = 0; }
  DnPrivate_Dump_InvalidMemoryLeakAlreadyLocked(print_func,&ptr->p_state);
  total_leaks += ptr->p_state.p_size;
  ++n_leaks;
#if DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
  {
   Dn_MemPtr_t *next = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
    DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
   DnPrivate_FixMemoryLeak(ptr,ac,addr); //< use the access given
   ptr = next;
  }
#else /* DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */
  ptr = *DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t **)(
   DEBUG_NEW_REINTERPRET_CAST(char *)(ptr)+offset_of_prev);
#endif /* !DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */
 }
 // v dump any remaining states, that got freed
 while (idx < state->s_npointer_states) {
  if (first_msg) { DnPrivate_Print_LeaksFirstMessageAlreadyLocked(print_func,state,ac,addr); first_msg = 0; }
  DnPrivate_Dump_InvalidMemoryFreeAlreadyLocked(print_func,&state->s_pointer_states[idx++]);
  total_frees += state->s_pointer_states[idx++].p_size;
  ++n_frees;
 }
 // v did anything happen at all?
 if (!first_msg) {
  // v dump combined stats
  DnPrivate_Print_LeakStatisticsAlreadyLocked(
   print_func,ac,addr,
   n_leaks,total_leaks,
   n_frees,total_frees);
 }
 DEBUG_NEW_PRINT_LOCK_LEAVE
 return n_leaks+n_frees; //< return total amount of inconsistencies
}

#undef DnState_Equal


//////////////////////////////////////////////////////////////////////////
// memory processing methods 
// (only used, to add file, line and func information to new and new[])
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_ProcessCXXPointer(void *ptr, char const *ac, void const *addr) {
 Dn_MemPtr_t *rec;
 if (!ptr) return;
 rec = DnPrivate_GetRecord(ptr,ac,addr);
 // v - The record isn't assigned (definitely not a pointer allocated with debug_new),
 //   - Magic is not initialized (should be from the call to operator new)
 //   - The access mode is assigned (is shouldn't be, because operator new can't do that)
 //   --> this should have been a placement new call!
 //   NOTES:
 //    - because we are accessing random memory, an ACCESS_VIOLATION
 //      might occurred within the "memcmp" call, if the pointer given
 //      point to the start of a memory-block allocated with a different
 //      allocator (If that is the case, this should have been a "placement new"call).
 //    - since these 2 conditions, could be achieved through random
 //      memory placement or because be are matching some random
 //      pattern used elsewhere, we can't always say for certain,
 //      when it should have been a "placement new" call
 //      --> That is why I implemented the "placement" keyword!
 if (!rec || memcmp(&rec->p_magic,&Dn_Magic,sizeof(Dn_Magic)) != 0 ||
     rec->p_state.p_access != NULL) {
  DnPrivate_HandleShouldBePlacementCall(ptr,ac,addr);
 } else {
  rec->p_state.p_access = ac;
  rec->p_state.p_address = addr;
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
  rec->p_state.p_alloc_info = NULL;
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
 }
}








//////////////////////////////////////////////////////////////////////////
// debug allocation / deallocation methods
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_PUSH()
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_DISABLE(28159)
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *)
DnDebug_Malloc(Dn_AllocType_t mode, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_Malloc(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *)
DnDebug_MallocNZ(Dn_AllocType_t mode, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_MallocNZ(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *)
DnPrivate_Debug_MallocNZ(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr) {
 if (!s) { DnPrivate_HandleNotXXXViolation("malloc_nz(0)","malloc_nz","malloc","0",ac,addr); return NULL; }
 return DnPrivate_Debug_Malloc(mode,s,ac,addr);
}
DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_WARNINGS_POP()
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *)
DnPrivate_Debug_Malloc(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr) {
 size_t s2;
 Dn_MemPtr_t *ptr;
 Dn_Thread_t *thread;
 // v "malloc(0)" returns "0"
 //   NOTE: this also implements:
 //          - "realloc(p,0)" 
 //          - "calloc(x,y) | x*y == 0"
 if (!s) return NULL;
#if DEBUG_NEW_CONFIG_TAILCHECK_SIZE
 s2 = s+DEBUG_NEW_PRIVATE_ALIGNED_POINTER_SIZE+DEBUG_NEW_CONFIG_TAILCHECK_SIZE;
#else /* DEBUG_NEW_CONFIG_TAILCHECK_SIZE */
 s2 = s+DEBUG_NEW_PRIVATE_ALIGNED_POINTER_SIZE;
#endif // !DEBUG_NEW_CONFIG_TAILCHECK_SIZE
 DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME(s2 != 0);
 if (s2 < s || !DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC(s2)) {
  // if "s2" is lower than "s", we got an integer overflow.
  // In that case, this is also too big of an allocation.
#if DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
  DnPrivate_HandleTooBigAlloc(s,ac,addr);
#else // DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
  DnPrivate_HandleOutOfMemory(s,ac,addr);
#endif // !DEBUG_NEW_CONFIG_HAVE_TOO_BIG_ALLOC
  return NULL;
 }
 if ((ptr = (Dn_MemPtr_t *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(s2)) == NULL) {
  DnPrivate_HandleOutOfMemory(s,ac,addr);
  return NULL;
 }
 memcpy(&ptr->p_magic,&Dn_Magic,sizeof(Dn_Magic));
 DEBUG_NEW_OS_MUTEX_ENTER_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
 if ((thread = DnPrivate_Thread_GetAlreadyLocked(ac,addr)) == NULL) {
  // ** out-of-memory 
  //    (already handled by "DnThread_Get")
  DEBUG_NEW_API_NATIVE_FREE_NN(ptr);
  DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
  return NULL;
 }
 ptr->p_state.p_thread = thread->t_thread_id;
 ptr->p_state.p_access = ac;
 ptr->p_state.p_address = addr;
 ptr->p_state.p_self = DEBUG_NEW_STATIC_CAST(void *)(DEBUG_NEW_REINTERPRET_CAST(
  char *)(ptr)+DEBUG_NEW_PRIVATE_ALIGNED_POINTER_SIZE);
 ptr->p_state.p_type = mode;
 ptr->p_state.p_size = s;
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
 ptr->p_state.p_alloc_id = DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_GET_NEXT();
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOC_ID */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
 ptr->p_state.p_alloc_info = NULL;
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
 DnPrivate_Global_PushPointer(ptr); //< push it into the thread list
 DnPrivate_Thread_PushPointer(thread,ptr); //< push it into the global list
#if DEBUG_NEW_CONFIG_HAVE_STATS
 switch (mode) {
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW:       ++thread->t_stats.total_calls_new;       break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW: ++thread->t_stats.total_calls_array_new; break;
#endif // DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW
  case DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC:    ++thread->t_stats.total_calls_malloc;  break;
  case DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC:   ++thread->t_stats.total_calls_realloc; break;
  case DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC:    ++thread->t_stats.total_calls_calloc;  break;
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC:  ++thread->t_stats.total_calls_aligned_malloc;  break;
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_REALLOC: ++thread->t_stats.total_calls_aligned_realloc; break;
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_CALLOC:  ++thread->t_stats.total_calls_aligned_calloc;  break;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
 }
 if (thread->t_stats.max_alloc_blocks < ++thread->t_stats.total_alloc_blocks)
  thread->t_stats.max_alloc_blocks = thread->t_stats.total_alloc_blocks;
 if (thread->t_stats.max_alloc < (thread->t_stats.total_alloc += s))
  thread->t_stats.max_alloc = thread->t_stats.total_alloc;
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
 DEBUG_NEW_OS_MUTEX_LEAVE_NOEXCEPT(DnPrivateGlobal_ThreadsLock);
#if DEBUG_NEW_CONFIG_TAILCHECK_SIZE
 memset(DEBUG_NEW_STATIC_CAST(char *)(
  ptr->p_state.p_self)+s,DEBUG_NEW_CONFIG_TAILCHECK_CHAR,DEBUG_NEW_CONFIG_TAILCHECK_SIZE);
#endif // !DEBUG_NEW_CONFIG_TAILCHECK_SIZE
 return ptr->p_state.p_self;
}

DEBUG_NEW_FUNC_IMPL(void) DnPrivate_FreeNoCheck(
 Dn_AllocType_t mode, Dn_MemPtr_t *rec, char const *ac) {
 DnPrivate_FreeNoCheck2(mode,rec,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(void) DnPrivate_FreeNoCheck2(
 Dn_AllocType_t mode, Dn_MemPtr_t *rec, char const *ac, void const *addr) {
 Dn_Thread_t *thread;
 DN_ASSERTE(rec && "Invalid pointer record",{return;});
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID &&\
    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
 if (DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_AllocationBreakpointIsSet &&
     rec->p_state.p_alloc_id == DEBUG_NEW_CXX_NAMESPACE_ROOT DnPrivateGlobal_AllocationBreakpoint
     ) DEBUG_NEW_BREAKPOINT();
#endif
#if DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE
 // v clear-on-free
 memset(rec->p_state.p_self,0,rec->p_state.p_size);
#endif /* DEBUG_NEW_CONFIG_HAVE_CLEAR_ON_FREE */
 DnPrivate_Global_PopPointer(rec); //< pop it from the thread list
 DnPrivate_Thread_PopPointer(rec,ac,addr); //< pop it from the global list
#if DEBUG_NEW_CONFIG_HAVE_STATS
 // ** update stats
 if ((thread = DnThread_Get()) != NULL) {
  // ^ the thread might not be assigned,
  //   if we are current freeing a pointer allocated
  //   by a different thread
  switch (mode) {
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE
   case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE:
    ++thread->t_stats.total_calls_delete;
    break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE
   case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE:
    ++thread->t_stats.total_calls_array_delete;
    break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#ifdef DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE
   case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE:
    ++thread->t_stats.total_calls_aligned_free;
    break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE */
   case DEBUG_NEW_ALLOC_CALL_TYPE_FREE:
    ++thread->t_stats.total_calls_free;
    break;
   default: break;
  }
  if (thread->t_stats.total_alloc_blocks)
   --thread->t_stats.total_alloc_blocks;
  thread->t_stats.total_alloc -= rec->p_state.p_size;
 }
#endif /* DEBUG_NEW_CONFIG_HAVE_STATS */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
 DEBUG_NEW_API_NATIVE_FREE(rec->p_state.p_alloc_info);
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
 DEBUG_NEW_API_NATIVE_FREE_NN(rec); /*< call the native free method */
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_FREE(p) void) DnDebug_Free(
 Dn_AllocType_t mode, void *p, char const *ac) {
 DnPrivate_Debug_Free(mode,p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_FREE_NN(p) void) DnDebug_FreeNN(
 Dn_AllocType_t mode, void *p, char const *ac) {
 DnPrivate_Debug_FreeNN(mode,p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_FREE_NN(p) void) DnPrivate_Debug_FreeNN(
 Dn_AllocType_t mode, void *p, char const *ac, void const *addr) {
 if (!p) { DnPrivate_HandleNotXXXViolation("free_nn(0)","free_nn","free","NULL",ac,addr); return; }
 DnPrivate_Debug_Free(mode,p,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_FREE(p) void) DnPrivate_Debug_Free(
 Dn_AllocType_t mode, void *p, char const *ac, void const *addr) {
 Dn_MemPtr_t *rec = DnPrivate_GetRecord(p,ac,addr);
 if (!DnPrivate_CheckRecordInternal(rec,ac,addr)) {
#if !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL
  // v tried to free 0-pointer
  //   (although allowed, this should not be
  //    practiced in my opinion)
  if (!p) DnPrivate_HandleFreeNullPointer(ac,addr);
#endif /* !DEBUG_NEW_CONFIG_HAVE_ALLOW_FREE_NULL */
  return; //< invalid pointer! -- bail
 }
 // ** check for correctness in deallocation mode
 switch (rec->p_state.p_type) {
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW) && \
    defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE)
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW:
   if (mode != DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE) 
    DnPrivate_HandleInvalidDeallocKind(ac,addr,rec,DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE,mode);
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_NEW && DEBUG_NEW_ALLOC_CALL_TYPE_CXX_DELETE */
#if defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW) && \
    defined(DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE)
  case DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW:
   if (mode != DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE) 
    DnPrivate_HandleInvalidDeallocKind(ac,addr,rec,DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE,mode);
   break;
#endif /* DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_NEW && DEBUG_NEW_ALLOC_CALL_TYPE_CXX_ARRAY_DELETE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_REALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_CALLOC:
   if (mode != DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE)
    DnPrivate_HandleInvalidDeallocKind(ac,addr,rec,DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE,mode);
   break;
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
  case DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC:
  case DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC:
   if (mode != DEBUG_NEW_ALLOC_CALL_TYPE_FREE) 
    DnPrivate_HandleInvalidDeallocKind(ac,addr,rec,DEBUG_NEW_ALLOC_CALL_TYPE_FREE,mode);
   break;
  default: break;
 }
 DnPrivate_FreeNoCheck2(mode,rec,ac,addr);
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *)
DnDebug_Realloc(void *p, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_Realloc(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *)
DnDebug_ReallocNZ(void *p, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_ReallocNZ(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *)
DnDebug_ReallocNN(void *p, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_ReallocNN(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *)
DnDebug_ReallocNNZ(void *p, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_ReallocNNZ(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *)
DnPrivate_Debug_ReallocNNZ(void *p, size_t s, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("realloc_nnz(NULL,...)","realloc_nnz","realloc_nz","NULL",ac,addr); return DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,ac,addr); }
 if (s == 0)    { DnPrivate_HandleNotXXXViolation("realloc_nnz(...,0)","realloc_nnz","realloc_nn","ZERO",ac,addr); DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,p,ac,addr); return NULL; }
 return DnPrivate_Debug_Realloc(p,s,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *)
DnPrivate_Debug_ReallocNN(void *p, size_t s, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("realloc_nn(NULL,...)","realloc_nn","realloc","NULL",ac,addr); return DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,s,ac,addr); }
 return DnPrivate_Debug_Realloc(p,s,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *)
DnPrivate_Debug_ReallocNZ(void *p, size_t s, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("realloc_nz(...,0)","realloc_nz","realloc","ZERO",ac,addr); DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,p,ac,addr); return NULL; }
 return DnPrivate_Debug_Realloc(p,s,ac,addr);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *)
DnPrivate_Debug_Realloc(void *p, size_t s, char const *ac, void const *addr) {
 Dn_MemPtr_t *rec;
 void *result;
 if (!s) {
  DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,p,ac,addr);
  return NULL;
 }
 rec = DnPrivate_GetRecord(p,ac,addr);
 if (!DnPrivate_CheckRecordInternal(rec,ac,addr)) {
#if DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL
  // v "realloc(NULL,x)" just calls "malloc(x)"
  if (!p) return DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC,s,ac,addr);
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOW_REALLOC_NULL */
  return NULL; //< this is an invalid pointer! better bail
 }
 DEBUG_NEW_COMPILER_MSVC_CODE_ANALYSIS_ASSUME(p != NULL);
 // ** I could have implemented something fancy here, that
 //    actually calls "realloc", but I'm too lazy right now.
 //    maybe in the future...
 result = DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_REALLOC,s,ac,addr);
 if (result) {
  //v copy memory into the new space
  memcpy(result,p,s < rec->p_state.p_size ? s : rec->p_state.p_size);
  DnPrivate_FreeNoCheck2(DEBUG_NEW_ALLOC_CALL_TYPE_FREE,rec,ac,addr); //< free the old pointer
 }
 return result; //< return the new space
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *)
DnDebug_Calloc(size_t n, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_Calloc(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *)
DnDebug_CallocNZ(size_t n, size_t s, char const *ac) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_CallocNZ(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *)
DnPrivate_Debug_Calloc(size_t n, size_t s, char const *ac, void const *addr) {
 void *ptr = DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC,(s*=n),ac,addr);
 return ptr ? memset(ptr,0,s) : ptr;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *)
DnPrivate_Debug_CallocNZ(size_t n, size_t s, char const *ac, void const *addr) {
 void *ptr;
 if ((s*=n) == 0) { DnPrivate_HandleNotXXXViolation("calloc_nz(n,s) | n*s == 0","calloc_nz","calloc","ZERO",ac,addr); return NULL; }
 ptr = DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_CALLOC,s,ac,addr);
 return ptr ? memset(ptr,0,s) : ptr;
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t)
DnDebug_MallocMSize(void *p, char const *ac) {
 return DnPrivate_Debug_MallocMSize(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t)
DnDebug_MallocMSizeNN(void *p, char const *ac) {
 return DnPrivate_Debug_MallocMSizeNN(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t)
DnDebug_MallocMSizeC(void const *p, char const *ac) {
 return DnPrivate_Debug_MallocMSize(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t)
DnDebug_MallocMSizeCNN(void const *p, char const *ac) {
 return DnPrivate_Debug_MallocMSizeNN(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE(p) size_t)
DnPrivate_Debug_MallocMSize(void const *p, char const *ac, void const *addr) {
 Dn_MemPtr_t *rec = DnPrivate_GetRecord(DEBUG_NEW_CONST_CAST(void *)(p),ac,addr);
 return DnPrivate_CheckRecordInternal(rec,ac,addr) ? rec->p_state.p_size : 0;
}
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28196)
#endif
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MSIZE_NN(p) size_t)
DnPrivate_Debug_MallocMSizeNN(void const *p, char const *ac, void const *addr) {
 Dn_MemPtr_t *rec;
 if (p == NULL) {
  DnPrivate_HandleNotXXXViolation("malloc_msize_nn(NULL)","malloc_msize_nn","malloc_msize","NULL",ac,addr);
  return 0;
 }
 rec = DnPrivate_GetRecord(DEBUG_NEW_CONST_CAST(void *)(p),ac,addr);
 return DnPrivate_CheckRecordInternal(rec,ac,addr) ? rec->p_state.p_size : 0;
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif


#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID &&\
    DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT
DEBUG_NEW_FUNC_IMPL(void) DnGetAllocBreakpointSet(size_t id) {
 DnPrivateGlobal_AllocationBreakpointIsSet = 1;
 DnPrivateGlobal_AllocationBreakpoint = id;
}
DEBUG_NEW_FUNC_IMPL(void) DnGetAllocBreakpointClear(void) {
 DnPrivateGlobal_AllocationBreakpointIsSet = 0;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t) DnGetAllocID(void) {
 return DnPrivateGlobal_AllocationBreakpoint;
}
#endif /* DEBUG_NEW_CONFIG_HAVE_ALLOC_ID && DEBUG_NEW_CONFIG_HAVE_ALLOC_ID_BREAKPOINT */


#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *) DnDebug_AlignedMalloc(size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedMalloc(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *) DnDebug_AlignedMallocNZ(size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedMallocNZ(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *) DnDebug_AlignedOffsetMalloc(size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetMalloc(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetMallocNZ(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *) DnDebug_AlignedCalloc(size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedCalloc(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *) DnDebug_AlignedCallocNZ(size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedCallocNZ(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *) DnDebug_AlignedOffsetCalloc(size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetCalloc(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *) DnDebug_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetCallocNZ(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *) DnDebug_AlignedRealloc(void *p, size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedRealloc(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *) DnDebug_AlignedReallocNZ(void *p, size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedReallocNZ(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *) DnDebug_AlignedReallocNN(void *p, size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedReallocNN(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *) DnDebug_AlignedReallocNNZ(void *p, size_t s, size_t a, char const *ac) { return DnPrivate_Debug_AlignedReallocNNZ(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *) DnDebug_AlignedOffsetRealloc(void *p, size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetRealloc(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNZ(void *p, size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetReallocNZ(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNN(void *p, size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetReallocNN(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *) DnDebug_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetReallocNNZ(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p) void) DnDebug_AlignedFree(void *p, char const *ac) { DnPrivate_Debug_AlignedFree(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) void) DnDebug_AlignedFreeNN(void *p, char const *ac) { DnPrivate_Debug_AlignedFreeNN(p,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a) size_t) DnDebug_AlignedMallocMSize(void const *p, size_t a, char const *ac) { return DnPrivate_Debug_AlignedMallocMSize(p,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) size_t) DnDebug_AlignedMallocMSizeNN(void const *p, size_t a, char const *ac) { return DnPrivate_Debug_AlignedMallocMSizeNN(p,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o) size_t) DnDebug_AlignedOffsetMallocMSize(void const *p, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetMallocMSize(p,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) size_t) DnDebug_AlignedOffsetMallocMSizeNN(void const *p, size_t a, size_t o, char const *ac) { return DnPrivate_Debug_AlignedOffsetMallocMSizeNN(p,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS()); }

DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *)
DnPrivate_Debug_AlignedMallocNZ(size_t s, size_t a, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_malloc_nz(0,...)","aligned_malloc_nz","aligned_malloc","ZERO",ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedMalloc(s,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *)
DnPrivate_Debug_AlignedOffsetMallocNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_offset_malloc_nz(0,...)","aligned_offset_malloc_nz","aligned_offset_malloc","ZERO",ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedOffsetMalloc(s,a,o,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *)
DnPrivate_Debug_AlignedCallocNZ(size_t s, size_t a, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_calloc_nz(0,...)","aligned_calloc_nz","aligned_calloc","ZERO",ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedCalloc(s,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *)
DnPrivate_Debug_AlignedOffsetCallocNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_offset_calloc_nz(0,...)","aligned_offset_calloc_nz","aligned_offset_calloc","ZERO",ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedOffsetCalloc(s,a,o,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *)
DnPrivate_Debug_AlignedReallocNZ(void *p, size_t s, size_t a, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_realloc_nz(?,0,...)","aligned_realloc_nz","aligned_realloc","ZERO",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedRealloc(p,s,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *)
DnPrivate_Debug_AlignedReallocNN(void *p, size_t s, size_t a, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_realloc_nn(?,0,...)","aligned_realloc_nn","aligned_realloc","NULL",ac,addr); return DnPrivate_Debug_AlignedMalloc(s,a,ac,addr); }
 return DnPrivate_Debug_AlignedRealloc(p,s,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *)
DnPrivate_Debug_AlignedReallocNNZ(void *p, size_t s, size_t a, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_realloc_nnz(NULL,...)","aligned_realloc_nnz","aligned_realloc_nz","NULL",ac,addr); return DnPrivate_Debug_AlignedMallocNZ(s,a,ac,addr); }
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_realloc_nnz(?,0,...)","aligned_realloc_nnz","aligned_realloc_nn","ZERO",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedRealloc(p,s,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *)
DnPrivate_Debug_AlignedOffsetReallocNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_offset_realloc_nz(?,0,...)","aligned_offset_realloc_nz","aligned_offset_realloc","ZERO",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedOffsetRealloc(p,s,a,o,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *)
DnPrivate_Debug_AlignedOffsetReallocNN(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_offset_realloc_nn(NULL,...)","aligned_offset_realloc_nn","aligned_offset_realloc","NULL",ac,addr); return DnPrivate_Debug_AlignedMalloc(s,a,ac,addr); }
 return DnPrivate_Debug_AlignedOffsetRealloc(p,s,a,o,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *)
DnPrivate_Debug_AlignedOffsetReallocNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_offset_realloc_nnz(NULL,...)","aligned_offset_realloc_nnz","aligned_offset_realloc_nz","NULL",ac,addr); return DnPrivate_Debug_AlignedMallocNZ(s,a,ac,addr); }
 if (s == 0) { DnPrivate_HandleNotXXXViolation("aligned_offset_realloc_nnz(?,0,...)","aligned_offset_realloc_nnz","aligned_offset_realloc_nn","ZERO",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 return DnPrivate_Debug_AlignedOffsetRealloc(p,s,a,o,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE_NN(p) void)
DnPrivate_Debug_AlignedFreeNN(void *p, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_free_nn(NULL)","aligned_free_nn","aligned_free","NULL",ac,addr); return; }
 DnPrivate_Debug_AlignedFree(p,ac,addr);
}
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28196)
#endif
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE_NN(p,a) size_t)
DnPrivate_Debug_AlignedMallocMSizeNN(void const *p, size_t a, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_malloc_msize_nn(NULL,...)","aligned_malloc_msize_nn","aligned_malloc_msize","NULL",ac,addr); return 0; }
 return DnPrivate_Debug_AlignedMallocMSize(p,a,ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE_NN(p,a,o) size_t)
DnPrivate_Debug_AlignedOffsetMallocMSizeNN(void const *p, size_t a, size_t o, char const *ac, void const *addr) {
 if (p == NULL) { DnPrivate_HandleNotXXXViolation("aligned_offset_malloc_msize_nn(NULL,...)","aligned_offset_malloc_msize_nn","aligned_offset_malloc_msize","NULL",ac,addr); return 0; }
 return DnPrivate_Debug_AlignedOffsetMallocMSize(p,a,o,ac,addr);
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif

#define DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE        sizeof(void*)
#define DEBUG_NEW_PRIVATE_INIT_ALIGNED(resp,basep,s) (((void**)(resp))[-1]=(basep))
#define DEBUG_NEW_PRIVATE_ALIGNED_BASEADDR(p)  \
 *(void **)((((uintptr_t)(p))&~(\
 DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-1))-\
 DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE)
#define DEBUG_NEW_PRIVATE_ALIGNED_ALLOC(base_alloc,result,s,a)          DEBUG_NEW_PRIVATE_UTIL_ALIGNED_MALLOC(result,base_alloc,s,a,DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE,DEBUG_NEW_PRIVATE_INIT_ALIGNED)
#define DEBUG_NEW_PRIVATE_ALIGNED_OFFSET_ALLOC(base_alloc,result,s,a,o) DEBUG_NEW_PRIVATE_UTIL_ALIGNED_OFFSET_MALLOC(result,base_alloc,s,a,o,DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE,DEBUG_NEW_PRIVATE_INIT_ALIGNED)
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4146)
#endif
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *)
DnPrivate_Debug_AlignedMalloc(size_t s, size_t a, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_malloc(?,non-power-of-2)","aligned_malloc","aligned_malloc","non-power-of-2",ac,addr); return NULL; }
#define DEBUG_NEW_PRIVATE_ALLOC(s) DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC,s,ac,addr);
 if (!s) return NULL;
 DEBUG_NEW_PRIVATE_ALIGNED_ALLOC(DEBUG_NEW_PRIVATE_ALLOC,result,s,a);
#undef DEBUG_NEW_PRIVATE_ALLOC
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *)
DnPrivate_Debug_AlignedOffsetMalloc(size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_offset_malloc(?,non-power-of-2,...)","aligned_offset_malloc","aligned_offset_malloc","non-power-of-2",ac,addr); return NULL; }
#define DEBUG_NEW_PRIVATE_ALLOC(s) DnPrivate_Debug_Malloc(DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_MALLOC,s,ac,addr);
 if (!s) return NULL;
 DEBUG_NEW_PRIVATE_ALIGNED_OFFSET_ALLOC(DEBUG_NEW_PRIVATE_ALLOC,result,s,a,o);
#undef DEBUG_NEW_PRIVATE_ALLOC
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *)
DnPrivate_Debug_AlignedCalloc(size_t s, size_t a, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_calloc(?,non-power-of-2)","aligned_calloc","aligned_calloc","non-power-of-2",ac,addr); return NULL; }
 result = DnPrivate_Debug_AlignedMalloc(s,a,ac,addr);
 if (result) memset(result,0,s);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *)
DnPrivate_Debug_AlignedOffsetCalloc(size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_offset_calloc(?,non-power-of-2,...)","aligned_offset_calloc","aligned_offset_calloc","non-power-of-2",ac,addr); return NULL; }
 result = DnPrivate_Debug_AlignedOffsetMalloc(s,a,o,ac,addr);
 if (result) memset(result,0,s);
 return result;
}

#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6001)
#endif
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *)
DnPrivate_Debug_AlignedRealloc(void *p, size_t s, size_t a, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_realloc(?,?,non-power-of-2)","aligned_realloc","aligned_realloc","non-power-of-2",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 if (!s) { DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 result = DnPrivate_Debug_AlignedMalloc(s,a,ac,addr);
 if (result && p) {
  size_t old_size = DnPrivate_Debug_AlignedMallocMSize(p,a,ac,addr);
  memcpy(result,p,old_size < s ? old_size : s);
  DnPrivate_Debug_AlignedFree(p,ac,addr);
 }
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *)
DnPrivate_Debug_AlignedOffsetRealloc(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr) {
 void *result;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_offset_realloc(?,?,non-power-of-2,...)","aligned_offset_realloc","aligned_offset_realloc","non-power-of-2",ac,addr); DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 if (!s) { DnPrivate_Debug_AlignedFree(p,ac,addr); return NULL; }
 result = DnPrivate_Debug_AlignedOffsetMalloc(s,a,o,ac,addr);
 if (result && p) {
  size_t old_size = DnPrivate_Debug_AlignedOffsetMallocMSize(p,a,o,ac,addr);
  memcpy(result,p,old_size < s ? old_size : s);
  DnPrivate_Debug_AlignedFree(p,ac,addr);
 }
 return result;
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif

DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_FREE(p) void)
DnPrivate_Debug_AlignedFree(void *p, char const *ac, void const *addr) {
 if (p) DnPrivate_Debug_Free(DEBUG_NEW_ALLOC_CALL_TYPE_ALIGNED_FREE,DEBUG_NEW_PRIVATE_ALIGNED_BASEADDR(p),ac,addr);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MSIZE(p,a) size_t)
DnPrivate_Debug_AlignedMallocMSize(void const *p, size_t a, char const *ac, void const *addr) {
 size_t sz_head,sz_foot;
 uintptr_t base;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_offset_malloc_msize(?,non-power-of-2,...)","aligned_offset_malloc_msize","aligned_offset_malloc_msize","non-power-of-2",ac,addr); return 0; }
 if (!p) return 0;
 sz_head = (uintptr_t)p-(base = (uintptr_t)DEBUG_NEW_PRIVATE_ALIGNED_BASEADDR(p));
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) {
  sz_foot = (a-1)+DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-sz_head;
 } else {
  sz_foot = (DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1)+DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-sz_head;
 }
 return DnPrivate_Debug_MallocMSize((void*)base,ac,addr)-(sz_head+sz_foot);
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MSIZE(p,a,o) size_t)
DnPrivate_Debug_AlignedOffsetMallocMSize(void const *p, size_t a, size_t o, char const *ac, void const *addr) {
 size_t sz_head,sz_foot,sz_skip;
 uintptr_t base;
 if (!DEBUG_NEW_UTIL_IS_2_POW_N(a)) { DnPrivate_HandleNotXXXViolation("aligned_offset_malloc_msize(?,non-power-of-2,...)","aligned_offset_malloc_msize","aligned_offset_malloc_msize","non-power-of-2",ac,addr); return 0; }
 if (!p) return 0;
 sz_head = (uintptr_t)p-(base = (uintptr_t)DEBUG_NEW_PRIVATE_ALIGNED_BASEADDR(p));
 sz_skip = (-o)&(DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-1);
 if ((a) > DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT) {
  sz_foot = sz_skip+(a-1)+
   DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-sz_head;
 } else {
  sz_foot = sz_skip+(DEBUG_NEW_CONFIG_PLATFORM_MIN_ALIGNMENT-1)+
   DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE-sz_head;
 }
 return DnPrivate_Debug_MallocMSize((void*)base,ac,addr)-(sz_head+sz_foot);
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#undef DEBUG_NEW_PRIVATE_ALIGNED_OFFSET_ALLOC
#undef DEBUG_NEW_PRIVATE_ALIGNED_ALLOC
#undef DEBUG_NEW_PRIVATE_ALIGNED_BASEADDR
#undef DEBUG_NEW_PRIVATE_INIT_ALIGNED
#undef DEBUG_NEW_PRIVATE_ALIGNED_HEADER_SIZE
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */




#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
#define DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(annotation,ret,namef,vnamef,args_,params_)\
DEBUG_NEW_FUNC_IMPL(annotation ret) namef args_ {\
 va_list args;\
 ret result;\
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();\
 va_start(args,fmt);\
 result = vnamef params_;\
 va_end(args);\
 return result;\
}
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_MALLOC(s),void *,DnDebug_Mallocf,DnPrivate_Debug_VMallocf,(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s),void *,DnDebug_MallocfNZ,DnPrivate_Debug_VMallocfNZ,(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC(p,s),void *,DnDebug_Reallocf,DnPrivate_Debug_VReallocf,(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s),void *,DnDebug_ReallocfNZ,DnPrivate_Debug_VReallocfNZ,(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s),void *,DnDebug_ReallocfNN,DnPrivate_Debug_VReallocfNN,(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s),void *,DnDebug_ReallocfNNZ,DnPrivate_Debug_VReallocfNNZ,(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_CALLOC(n,s),void *,DnDebug_Callocf,DnPrivate_Debug_VCallocf,(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s),void *,DnDebug_CallocfNZ,DnPrivate_Debug_VCallocfNZ,(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_MALLOC(s),void *,DnPrivate_Debug_Mallocf,DnPrivate_Debug_VMallocf,(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(mode,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s),void *,DnPrivate_Debug_MallocfNZ,DnPrivate_Debug_VMallocfNZ,(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(mode,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC(p,s),void *,DnPrivate_Debug_Reallocf,DnPrivate_Debug_VReallocf,(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s),void *,DnPrivate_Debug_ReallocfNZ,DnPrivate_Debug_VReallocfNZ,(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s),void *,DnPrivate_Debug_ReallocfNN,DnPrivate_Debug_VReallocfNN,(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s),void *,DnPrivate_Debug_ReallocfNNZ,DnPrivate_Debug_VReallocfNNZ,(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_CALLOC(n,s),void *,DnPrivate_Debug_Callocf,DnPrivate_Debug_VCallocf,(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(n,s,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s),void *,DnPrivate_Debug_CallocfNZ,DnPrivate_Debug_VCallocfNZ,(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(n,s,ac,addr,fmt,args))

#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a),void *,DnDebug_AlignedMallocf,DnPrivate_Debug_VAlignedMallocf,(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a),void *,DnDebug_AlignedMallocfNZ,DnPrivate_Debug_VAlignedMallocfNZ,(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o),void *,DnDebug_AlignedOffsetMallocf,DnPrivate_Debug_VAlignedOffsetMallocf,(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o),void *,DnDebug_AlignedOffsetMallocfNZ,DnPrivate_Debug_VAlignedOffsetMallocfNZ,(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a),void *,DnDebug_AlignedCallocf,DnPrivate_Debug_VAlignedCallocf,(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a),void *,DnDebug_AlignedCallocfNZ,DnPrivate_Debug_VAlignedCallocfNZ,(size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o),void *,DnDebug_AlignedOffsetCallocf,DnPrivate_Debug_VAlignedOffsetCallocf,(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o),void *,DnDebug_AlignedOffsetCallocfNZ,DnPrivate_Debug_VAlignedOffsetCallocfNZ,(size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a),void *,DnDebug_AlignedReallocf,DnPrivate_Debug_VAlignedReallocf,(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a),void *,DnDebug_AlignedReallocfNZ,DnPrivate_Debug_VAlignedReallocfNZ,(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a),void *,DnDebug_AlignedReallocfNN,DnPrivate_Debug_VAlignedReallocfNN,(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a),void *,DnDebug_AlignedReallocfNNZ,DnPrivate_Debug_VAlignedReallocfNNZ,(void *p, size_t s, size_t a, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o),void *,DnDebug_AlignedOffsetReallocf,DnPrivate_Debug_VAlignedOffsetReallocf,(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o),void *,DnDebug_AlignedOffsetReallocfNZ,DnPrivate_Debug_VAlignedOffsetReallocfNZ,(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o),void *,DnDebug_AlignedOffsetReallocfNN,DnPrivate_Debug_VAlignedOffsetReallocfNN,(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o),void *,DnDebug_AlignedOffsetReallocfNNZ,DnPrivate_Debug_VAlignedOffsetReallocfNNZ,(void *p, size_t s, size_t a, size_t o, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args))

DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a),void *,DnPrivate_Debug_AlignedMallocf,DnPrivate_Debug_VAlignedMallocf,(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a),void *,DnPrivate_Debug_AlignedMallocfNZ,DnPrivate_Debug_VAlignedMallocfNZ,(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o),void *,DnPrivate_Debug_AlignedOffsetMallocf,DnPrivate_Debug_VAlignedOffsetMallocf,(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o),void *,DnPrivate_Debug_AlignedOffsetMallocfNZ,DnPrivate_Debug_VAlignedOffsetMallocfNZ,(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a),void *,DnPrivate_Debug_AlignedCallocf,DnPrivate_Debug_VAlignedCallocf,(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a),void *,DnPrivate_Debug_AlignedCallocfNZ,DnPrivate_Debug_VAlignedCallocfNZ,(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o),void *,DnPrivate_Debug_AlignedOffsetCallocf,DnPrivate_Debug_VAlignedOffsetCallocf,(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o),void *,DnPrivate_Debug_AlignedOffsetCallocfNZ,DnPrivate_Debug_VAlignedOffsetCallocfNZ,(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a),void *,DnPrivate_Debug_AlignedReallocf,DnPrivate_Debug_VAlignedReallocf,(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a),void *,DnPrivate_Debug_AlignedReallocfNZ,DnPrivate_Debug_VAlignedReallocfNZ,(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a),void *,DnPrivate_Debug_AlignedReallocfNN,DnPrivate_Debug_VAlignedReallocfNN,(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a),void *,DnPrivate_Debug_AlignedReallocfNNZ,DnPrivate_Debug_VAlignedReallocfNNZ,(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o),void *,DnPrivate_Debug_AlignedOffsetReallocf,DnPrivate_Debug_VAlignedOffsetReallocf,(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o),void *,DnPrivate_Debug_AlignedOffsetReallocfNZ,DnPrivate_Debug_VAlignedOffsetReallocfNZ,(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o),void *,DnPrivate_Debug_AlignedOffsetReallocfNN,DnPrivate_Debug_VAlignedOffsetReallocfNN,(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,addr,fmt,args))
DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o),void *,DnPrivate_Debug_AlignedOffsetReallocfNNZ,DnPrivate_Debug_VAlignedOffsetReallocfNNZ,(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, ...),(p,s,a,o,ac,addr,fmt,args))
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#undef DEBUG_NEW_DECLARE_VA_LIST_FORMAT_PROXY

static char *DnPrivateSource_GenerateAllocInfo(char const *fmt, va_list args) {
 char buffer[2048];
 int len = vsprintf(buffer,fmt,args);
 char *result;
 if (len < 0) return NULL; /*< invalid format */
 if ((result = (char *)DEBUG_NEW_API_NATIVE_MALLOC_NZ(((size_t)len)+1)) == NULL) return NULL;
 DEBUG_NEW_STATIC_CAST(char *)(memcpy(result,buffer,(size_t)len))[len] = '\0';
 return result;
}

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *)
DnDebug_VMallocf(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VMallocf(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *)
DnDebug_VMallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VMallocfNZ(mode,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *)
DnDebug_VReallocf(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VReallocf(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *)
DnDebug_VReallocfNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VReallocfNZ(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *)
DnDebug_VReallocfNN(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VReallocfNN(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *)
DnDebug_VReallocfNNZ(void *p, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VReallocfNNZ(p,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *)
DnDebug_VCallocf(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VCallocf(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *)
DnDebug_VCallocfNZ(size_t n, size_t s, char const *ac, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();
 return DnPrivate_Debug_VCallocfNZ(n,s,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS(),fmt,args);
}


DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC(s) void *)
DnPrivate_Debug_VMallocf(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_Malloc(mode,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_MALLOC_NZ(s) void *)
DnPrivate_Debug_VMallocfNZ(Dn_AllocType_t mode, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_MallocNZ(mode,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC(p,s) void *)
DnPrivate_Debug_VReallocf(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_Realloc(p,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NZ(p,s) void *)
DnPrivate_Debug_VReallocfNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_ReallocNZ(p,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NN(p,s) void *)
DnPrivate_Debug_VReallocfNN(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_ReallocNN(p,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_REALLOC_NNZ(p,s) void *)
DnPrivate_Debug_VReallocfNNZ(void *p, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_ReallocNNZ(p,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC(n,s) void *)
DnPrivate_Debug_VCallocf(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_Calloc(n,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_CALLOC_NZ(n,s) void *)
DnPrivate_Debug_VCallocfNZ(size_t n, size_t s, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_CallocNZ(n,s,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}

#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC(s,a) void *)
DnPrivate_Debug_VAlignedMallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedMalloc(s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_MALLOC_NZ(s,a) void *)
DnPrivate_Debug_VAlignedMallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedMallocNZ(s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC(s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetMallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetMalloc(s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_MALLOC_NZ(s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetMallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetMallocNZ(s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC(s,a) void *)
DnPrivate_Debug_VAlignedCallocf(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedCalloc(s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_CALLOC_NZ(s,a) void *)
DnPrivate_Debug_VAlignedCallocfNZ(size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedCallocNZ(s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC(s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetCallocf(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetCalloc(s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_CALLOC_NZ(s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetCallocfNZ(size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetCallocNZ(s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC(p,s,a) void *)
DnPrivate_Debug_VAlignedReallocf(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedRealloc(p,s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NZ(p,s,a) void *)
DnPrivate_Debug_VAlignedReallocfNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedReallocNZ(p,s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NN(p,s,a) void *)
DnPrivate_Debug_VAlignedReallocfNN(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedReallocNN(p,s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_REALLOC_NNZ(p,s,a) void *)
DnPrivate_Debug_VAlignedReallocfNNZ(void *p, size_t s, size_t a, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedReallocNNZ(p,s,a,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC(p,s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetReallocf(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetRealloc(p,s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetReallocfNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetReallocNZ(p,s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetReallocfNN(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetReallocNN(p,s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
DEBUG_NEW_FUNC_DECL(DEBUG_NEW_ANNOTATION_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o) void *)
DnPrivate_Debug_VAlignedOffsetReallocfNNZ(void *p, size_t s, size_t a, size_t o, char const *ac, void const *addr, DEBUG_NEW_ANNOTATION_PRINTF_STR char const *fmt, va_list args) {
 void *result = DnPrivate_Debug_AlignedOffsetReallocNNZ(p,s,a,o,ac,addr);
 if (!result) return NULL;
 DnPrivate_GetRecord(result,ac,addr)->p_state.
  p_alloc_info = DnPrivateSource_GenerateAllocInfo(fmt,args);
 return result;
}
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */


#if DEBUG_NEW_CONFIG_TAILCHECK_SIZE
//////////////////////////////////////////////////////////////////////////
// Tail check
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT void const *)
DnCheck_Tail(Dn_MemPtr_t const *ptr) {
 char const *tail_ptr;
 unsigned int i;
 DN_ASSERTE(ptr && "Invalid pointer record",{return NULL;});
 tail_ptr = DEBUG_NEW_STATIC_CAST(char*)(ptr->p_state.p_self)+ptr->p_state.p_size;
 for (i = 0; i < DEBUG_NEW_CONFIG_TAILCHECK_SIZE; ++i)
  if (tail_ptr[i] != DEBUG_NEW_CONFIG_TAILCHECK_CHAR)
   return DEBUG_NEW_STATIC_CAST(void const *)(tail_ptr+i);
 return NULL;
}
#endif /* DEBUG_NEW_CONFIG_TAILCHECK_SIZE */

DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnCheck_Record(Dn_MemPtr_t const *rec, char const *ac) {
 return DnPrivate_CheckRecordInternal(rec,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnPrivate_CheckRecordInternal(Dn_MemPtr_t const *rec, char const *ac, void const *addr) {
 if (!rec) return 0; //< invalid pointer
 if (memcmp(&rec->p_magic,&Dn_Magic,sizeof(Dn_Magic)) != 0) {
  DnPrivate_HandleInvalidMagic(rec,ac,addr);
  return 0;
 }
#if DEBUG_NEW_CONFIG_TAILCHECK_SIZE
 {void const *tail_corruption = DnCheck_Tail(rec);
  if (tail_corruption)
   DnPrivate_HandleTailCorruption(rec,ac,addr,tail_corruption);
 }
#endif /* DEBUG_NEW_CONFIG_TAILCHECK_SIZE */
 return 1;
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnCheck_Pointer(void const *ptr, char const *ac) {
 return DnPrivate_CheckPointerInternal(ptr,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int)
DnPrivate_CheckPointerInternal(void const *ptr, char const *ac, void const *addr) {
 return DnPrivate_CheckRecordInternal(DnPrivate_GetRecord(DEBUG_NEW_CONST_CAST(void *)(ptr),ac,addr),ac,addr);
}


//////////////////////////////////////////////////////////////////////////
// Public functions for accessing pointer attributes
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *DnPointer_GetFile(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? DnAccess_GetFile(rec->p_state.p_access) : NULL;
}
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT int DnPointer_GetLine(void const *ptr, char const *ac) {
 int result; char const *line_str;
 if ((line_str = DnPointer_GetLineStr(ptr,ac)) == NULL) return -1;
 result = 0; while (*line_str) result = result*10+(*line_str++)-'0';
 return result;
}
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *DnPointer_GetLineStr(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? DnAccess_GetLine(rec->p_state.p_access) : NULL;
}
#if DEBUG_NEW_PRIVATE_HAVE_PP_FUNC
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *DnPointer_GetFunc(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? DnAccess_GetFunc(rec->p_state.p_access) : NULL;
}
#endif
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *DnPointer_GetExpr(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? DnAccess_GetExpr(rec->p_state.p_access) : NULL;
}
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT char const *DnPointer_GetMesg(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? rec->p_state.p_alloc_info : NULL;
}
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC == 1 */
#if DEBUG_NEW_CONFIG_HAVE_ALLOC_ID
DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT size_t DnPointer_GetID(void const *ptr, char const *ac) {
 Dn_MemPtr_t *rec = DnRecord_Get((void *)ptr,ac);
 return rec ? rec->p_state.p_alloc_id : 0;
}
#endif


//////////////////////////////////////////////////////////////////////////
// Pointer --> record transformation
//////////////////////////////////////////////////////////////////////////
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_MemPtr_t *)
DnRecord_Get(void *ptr, char const *ac) {
 return DnPrivate_GetRecord(ptr,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());
}
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ATTRIBUTE_SHOULD_USE_RESULT Dn_MemPtr_t *)
DnPrivate_GetRecord(void *ptr, char const *ac, void const *addr) {
 size_t offset;
 if (!ptr) return NULL;
 offset = DEBUG_NEW_STATIC_CAST(size_t)(
  DEBUG_NEW_STATIC_CAST(char *)(ptr)-DEBUG_NEW_STATIC_CAST(char *)(NULL));
 if (offset % DEBUG_NEW_PRIVATE_PLATFORM_MEM_ALIGNMENT != 0) {
  offset -= sizeof(size_t);
  if (offset % DEBUG_NEW_PRIVATE_PLATFORM_MEM_ALIGNMENT != 0) {
   DnPrivate_HandleUnalignedPointer(ptr,ac,addr);
   return NULL;
  }
  ptr = (char *)ptr-sizeof(size_t);
 }
 return DEBUG_NEW_REINTERPRET_CAST(Dn_MemPtr_t *)(
  DEBUG_NEW_STATIC_CAST(char*)(ptr)-DEBUG_NEW_PRIVATE_ALIGNED_POINTER_SIZE);
}


//////////////////////////////////////////////////////////////////////////
// debug_new c-utility hooks
//////////////////////////////////////////////////////////////////////////
#define DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNCS(\
 T,name,impl_name,nname,impl_nname,namenz,impl_namenz,nnamenz,impl_nnamenz,\
 strlen_func,strnlen_func,str_name,str_nname,str_namenz,str_nnamenz)\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRDUP(str) T *)\
name(T const *str, char const *ac) {\
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();\
 return impl_name(str,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) T *)\
nname(T const *str, size_t n, char const *ac) {\
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();\
 return impl_nname(str,n,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) T *)\
namenz(T const *str, char const *ac) {\
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();\
 return impl_namenz(str,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) T *)\
nnamenz(T const *str, size_t n, char const *ac) {\
 DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK();\
 return impl_nnamenz(str,n,ac,DEBUG_NEW_PRIVATE_RETURN_ADDRESS());\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRDUP_NZ(str) T *)\
impl_namenz(T const *str, char const *ac, void const *addr) {\
 if (!str) { DnPrivate_HandleStrDupNull(str_name,ac,addr); return NULL; }\
 if (!*str) { DnPrivate_HandleNotXXXViolation(\
  str_namenz "(\"\\0\")",str_namenz,str_name,"EmptyString",ac,addr); \
  return NULL;\
 }\
 return impl_name(str,ac,addr);\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRDUP(str) T *)\
impl_name(T const *str, char const *ac, void const *addr) {\
 size_t str_size;\
 T *result;\
 if (!str) { DnPrivate_HandleStrDupNull(str_name,ac,addr); return NULL; }\
 if (!*str) return NULL;\
 str_size = (strlen_func(str)+1)*sizeof(T);\
 result = DEBUG_NEW_STATIC_CAST(T *)(DnPrivate_Debug_Malloc(\
  DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,str_size,ac,addr));\
 if (result) memcpy(result,str,str_size);\
 return result;\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRNDUP_NZ(str,n) T *)\
impl_nnamenz(T const *str, size_t n, char const *ac, void const *addr) {\
 if (!str) { DnPrivate_HandleStrDupNull(str_nname,ac,addr); return NULL; }\
 if (!*str) { DnPrivate_HandleNotXXXViolation(\
  str_nnamenz "(\"\\0\",...)",str_nnamenz,str_nname,"EmptyString",ac,addr); \
  return NULL;\
 }\
 if (!n) { DnPrivate_HandleNotXXXViolation(\
  str_nnamenz "(...,0)",str_nnamenz,str_nname,"ZERO",ac,addr);\
  return NULL;\
 }\
 return impl_nname(str,n,ac,addr);\
}\
DEBUG_NEW_FUNC_IMPL(DEBUG_NEW_ANNOTATION_STRNDUP(str,n) T *)\
impl_nname(T const *str, size_t n, char const *ac, void const *addr) {\
 size_t str_size;\
 T *result;\
 if (!n) return NULL;\
 if (!str) { DnPrivate_HandleNotXXXViolation(\
  str_nnamenz "(NULL,...)",str_nname,str_nname,"ZERO",ac,addr); \
  return NULL;\
 }\
 if (!*str) return NULL;\
 str_size = strnlen_func(str,n)*sizeof(T);\
 result = DEBUG_NEW_STATIC_CAST(T *)(DnPrivate_Debug_Malloc(\
  DEBUG_NEW_ALLOC_CALL_TYPE_MALLOC,str_size+sizeof(T),ac,addr));\
 if (result) DEBUG_NEW_STATIC_CAST(T *)(\
  memcpy(result,str,str_size))[str_size] = '\0';\
 return result;\
}

DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNCS(
 char,
 DnDebug_StrDup,   DnPrivate_Debug_StrDup,
 DnDebug_StrNDup,  DnPrivate_Debug_StrNDup,
 DnDebug_StrDupNZ, DnPrivate_Debug_StrDupNZ,
 DnDebug_StrNDupNZ,DnPrivate_Debug_StrNDupNZ,
 strlen,DnPrivate_StrNLen,
 "strdup","strndup","strdup_nz","strndup_nz")
DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNCS(
 DnWchar_t,
 DnDebug_WcsDup,   DnPrivate_Debug_WcsDup,
 DnDebug_WcsNDup,  DnPrivate_Debug_WcsNDup,
 DnDebug_WcsDupNZ, DnPrivate_Debug_WcsDupNZ,
 DnDebug_WcsNDupNZ,DnPrivate_Debug_WcsNDupNZ,
 DEBUG_NEW_PRIVATE_WCSLEN,DnPrivate_WcsNLen,
 "wcsdup","wcsndup","wcsdup_nz","wcsndup_nz")
DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNCS(
 unsigned char,
 DnDebug_MbsDup,   DnPrivate_Debug_MbsDup,
 DnDebug_MbsNDup,  DnPrivate_Debug_MbsNDup,
 DnDebug_MbsDupNZ, DnPrivate_Debug_MbsDupNZ,
 DnDebug_MbsNDupNZ,DnPrivate_Debug_MbsNDupNZ,
 DnPrivate_MbsLen,DnPrivate_MbsNLen,
 "_mbsdup","_mbsndup","_mbsdup_nz","_mbsndup_nz")
#undef DEBUG_NEW_PRIVATE_MAKE_STRDUP_FUNCS

//////////////////////////////////////////////////////////////////////////
// Deprecated functions
#if DEBUG_NEW_CONFIG_HAVE_DEPRECATED
DEBUG_NEW_FUNC_IMPL(void) Dn_Print(char const *msg) {
 DnPrivate_Print(DnThread_GetPrintFunc(),msg);
}
DEBUG_NEW_FUNC_IMPL(void) Dn_Printf(char const *msg, ...) {
 char buffer[2048];
 va_list args;
 va_start(args,msg);
 vsprintf(buffer,msg,args);
 va_end(args);
 DnPrivate_Print(DnThread_GetPrintFunc(),buffer);
}
DEBUG_NEW_FUNC_IMPL(void) Dn_VPrintf(char const *msg, va_list args) {
 DnPrivate_VPrintf(DnThread_GetPrintFunc(),msg,args);
}
#endif // DEBUG_NEW_CONFIG_HAVE_DEPRECATED

#ifdef __cplusplus
}
#endif /* __cplusplus */

DEBUG_NEW_CXX_NAMESPACE_LEAVE_ROOT

#undef DEBUG_NEW_PRINT_LOCK_ENTER
#undef DEBUG_NEW_PRINT_LOCK_LEAVE
#undef DEBUG_NEW_PRIVATE_SOURCE_OS_MEM_CAN_ALLOC
#undef DEBUG_NEW_PRIVATE_SOURCE_LOG_OPEN
#undef DEBUG_NEW_PRIVATE_SOURCE_LOG_INIT
#undef DEBUG_NEW_PRIVATE_SOURCE_LOG_QUIT
#undef DEBUG_NEW_PRIVATE_SOURCE_LOG_DO_WRITE
#undef DEBUG_NEW_PRIVATE_SOURCE_LOG_WRITE
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_INIT
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_QUIT
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_ID_GET_NEXT
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_CHECK
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_INIT
#undef DEBUG_NEW_PRIVATE_SOURCE_ALLOC_BREAKPOINT_QUIT
#undef DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_INITIALIZED
#undef DEBUG_NEW_PRIVATE_SOURCE_LIB_SET_FINALIZED
#undef DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_FMT
#undef DEBUG_NEW_PRIVATE_PRINTF_SIZE_T_ARG
#undef DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_FMT
#undef DEBUG_NEW_PRIVATE_PRINTF_SSIZE_T_ARG
#undef DEBUG_NEW_PRIVATE_OFFSETOF
#undef DEBUG_NEW_PRIVATE_TRY_OUTPUT_DEBUG_STRING
#undef DEBUG_NEW_PRIVATE_SOURCE_CHECK_INIT

//////////////////////////////////////////////////////////////////////////
// End implementation of <debug_new.c>
#ifdef _MSC_VER
#pragma warning(pop)
#endif /* _MSC_VER */

/* v re-enable debug_new */
#include "debug_new_enable.inl"
#endif /* DEBUG_NEW_CONFIG_ENABLED */

#undef DEBUG_NEW_CONFIG_HAVE_LIMITED_API
#if defined(DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API)
#if DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API
# define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 1
#else /* DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API */
# define DEBUG_NEW_CONFIG_HAVE_LIMITED_API 0
#endif /* !DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API */
#undef DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API
#endif /* DEBUG_NEW_PRIVATE_OLD_CONFIG_HAVE_LIMITED_API */

#if defined(_MSC_VER) && !DEBUG_NEW_CONFIG_LINK_STATICALLY
#pragma warning(disable:4710) /*< 'xxx' : function not inlined */
#pragma warning(disable:4505) /*< 'xxx' : unreferenced local function has been removed */
#pragma warning(disable:4514) /*< 'xxx' : unreferenced inline function has been removed */
#endif /* _MSC_VER && !DEBUG_NEW_CONFIG_LINK_STATICALLY */

#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !_DEBUG_NEW_CPP */
#endif /* !DEBUG_NEW_CONFIG_HAVE_DEPRECATED || !_DEBUG_NEW_C */
#undef DEBUG_NEW_CONFIG_BUILDING_SOURCE
#endif /* !GUARD_DEBUG_NEW_C */
