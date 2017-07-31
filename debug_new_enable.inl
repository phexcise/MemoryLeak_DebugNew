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

/*
 *
 * This is an auto generated file.
 * Changes to this file will be
 * overwritten without any warning.
 *
 * To change this file, please change
 * "__debug_new_enable_disable_gen.py" instead
 *
 * Generated at:
 *   Thu, 03 Mar 2016 14:56:07
 *
 */

#ifndef DEBUG_NEW_PRIVATE_DISABLED
#ifdef DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED
# define DEBUG_NEW_PRIVATE_DISABLED 1
#else /* DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED */
/* define DEBUG_NEW_PRIVATE_DISABLED with an invalid value, to force a recursion error */
# define DEBUG_NEW_PRIVATE_DISABLED 26
#endif /* !DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED */
#endif /* !DEBUG_NEW_PRIVATE_DISABLED */
#if DEBUG_NEW_PRIVATE_DISABLED == 25
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 24
#elif DEBUG_NEW_PRIVATE_DISABLED == 24
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 23
#elif DEBUG_NEW_PRIVATE_DISABLED == 23
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 22
#elif DEBUG_NEW_PRIVATE_DISABLED == 22
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 21
#elif DEBUG_NEW_PRIVATE_DISABLED == 21
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 20
#elif DEBUG_NEW_PRIVATE_DISABLED == 20
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 19
#elif DEBUG_NEW_PRIVATE_DISABLED == 19
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 18
#elif DEBUG_NEW_PRIVATE_DISABLED == 18
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 17
#elif DEBUG_NEW_PRIVATE_DISABLED == 17
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 16
#elif DEBUG_NEW_PRIVATE_DISABLED == 16
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 15
#elif DEBUG_NEW_PRIVATE_DISABLED == 15
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 14
#elif DEBUG_NEW_PRIVATE_DISABLED == 14
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 13
#elif DEBUG_NEW_PRIVATE_DISABLED == 13
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 12
#elif DEBUG_NEW_PRIVATE_DISABLED == 12
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 11
#elif DEBUG_NEW_PRIVATE_DISABLED == 11
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 10
#elif DEBUG_NEW_PRIVATE_DISABLED == 10
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 9
#elif DEBUG_NEW_PRIVATE_DISABLED == 9
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 8
#elif DEBUG_NEW_PRIVATE_DISABLED == 8
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 7
#elif DEBUG_NEW_PRIVATE_DISABLED == 7
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 6
#elif DEBUG_NEW_PRIVATE_DISABLED == 6
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 5
#elif DEBUG_NEW_PRIVATE_DISABLED == 5
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 4
#elif DEBUG_NEW_PRIVATE_DISABLED == 4
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 3
#elif DEBUG_NEW_PRIVATE_DISABLED == 3
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 2
#elif DEBUG_NEW_PRIVATE_DISABLED == 2
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 1
#elif DEBUG_NEW_PRIVATE_DISABLED == 1
# undef DEBUG_NEW_PRIVATE_DISABLED
#ifdef GUARD_DEBUG_NEW_H
#if DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS
#ifndef DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED
# ifdef __cplusplus
#  undef new
# endif /* __cplusplus */
# if DEBUG_NEW_CONFIG_ENABLED || !defined(DEBUG_NEW_CONFIG_USE_C_MALLOC)
#  undef malloc
#  undef free
#  undef realloc
#  undef calloc
#  undef strdup
#  undef strndup
#  undef wcsdup
#  undef wcsndup
#  undef mbsdup
#  undef mbsndup
#  undef _strdup
#  undef _wcsdup
#  undef _mbsdup
#  if DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC && DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#   undef _aligned_free
#   undef _aligned_malloc
#   undef _aligned_offset_malloc
#   undef _aligned_realloc
#   undef _aligned_offset_realloc
#   undef _aligned_msize
#  endif /* DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC && DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#  if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#   undef malloc_usable_size
#  endif /* DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE */
#  if DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#   undef _msize
#  endif /* DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE */
#  if DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#   undef malloc_size
#  endif /* DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE */
# endif /* DEBUG_NEW_CONFIG_ENABLED || !DEBUG_NEW_CONFIG_USE_C_MALLOC */
# ifdef __cplusplus
#  undef placement
# endif /* __cplusplus */
# undef mstate_push
# undef mstate_pop
# undef mstate_global_push
# undef mstate_global_pop
# if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#  undef mallocf
#  undef reallocf
#  undef callocf
#  undef vmallocf
#  undef vreallocf
#  undef vcallocf
#  if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#   undef mallocf_nz
#   undef reallocf_nz
#   undef reallocf_nn
#   undef reallocf_nnz
#   undef callocf_nz
#   undef vmallocf_nz
#   undef vreallocf_nz
#   undef vreallocf_nn
#   undef vreallocf_nnz
#   undef vcallocf_nz
#  endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  undef malloc_nz
#  undef realloc_nz
#  undef realloc_nn
#  undef realloc_nnz
#  undef calloc_nz
#  undef free_nn
#  undef strdup_nz
#  undef strndup_nz
#  undef wcsdup_nz
#  undef wcsndup_nz
#  undef mbsdup_nz
#  undef mbsndup_nz
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
#  undef malloc_msize
#  if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#   undef malloc_msize_nn
#  endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */
# if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#  undef aligned_malloc
#  undef aligned_calloc
#  undef aligned_realloc
#  ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#   undef aligned_malloc_msize
#  endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE */
#  undef aligned_offset_malloc
#  undef aligned_offset_calloc
#  undef aligned_offset_realloc
#  ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#   undef aligned_offset_malloc_msize
#  endif /*DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE */
#  undef aligned_free
#  if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#   undef aligned_malloc_nz
#   undef aligned_calloc_nz
#   undef aligned_realloc_nz
#   undef aligned_realloc_nn
#   undef aligned_realloc_nnz
#   ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#    undef aligned_malloc_msize_nn
#   endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN */
#   undef aligned_offset_malloc_nz
#   undef aligned_offset_calloc_nz
#   undef aligned_offset_realloc_nz
#   undef aligned_offset_realloc_nn
#   undef aligned_offset_realloc_nnz
#   ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#    undef aligned_offset_malloc_msize_nn
#   endif /* DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#   undef aligned_free_nn
#  endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#  if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#   undef aligned_mallocf
#   undef aligned_callocf
#   undef aligned_reallocf
#   undef aligned_offset_mallocf
#   undef aligned_offset_callocf
#   undef aligned_offset_reallocf
#   undef valigned_mallocf
#   undef valigned_callocf
#   undef valigned_reallocf
#   undef valigned_offset_mallocf
#   undef valigned_offset_callocf
#   undef valigned_offset_reallocf
#   if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#    undef aligned_mallocf_nz
#    undef aligned_callocf_nz
#    undef aligned_reallocf_nz
#    undef aligned_reallocf_nn
#    undef aligned_reallocf_nnz
#    undef aligned_offset_mallocf_nz
#    undef aligned_offset_callocf_nz
#    undef aligned_offset_reallocf_nz
#    undef aligned_offset_reallocf_nn
#    undef aligned_offset_reallocf_nnz
#    undef valigned_mallocf_nz
#    undef valigned_callocf_nz
#    undef valigned_reallocf_nz
#    undef valigned_reallocf_nn
#    undef valigned_reallocf_nnz
#    undef valigned_offset_mallocf_nz
#    undef valigned_offset_callocf_nz
#    undef valigned_offset_reallocf_nz
#    undef valigned_offset_reallocf_nn
#    undef valigned_offset_reallocf_nnz
#   endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#  endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
# endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#else /* !DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED */
#undef DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED
#endif /* DEBUG_NEW_PRIVATE_WANT_QUICK_ENABLED */
//////////////////////////////////////////////////////////////////////////
// Override native API Methods
# if DEBUG_NEW_CONFIG_ENABLED && defined(DEBUG_NEW_CXX_NEW)
#  define new                 DEBUG_NEW_CXX_NEW
# endif /* DEBUG_NEW_CONFIG_ENABLED && defined(DEBUG_NEW_CXX_NEW) */
# if DEBUG_NEW_CONFIG_ENABLED || !defined(DEBUG_NEW_CONFIG_USE_C_MALLOC)
#  define malloc              DEBUG_NEW_API_MALLOC
#  define free                DEBUG_NEW_API_FREE
#  define realloc             DEBUG_NEW_API_REALLOC
#  define calloc              DEBUG_NEW_API_CALLOC
#  define strdup              DEBUG_NEW_API_STRDUP
#  define strndup             DEBUG_NEW_API_STRNDUP
#  define wcsdup              DEBUG_NEW_API_WCSDUP
#  define wcsndup             DEBUG_NEW_API_WCSNDUP
#  define mbsdup              DEBUG_NEW_API_MBSDUP
#  define mbsndup             DEBUG_NEW_API_MBSNDUP
#  define _strdup             DEBUG_NEW_API_STRDUP
#  define _wcsdup             DEBUG_NEW_API_WCSDUP
#  define _mbsdup             DEBUG_NEW_API_MBSDUP
#  if DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC && DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#   define _aligned_free            DEBUG_NEW_API_ALIGNED_FREE
#   define _aligned_malloc          DEBUG_NEW_API_ALIGNED_MALLOC
#   define _aligned_offset_malloc   DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC
#   define _aligned_realloc         DEBUG_NEW_API_ALIGNED_REALLOC
#   define _aligned_offset_realloc  DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC
#   define _aligned_msize           DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#  endif /* DEBUG_NEW_COMPILER_HAVE_MSVC_ALIGNED_MALLOC && DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#  if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#   define malloc_usable_size DEBUG_NEW_API_MALLOC_MSIZE
#  endif /* DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE */
#  if DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#   define _msize             DEBUG_NEW_API_MALLOC_MSIZE
#  endif /* DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE */
#  if DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#   define malloc_size        DEBUG_NEW_API_MALLOC_MSIZE_C
#  endif /* DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE */
# endif /* DEBUG_NEW_CONFIG_ENABLED || !DEBUG_NEW_CONFIG_USE_C_MALLOC */
#endif /* DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS */

#ifdef DEBUG_NEW_CXX_PLACEMENT
//////////////////////////////////////////////////////////////////////////
// <<placement>>
//   - indicator keyword for placement new
//   - Because of the way I overwrite the new keyword, to add file, line,
//     expr and func information, I have problems detecting placement new calls:
//     >> int *x = (int *)malloc(sizeof(int));
//     >>  placement new(x) int(42); //< when calling a placement new,
//     >>                            //  add the keyword "placement"
//     >>                            //  in front of it.
//     >> free(x);
//   - since this is not a standard keyword --> allways overwritten
//   - If debug_new is disabled, "placement" will expand to nothing
//////////////////////////////////////////////////////////////////////////
#define placement            DEBUG_NEW_CXX_PLACEMENT
#endif /* DEBUG_NEW_CXX_PLACEMENT */

//////////////////////////////////////////////////////////////////////////
// <<Memory state macros>>
//  - When programming, a single-threaded application,
//    it doesn't matter, if you use:
//    - "mstate_push()" or "mstate_global_push()" for pushing
//    - "mstate_pop()" or "mstate_global_pop()" for popping
//    - this is useful, when executing test code and
//      debugging memory leaks, without exiting the application:
//    >> void some_func(void) {
//    >>  mstate_push();
//    >>   int *x = new int[42];
//    >>   int *y = new int();
//    >>  mstate_pop(); //< memory differences between here and
//    >>                //  and the last call to "mstate_push()"
//    >>                //  will be listed (if any)
//    >> }
//  - When programming, a multi-threaded application,
//    - "mstate_push()" is used, to push the state of the current thread
//    - "mstate_pop()" is used, to pop it
//    - "mstate_global_push()", to push the global application state.
//    - "mstate_global_pop()", to pop it
//    - This differentiation is required, to safely test code running
//      is a multi-threaded application and debug memory-leaks associated
//      with the execution of threads:
//     >> void ThreadA(void) {
//     >>  mstate_push();
//     >>   int *x = new int[42];
//     >>   int *y = new int();
//     >>  mstate_pop();
//     >> }
//     >> void ThreadB(void) {
//     >>  mstate_push();
//     >>   int *x = new int[123];
//     >>   int *y = new int();
//     >>  mstate_pop();
//     >> }
//     >> // ThreadA and ThreadB
//     >> // can run in parallel;
//     >> void main_thread_launcher(void) {
//     >>  mstate_global_push();
//     >>   MagicThread<ThreadA>::launch();
//     >>   MagicThread<ThreadB>::launch();
//     >>  mstate_global_pop();
//     >>  // ^ using global push and pop,
//     >>  //   because we are the main thread
//     >>  //   this way we get information
//     >>  //   about leaks, that may have occurred, in "MagicThread"
//     >> }
//////////////////////////////////////////////////////////////////////////
#define mstate_push          DEBUG_NEW_API_MSTATE_PUSH
#define mstate_pop           DEBUG_NEW_API_MSTATE_POP
#define mstate_global_push   DEBUG_NEW_API_MSTATE_GLOBAL_PUSH
#define mstate_global_pop    DEBUG_NEW_API_MSTATE_GLOBAL_POP

#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
//////////////////////////////////////////////////////////////////////////
// Formatted allocation functions
#define mallocf              DEBUG_NEW_API_MALLOCF
#define reallocf             DEBUG_NEW_API_REALLOCF
#define callocf              DEBUG_NEW_API_CALLOCF
#define vmallocf             DEBUG_NEW_API_VMALLOCF
#define vreallocf            DEBUG_NEW_API_VREALLOCF
#define vcallocf             DEBUG_NEW_API_VCALLOCF
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */

#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
//////////////////////////////////////////////////////////////////////////
// Optimized allocation functions (improve allocation speed, by reducing parameter checks)
// --> *_nz  : Size parameter may not be ZERO
// --> *_nn  : Pointer parameter may not be NULL
// --> *_nnz : Both of the above
#define malloc_nz            DEBUG_NEW_API_MALLOC_NZ
#define realloc_nz           DEBUG_NEW_API_REALLOC_NZ
#define realloc_nn           DEBUG_NEW_API_REALLOC_NN
#define realloc_nnz          DEBUG_NEW_API_REALLOC_NNZ
#define calloc_nz            DEBUG_NEW_API_CALLOC_NZ
#define free_nn              DEBUG_NEW_API_FREE_NN
#define strdup_nz            DEBUG_NEW_API_STRDUP_NZ
#define wcsdup_nz            DEBUG_NEW_API_WCSDUP_NZ
#define mbsdup_nz            DEBUG_NEW_API_MBSDUP_NZ
#define strndup_nz           DEBUG_NEW_API_STRNDUP_NZ
#define wcsndup_nz           DEBUG_NEW_API_WCSNDUP_NZ
#define mbsndup_nz           DEBUG_NEW_API_MBSNDUP_NZ
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#define mallocf_nz           DEBUG_NEW_API_MALLOCF_NZ
#define reallocf_nz          DEBUG_NEW_API_REALLOCF_NZ
#define reallocf_nn          DEBUG_NEW_API_REALLOCF_NN
#define reallocf_nnz         DEBUG_NEW_API_REALLOCF_NNZ
#define callocf_nz           DEBUG_NEW_API_CALLOCF_NZ
#define vmallocf_nz          DEBUG_NEW_API_VMALLOCF_NZ
#define vreallocf_nz         DEBUG_NEW_API_VREALLOCF_NZ
#define vreallocf_nn         DEBUG_NEW_API_VREALLOCF_NN
#define vreallocf_nnz        DEBUG_NEW_API_VREALLOCF_NNZ
#define vcallocf_nz          DEBUG_NEW_API_VCALLOCF_NZ
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */

//////////////////////////////////////////////////////////////////////////
// malloc_msize returns the size of a given memory block
// By default this extension will only be provided, if the current
// platform is naturally able, to provide such feature.
// - To make sure you code is portable, you
//   can always test for this extensions:
// >> void *p = malloc(42);
// >> #ifdef malloc_msize
// >> size_t s = malloc_msize(p);
// >> #endif /* malloc_msize */
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
# define malloc_msize        DEBUG_NEW_API_MALLOC_MSIZE_C
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
# define malloc_msize_nn     DEBUG_NEW_API_MALLOC_MSIZE_C_NN
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */

//////////////////////////////////////////////////////////////////////////
// aligned_malloc and his friends can be used, to
// allocate memory aligned by a custom degree in its address.
// e.g.:
// >> void *p = aligned_malloc(16,64); // optimize to aligned_malloc_nz
// >> if (p) {
// >>  assert((uintptr_t)p % 64 == 0);
// >>  aligned_free(p); // optimize to aligned_free_nn
// >> }
// NOTE: When using aligned_malloc_msize portably, you must still
//       check for its availability by checking, if it is defined as a macro:
//    >> #ifdef aligned_malloc_msize
//    >>  ... // Code that uses aligned_malloc_msize
//    >> #endif /* aligned_malloc_msize */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
#define aligned_malloc                 DEBUG_NEW_API_ALIGNED_MALLOC
#define aligned_calloc                 DEBUG_NEW_API_ALIGNED_CALLOC
#define aligned_realloc                DEBUG_NEW_API_ALIGNED_REALLOC
#ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#define aligned_malloc_msize           DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE */
#define aligned_offset_malloc          DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC
#define aligned_offset_calloc          DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC
#define aligned_offset_realloc         DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC
#ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#define aligned_offset_malloc_msize    DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#endif /*DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE */
#define aligned_free                   DEBUG_NEW_API_ALIGNED_FREE
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#define aligned_malloc_nz              DEBUG_NEW_API_ALIGNED_MALLOC_NZ
#define aligned_calloc_nz              DEBUG_NEW_API_ALIGNED_CALLOC_NZ
#define aligned_realloc_nz             DEBUG_NEW_API_ALIGNED_REALLOC_NZ
#define aligned_realloc_nn             DEBUG_NEW_API_ALIGNED_REALLOC_NN
#define aligned_realloc_nnz            DEBUG_NEW_API_ALIGNED_REALLOC_NNZ
#ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#define aligned_malloc_msize_nn        DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN */
#define aligned_offset_malloc_nz       DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_NZ
#define aligned_offset_calloc_nz       DEBUG_NEW_API_ALIGNED_OFFSET_CALLOC_NZ
#define aligned_offset_realloc_nz      DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NZ
#define aligned_offset_realloc_nn      DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NN
#define aligned_offset_realloc_nnz     DEBUG_NEW_API_ALIGNED_OFFSET_REALLOC_NNZ
#ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#define aligned_offset_malloc_msize_nn DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#endif /* DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#define aligned_free_nn                DEBUG_NEW_API_ALIGNED_FREE_NN
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#define aligned_mallocf                DEBUG_NEW_API_ALIGNED_MALLOCF
#define aligned_callocf                DEBUG_NEW_API_ALIGNED_CALLOCF
#define aligned_reallocf               DEBUG_NEW_API_ALIGNED_REALLOCF
#define aligned_offset_mallocf         DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF
#define aligned_offset_callocf         DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF
#define aligned_offset_reallocf        DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF
#define valigned_mallocf               DEBUG_NEW_API_VALIGNED_MALLOCF
#define valigned_callocf               DEBUG_NEW_API_VALIGNED_CALLOCF
#define valigned_reallocf              DEBUG_NEW_API_VALIGNED_REALLOCF
#define valigned_offset_mallocf        DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF
#define valigned_offset_callocf        DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF
#define valigned_offset_reallocf       DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#define aligned_mallocf_nz             DEBUG_NEW_API_ALIGNED_MALLOCF_NZ
#define aligned_callocf_nz             DEBUG_NEW_API_ALIGNED_CALLOCF_NZ
#define aligned_reallocf_nz            DEBUG_NEW_API_ALIGNED_REALLOCF_NZ
#define aligned_reallocf_nn            DEBUG_NEW_API_ALIGNED_REALLOCF_NN
#define aligned_reallocf_nnz           DEBUG_NEW_API_ALIGNED_REALLOCF_NNZ
#define aligned_offset_mallocf_nz      DEBUG_NEW_API_ALIGNED_OFFSET_MALLOCF_NZ
#define aligned_offset_callocf_nz      DEBUG_NEW_API_ALIGNED_OFFSET_CALLOCF_NZ
#define aligned_offset_reallocf_nz     DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NZ
#define aligned_offset_reallocf_nn     DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NN
#define aligned_offset_reallocf_nnz    DEBUG_NEW_API_ALIGNED_OFFSET_REALLOCF_NNZ
#define valigned_mallocf_nz            DEBUG_NEW_API_VALIGNED_MALLOCF_NZ
#define valigned_callocf_nz            DEBUG_NEW_API_VALIGNED_CALLOCF_NZ
#define valigned_reallocf_nz           DEBUG_NEW_API_VALIGNED_REALLOCF_NZ
#define valigned_reallocf_nn           DEBUG_NEW_API_VALIGNED_REALLOCF_NN
#define valigned_reallocf_nnz          DEBUG_NEW_API_VALIGNED_REALLOCF_NNZ
#define valigned_offset_mallocf_nz     DEBUG_NEW_API_VALIGNED_OFFSET_MALLOCF_NZ
#define valigned_offset_callocf_nz     DEBUG_NEW_API_VALIGNED_OFFSET_CALLOCF_NZ
#define valigned_offset_reallocf_nz    DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NZ
#define valigned_offset_reallocf_nn    DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NN
#define valigned_offset_reallocf_nnz   DEBUG_NEW_API_VALIGNED_OFFSET_REALLOCF_NNZ
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */

#endif /* GUARD_DEBUG_NEW_H */
#else
# error Recursion error in debug_new enable/disable \
  - <debug_new_enable.inl> got included without <debug_new_disable.inl>
#endif
