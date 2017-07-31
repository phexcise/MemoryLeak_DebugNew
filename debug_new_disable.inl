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

#if !defined(DEBUG_NEW_PRIVATE_DISABLED) || defined(DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED)
#ifdef GUARD_DEBUG_NEW_H
#ifndef DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED
#define DEBUG_NEW_PRIVATE_DISABLED 1
#ifdef __cplusplus
# undef placement
#endif /* __cplusplus */
#undef mstate_push
#undef mstate_pop
#undef mstate_global_push
#undef mstate_global_pop
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
# undef mallocf
# undef reallocf
# undef callocf
# undef vmallocf
# undef vreallocf
# undef vcallocf
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  undef mallocf_nz
#  undef reallocf_nz
#  undef reallocf_nn
#  undef reallocf_nnz
#  undef callocf_nz
#  undef vmallocf_nz
#  undef vreallocf_nz
#  undef vreallocf_nn
#  undef vreallocf_nnz
#  undef vcallocf_nz
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
# undef malloc_nz
# undef realloc_nz
# undef realloc_nn
# undef realloc_nnz
# undef calloc_nz
# undef free_nn
# undef strdup_nz
# undef strndup_nz
# undef wcsdup_nz
# undef wcsndup_nz
# undef mbsdup_nz
# undef mbsndup_nz
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
# undef malloc_msize
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  undef malloc_msize_nn
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
# undef aligned_malloc
# undef aligned_calloc
# undef aligned_realloc
# ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE
#  undef aligned_malloc_msize
# endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE */
# undef aligned_offset_malloc
# undef aligned_offset_calloc
# undef aligned_offset_realloc
# ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE
#  undef aligned_offset_malloc_msize
# endif /*DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE */
# undef aligned_free
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  undef aligned_malloc_nz
#  undef aligned_calloc_nz
#  undef aligned_realloc_nz
#  undef aligned_realloc_nn
#  undef aligned_realloc_nnz
#  ifdef DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN
#   undef aligned_malloc_msize_nn
#  endif /* DEBUG_NEW_API_ALIGNED_MALLOC_MSIZE_NN */
#  undef aligned_offset_malloc_nz
#  undef aligned_offset_calloc_nz
#  undef aligned_offset_realloc_nz
#  undef aligned_offset_realloc_nn
#  undef aligned_offset_realloc_nnz
#  ifdef DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#   undef aligned_offset_malloc_msize_nn
#  endif /* DEBUG_NEW_API_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#  undef aligned_free_nn
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#  undef aligned_mallocf
#  undef aligned_callocf
#  undef aligned_reallocf
#  undef aligned_offset_mallocf
#  undef aligned_offset_callocf
#  undef aligned_offset_reallocf
#  undef valigned_mallocf
#  undef valigned_callocf
#  undef valigned_reallocf
#  undef valigned_offset_mallocf
#  undef valigned_offset_callocf
#  undef valigned_offset_reallocf
#  if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#   undef aligned_mallocf_nz
#   undef aligned_callocf_nz
#   undef aligned_reallocf_nz
#   undef aligned_reallocf_nn
#   undef aligned_reallocf_nnz
#   undef aligned_offset_mallocf_nz
#   undef aligned_offset_callocf_nz
#   undef aligned_offset_reallocf_nz
#   undef aligned_offset_reallocf_nn
#   undef aligned_offset_reallocf_nnz
#   undef valigned_mallocf_nz
#   undef valigned_callocf_nz
#   undef valigned_reallocf_nz
#   undef valigned_reallocf_nn
#   undef valigned_reallocf_nnz
#   undef valigned_offset_mallocf_nz
#   undef valigned_offset_callocf_nz
#   undef valigned_offset_reallocf_nz
#   undef valigned_offset_reallocf_nn
#   undef valigned_offset_reallocf_nnz
#  endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#if DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS
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
#endif /* DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS */
#else /* !DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED */
#undef DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED
#endif /* !DEBUG_NEW_PRIVATE_WANT_QUICK_DISABLED */
#if DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED
//////////////////////////////////////////////////////////////////////////
// Link extensions as stub / alias
#ifdef __cplusplus
# define placement                  /* nothing */
#endif /* __cplusplus */
#define mstate_push()               1
#define mstate_pop()                0
#define mstate_global_push()        1
#define mstate_global_pop()         0
#if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
# define mallocf(s,...)               malloc(s)
# define reallocf(p,s,...)            realloc(p,s)
# define callocf(n,s,...)             calloc(n,s)
# define vmallocf(s,fmt,args)         malloc(s)
# define vreallocf(p,s,fmt,args)      realloc(p,s)
# define vcallocf(n,s,fmt,args)       calloc(n,s)
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  define mallocf_nz(s,...)           malloc(s)
#  define reallocf_nz(p,s,...)        realloc(p,s)
#  define reallocf_nn(p,s,...)        realloc(p,s)
#  define reallocf_nnz(p,s,...)       realloc(p,s)
#  define callocf_nz(n,s,...)         calloc(n,s)
#  define vmallocf_nz(s,fmt,args)     malloc(s)
#  define vreallocf_nz(p,s,fmt,args)  realloc(p,s)
#  define vreallocf_nn(p,s,fmt,args)  realloc(p,s)
#  define vreallocf_nnz(p,s,fmt,args) realloc(p,s)
#  define vcallocf_nz(n,s,fmt,args)   calloc(n,s)
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
# define malloc_nz                    malloc
# define realloc_nz                   realloc
# define realloc_nn                   realloc
# define realloc_nnz                  realloc
# define calloc_nz                    calloc
# define free_nn                      free
# define strdup_nz                    strdup
# define wcsdup_nz                    wcsdup
# define mbsdup_nz                    mbsdup
# define strndup_nz                   strndup
# define wcsndup_nz                   wcsndup
# define mbsndup_nz                   mbsndup
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#if DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE
# if DEBUG_NEW_CONFIG_HAVE_POSIX_MALLOC_USABLE_SIZE
#  define malloc_msize(p)             malloc_usable_size(DEBUG_NEW_CONST_CAST(void*)(p))
# elif DEBUG_NEW_CONFIG_HAVE_MSVC_MSIZE
#  define malloc_msize(p)             _msize(DEBUG_NEW_CONST_CAST(void*)(p))
# elif DEBUG_NEW_CONFIG_HAVE_APPLE_MALLOC_SIZE
#  define malloc_msize(p)             malloc_size(DEBUG_NEW_CONST_CAST(void*)(p))
# endif
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  define malloc_msize_nn             malloc_msize
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
#endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_MSIZE */
#if DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC
# define aligned_malloc                                 DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC
# define aligned_calloc                                 DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC
# define aligned_realloc                                DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC
# ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE
#  define aligned_malloc_msize                          DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE
# endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE */
# define aligned_offset_malloc                          DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC
# define aligned_offset_calloc                          DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC
# define aligned_offset_realloc                         DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC
# ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE
#  define aligned_offset_malloc_msize                   DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE
# endif /* DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE */
# define aligned_free                                   DEBUG_NEW_API_NATIVE_ALIGNED_FREE
# if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#  define aligned_malloc_nz                             DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ
#  define aligned_calloc_nz                             DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ
#  define aligned_realloc_nz                            DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ
#  define aligned_realloc_nn                            DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN
#  define aligned_realloc_nnz                           DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ
#  ifdef DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#   define aligned_malloc_msize_nn                      DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN
#  endif /* DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_MSIZE_NN */
#  define aligned_offset_malloc_nz                      DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ
#  define aligned_offset_calloc_nz                      DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ
#  define aligned_offset_realloc_nz                     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ
#  define aligned_offset_realloc_nn                     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN
#  define aligned_offset_realloc_nnz                    DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ
#  ifdef DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#   define aligned_offset_malloc_msize_nn               DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN
#  endif /* DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_MSIZE_NN */
#  define aligned_free_nn                               DEBUG_NEW_API_NATIVE_ALIGNED_FREE_NN
# endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# if DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0
#  define aligned_mallocf(s,a,...)                      DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a)
#  define aligned_callocf(s,a,...)                      DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC(s,a)
#  define aligned_reallocf(p,s,a,...)                   DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC(p,s,a)
#  define aligned_offset_mallocf(s,a,o,...)             DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o)
#  define aligned_offset_callocf(s,a,o,...)             DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC(s,a,o)
#  define aligned_offset_reallocf(p,s,a,o,...)          DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC(p,s,a,o)
#  define valigned_mallocf(s,a,fmt,args)                DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC(s,a)
#  define valigned_callocf(s,a,fmt,args)                DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC(s,a)
#  define valigned_reallocf(p,s,a,fmt,args)             DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC(p,s,a)
#  define valigned_offset_mallocf(s,a,o,fmt,args)       DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC(s,a,o)
#  define valigned_offset_callocf(s,a,o,fmt,args)       DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC(s,a,o)
#  define valigned_offset_reallocf(p,s,a,o,fmt,args)    DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC(p,s,a,o)
#  if DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ
#   define aligned_mallocf_nz(s,a,...)                  DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a)
#   define aligned_callocf_nz(s,a,...)                  DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a)
#   define aligned_reallocf_nz(p,s,a,...)               DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ(p,s,a)
#   define aligned_reallocf_nn(p,s,a,...)               DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN(p,s,a)
#   define aligned_reallocf_nnz(p,s,a,...)              DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
#   define aligned_offset_mallocf_nz(s,a,o,...)         DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#   define aligned_offset_callocf_nz(s,a,o,...)         DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
#   define aligned_offset_reallocf_nz(p,s,a,o,...)      DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)
#   define aligned_offset_reallocf_nn(p,s,a,o,...)      DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)
#   define aligned_offset_reallocf_nnz(p,s,a,o,...)     DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
#   define valigned_mallocf_nz(s,a,fmt,args)            DEBUG_NEW_API_NATIVE_ALIGNED_MALLOC_NZ(s,a)
#   define valigned_callocf_nz(s,a,fmt,args)            DEBUG_NEW_API_NATIVE_ALIGNED_CALLOC_NZ(s,a)
#   define valigned_reallocf_nz(p,s,a,fmt,args)         DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NZ(p,s,a)
#   define valigned_reallocf_nn(p,s,a,fmt,args)         DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NN(p,s,a)
#   define valigned_reallocf_nnz(p,s,a,fmt,args)        DEBUG_NEW_API_NATIVE_ALIGNED_REALLOC_NNZ(p,s,a)
#   define valigned_offset_mallocf_nz(s,a,o,fmt,args)   DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_MALLOC_NZ(s,a,o)
#   define valigned_offset_callocf_nz(s,a,o,fmt,args)   DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_CALLOC_NZ(s,a,o)
#   define valigned_offset_reallocf_nz(p,s,a,o,fmt,args) DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NZ(p,s,a,o)
#   define valigned_offset_reallocf_nn(p,s,a,o,fmt,args) DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NN(p,s,a,o)
#   define valigned_offset_reallocf_nnz(p,s,a,o,fmt,args) DEBUG_NEW_API_NATIVE_ALIGNED_OFFSET_REALLOC_NNZ(p,s,a,o)
#  endif /* DEBUG_NEW_CONFIG_HAVE_MALLOC_NZ */
# endif /* DEBUG_NEW_CONFIG_HAVE_FMT_ALLOC != 0 */
#endif /* DEBUG_NEW_CONFIG_HAVE_ALIGNED_MALLOC */
#endif /* DEBUG_NEW_CONFIG_LINK_EXTENSIONS_WHEN_DISABLED */
#else /* GUARD_DEBUG_NEW_H */
# define DEBUG_NEW_PRIVATE_DISABLED 1
#endif /* !GUARD_DEBUG_NEW_H */
#elif DEBUG_NEW_PRIVATE_DISABLED == 1
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 2
#elif DEBUG_NEW_PRIVATE_DISABLED == 2
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 3
#elif DEBUG_NEW_PRIVATE_DISABLED == 3
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 4
#elif DEBUG_NEW_PRIVATE_DISABLED == 4
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 5
#elif DEBUG_NEW_PRIVATE_DISABLED == 5
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 6
#elif DEBUG_NEW_PRIVATE_DISABLED == 6
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 7
#elif DEBUG_NEW_PRIVATE_DISABLED == 7
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 8
#elif DEBUG_NEW_PRIVATE_DISABLED == 8
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 9
#elif DEBUG_NEW_PRIVATE_DISABLED == 9
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 10
#elif DEBUG_NEW_PRIVATE_DISABLED == 10
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 11
#elif DEBUG_NEW_PRIVATE_DISABLED == 11
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 12
#elif DEBUG_NEW_PRIVATE_DISABLED == 12
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 13
#elif DEBUG_NEW_PRIVATE_DISABLED == 13
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 14
#elif DEBUG_NEW_PRIVATE_DISABLED == 14
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 15
#elif DEBUG_NEW_PRIVATE_DISABLED == 15
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 16
#elif DEBUG_NEW_PRIVATE_DISABLED == 16
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 17
#elif DEBUG_NEW_PRIVATE_DISABLED == 17
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 18
#elif DEBUG_NEW_PRIVATE_DISABLED == 18
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 19
#elif DEBUG_NEW_PRIVATE_DISABLED == 19
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 20
#elif DEBUG_NEW_PRIVATE_DISABLED == 20
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 21
#elif DEBUG_NEW_PRIVATE_DISABLED == 21
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 22
#elif DEBUG_NEW_PRIVATE_DISABLED == 22
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 23
#elif DEBUG_NEW_PRIVATE_DISABLED == 23
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 24
#elif DEBUG_NEW_PRIVATE_DISABLED == 24
# undef DEBUG_NEW_PRIVATE_DISABLED
# define DEBUG_NEW_PRIVATE_DISABLED 25
#else
# error Too high recursion in debug_new enable/disable               \
  - Check for possible infinite-recursion, when including files      \
  - Reduce the complexity of you code                                \
  - Make sure every inclusion of <debug_new_disable.inl> is followed \
    by an inclusion of <debug_new_enable.inl>                        \
  - Increase the "max_recursion" variable in "__debug_new_enable_disable_gen.py"
#endif /* DEBUG_NEW_PRIVATE_DISABLED */
