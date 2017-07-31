/* Copyright (c) 2016 Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                               *
 * This software is provided 'as-is', without any express or implied             *
 * warranty. In no event will the authors be held liable for any damages         *
 * arising from the use of this software.                                        *
 *                                                                               *
 * Permission is granted to anyone to use this software for any purpose,         *
 * including commercial applications, and to alter it and redistribute it        *
 * freely, subject to the following restrictions:                                *
 *                                                                               *
 * 1. The origin of this software must not be misrepresented; you must not       *
 *    claim that you wrote the original software. If you use this software       *
 *    in a product, an acknowledgement in the product documentation would be     *
 *    appreciated but is not required.                                           *
 * 2. Altered source versions must be plainly marked as such, and must not be    *
 *    misrepresented as being the original software.                             *
 * 3. This notice may not be removed or altered from any source distribution.    *
 */
// NOTE:
//  THIS FILE IF NOT REQUIRED, FOR DEBUG_NEW TO WORK
//  IT IS ONLY A TESTING SUIT, TO DEBUG DEBUG_NEW ITSELF
 
#ifndef __cplusplus
#define DEBUG_NEW_MAIN_HOOK 1
#endif /* !__cplusplus */
//#define DEBUG_NEW_CONFIG_LINK_STATICALLY     1
#define DEBUG_NEW_CONFIG_HAVE_AUTO_INIT      1
//#define DEBUG_NEW_CONFIG_HAVE_OVERWRITE_APIS 1

// ** test inclusion of debug_new, while it is disabled
#include <debug_new_disable.inl>
#include <debug_new.h>
#ifndef DEBUG_NEW_PRIVATE_DISABLED
# error Inclusion of <debug_new.h> enabled debug_new
#endif /* DEBUG_NEW_PRIVATE_DISABLED */
#include <debug_new_enable.inl>

#ifdef DEBUG_NEW_PRIVATE_DISABLED
# error Loaded disabled version of debug_new
#endif /* DEBUG_NEW_PRIVATE_DISABLED */

#if !defined(DEBUG_NEW_CONFIG_ENABLED) || !DEBUG_NEW_CONFIG_ENABLED
//# error "DEBUG_NEW_CONFIG_ENABLED" must be enabled, for testing
#endif /* !DEBUG_NEW_CONFIG_ENABLED */

#include <debug_new_disable.inl>
#ifndef DEBUG_NEW_PRIVATE_DISABLED
# error "Debug new isn't disabled"
#endif /* DEBUG_NEW_PRIVATE_DISABLED */
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <debug_new_enable.inl>
#ifdef DEBUG_NEW_PRIVATE_DISABLED
# error "Debug new is still disabled"
#endif /* DEBUG_NEW_PRIVATE_DISABLED */

static int failed_assertions = 0;
#define assert(cond) (DN_ASSERT(cond)?1:(++failed_assertions,0))

void test_detect_leak(void) {
 void *p;
 assert(mstate_push());
 p = malloc(10); // Supposed, to be a leak!
 assert(mstate_pop() == 1);
#if !DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS
 free(p);
#endif /* !DEBUG_NEW_CONFIG_HAVE_FIX_LEAKS */
}
void test_detect_free(void) {
 void *p = malloc(10); // Supposed, to be a free!
 assert(mstate_push());
 free(p);
 assert(mstate_pop() == 1);
}

void test_correct_code(void) {
 void *a,*b;
 assert(mstate_push());
 a = malloc(10);
 assert(mstate_push());
 b = malloc(20);
 free(b);
 assert(mstate_pop() == 0);
 free(a);
 assert(mstate_pop() == 0);
}

void test_thread_state_push_pop_with_alloc(void) {
#define DEBUG_NEW_TEST_SIZE 512
 void *allocs[DEBUG_NEW_TEST_SIZE];
 unsigned int i;
 for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
  assert(mstate_push());
  allocs[i] = malloc(i*123);
 }
 i = DEBUG_NEW_TEST_SIZE;
 while (--i) {
  free(allocs[i]);
  assert(mstate_pop() == 0);
 }
#undef DEBUG_NEW_TEST_SIZE
}

void test_random_dealloc_order(void) {
#define DEBUG_NEW_TEST_SIZE_2 64
#define DEBUG_NEW_TEST_SIZE (\
 DEBUG_NEW_TEST_SIZE_2 > RAND_MAX-1 ? RAND_MAX-1 : DEBUG_NEW_TEST_SIZE_2)
 void *allocs[DEBUG_NEW_TEST_SIZE];
 unsigned int i,n_dealloc;
 DEBUG_NEW_API_PRINTF("Before mstate_push()\n");
 assert(mstate_push());
 DEBUG_NEW_API_PRINTF("After Before mstate_push()\n");
 n_dealloc=0;
 for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
  DEBUG_NEW_API_PRINTF("malloc::done: %u/%u\n",
   DEBUG_NEW_STATIC_CAST(unsigned int)(i),
   DEBUG_NEW_STATIC_CAST(unsigned int)(DEBUG_NEW_TEST_SIZE));
  allocs[i] = malloc((i+1)*123);
 }
 i = DEBUG_NEW_TEST_SIZE;
 srand(DEBUG_NEW_STATIC_CAST(unsigned int)(time(NULL)));
 do {
  int rnd = rand();
  if (rnd >= 0) {
   rnd %= DEBUG_NEW_TEST_SIZE;
   if (allocs[rnd]) {
    free(allocs[rnd]);
    allocs[rnd] = NULL;
    ++n_dealloc;
    DEBUG_NEW_API_PRINTF("free::done: %u/%u\n",
     DEBUG_NEW_STATIC_CAST(unsigned int)(n_dealloc),
     DEBUG_NEW_STATIC_CAST(unsigned int)(DEBUG_NEW_TEST_SIZE));
   }
  }
 } while (n_dealloc != DEBUG_NEW_TEST_SIZE);
 assert(mstate_pop() == 0);
#undef DEBUG_NEW_TEST_SIZE
}



void test_push_pop_print_funcs(void) {
#if DEBUG_NEW_CONFIG_ENABLED
 unsigned int i;
 DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_PrintFunc_t default_print_func = 
  DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_GetDefaultPrintFunc();
 assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 0);
 assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 1);
 assert(DEBUG_NEW_API_PRINTFUNC_POP());
 assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 1);
  assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 2);
   assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
    assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 3);
   assert(DEBUG_NEW_API_PRINTFUNC_POP());
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 2);
  assert(DEBUG_NEW_API_PRINTFUNC_POP());
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 1);
  assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 2);
  assert(DEBUG_NEW_API_PRINTFUNC_POP());
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 1);
  assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 2);
   assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
    assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 3);
    assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
     assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 4);
     assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
      assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 5);
      assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
       assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 6);
      assert(DEBUG_NEW_API_PRINTFUNC_POP());
     assert(DEBUG_NEW_API_PRINTFUNC_POP());
    assert(DEBUG_NEW_API_PRINTFUNC_POP());
   assert(DEBUG_NEW_API_PRINTFUNC_POP());
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 2);
  assert(DEBUG_NEW_API_PRINTFUNC_POP());
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 1);
 assert(DEBUG_NEW_API_PRINTFUNC_POP());
 assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 0);
#define DEBUG_NEW_TEST_SIZE 256
 for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
  assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == i+1);
 }
 for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
  assert(DEBUG_NEW_API_PRINTFUNC_POP());
  assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == (DEBUG_NEW_TEST_SIZE-1)-i);
 }
 assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 0);
 assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
  for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
   assert(DEBUG_NEW_API_PRINTFUNC_PUSH(default_print_func));
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == i+2);
  }
  for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
   assert(DEBUG_NEW_API_PRINTFUNC_POP());
   assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == ((DEBUG_NEW_TEST_SIZE-1)-i)+1);
  }
 assert(DEBUG_NEW_API_PRINTFUNC_POP());
 assert(DEBUG_NEW_API_PRINTFUNC_STACK_SIZE() == 0);
#undef DEBUG_NEW_TEST_SIZE
#endif /* DEBUG_NEW_CONFIG_ENABLED */
}

void test_adv_alloc(void) {
#define DEBUG_NEW_TEST_SIZE 1024
 void *allocs[DEBUG_NEW_TEST_SIZE];
 unsigned int i;
 for (i = 0; i < DEBUG_NEW_TEST_SIZE; ++i) {
  assert(mstate_push());
  allocs[i] = mallocf(i*123,"allocs[%d]",i);
 }
 //DEBUG_NEW_CXX_NAMESPACE_ROOT DnDump_MostGlobalAllocs(0,1);
 i = DEBUG_NEW_TEST_SIZE;
 while (--i) {
  free(allocs[i]);
  assert(mstate_pop() == 0);
 }
#undef DEBUG_NEW_TEST_SIZE
}


#ifdef __cplusplus
static int nallocs = 0;
struct test_struct {
 public:
  test_struct(void)  { ++nallocs; }
  ~test_struct(void) { --nallocs; }
};
void test_operator_new(void) {
 assert(mstate_push());
 nallocs = 0;
 test_struct *p1 = new test_struct[20];
 assert(nallocs == 20);
 delete[] p1;
 assert(nallocs == 0);
 test_struct *p2 = new test_struct[20];
 test_struct *p3 = new test_struct[10];
 assert(nallocs == 30);
 delete[] p2;
 assert(nallocs == 10);
 delete[] p3;
 assert(mstate_pop() == 0);
}

void test_placement_new(void) {
 nallocs = 0;
 char memory[sizeof(test_struct)];
 placement new(memory) test_struct();
 assert(nallocs == 1);
 DEBUG_NEW_REINTERPRET_CAST(test_struct *)(memory)->~test_struct();
 assert(nallocs == 0);
}

#endif

void test_debug_new_disabled(void) {
 void *leak;
 mstate_push();
#include <debug_new_disable.inl>
 leak = malloc(42);
#include <debug_new_enable.inl>
 assert(mstate_pop() == 0); // make sure leaks go undetected, while debug_new is disabled
 // test disable / enable recursion
#include <debug_new_disable.inl>
#include <debug_new_disable.inl>
#include <debug_new_disable.inl>
#include <debug_new_enable.inl>
#include <debug_new_enable.inl>
 // objects allocated, while debug_new is disabled must also be freed,
 // without debug_new, as they won't have debug-records associated with them.
 free(leak);
#include <debug_new_enable.inl>
}
#ifdef DEBUG_NEW_PRIVATE_DISABLED
#error Debug new is still disabled
#endif /* DEBUG_NEW_PRIVATE_DISABLED */

#ifdef malloc_msize
void test_debug_new_malloc_msize(void) {
 void *p = malloc(10);
 size_t s = malloc_msize(p);
 assert(s == 10);
 free(p);
}
#endif /* malloc_msize */


#define is_aligned(p,a) ((uintptr_t)(p)%(a) == 0)


void test_aligned_malloc(void) {
 size_t align = 64;
 void *p = aligned_malloc(42,align);
 if (p) {
  void *p2;
  assert(is_aligned(p,align));
#ifdef aligned_malloc_msize_nn
  assert(aligned_malloc_msize_nn(p,align) == 42);
#endif /* aligned_malloc_msize_nn */
  p2 = aligned_realloc_nnz(p,84,align);
  if (p2) {
   assert(is_aligned(p,align));
#ifdef aligned_malloc_msize_nn
   assert(aligned_malloc_msize_nn(p2,align) == 84);
#endif /* aligned_malloc_msize_nn */
   aligned_free_nn(p2);
  } else aligned_free_nn(p);
 }
}


#define DEBU_NEW_TEST_EXEC(NAME) (\
 DEBUG_NEW_API_PRINT("Executing test \"" #NAME "\" ...\n"),NAME())\

#include "stdio.h"

int main(int argc, char* argv[]) 
{
// DEBUG_NEW_UNREFERENCED(argc);
// DEBUG_NEW_UNREFERENCED(argv);
// DEBU_NEW_TEST_EXEC(test_detect_leak);
// DEBU_NEW_TEST_EXEC(test_detect_free);
// DEBU_NEW_TEST_EXEC(test_correct_code);
// DEBU_NEW_TEST_EXEC(test_thread_state_push_pop_with_alloc);
// DEBU_NEW_TEST_EXEC(test_random_dealloc_order);
// DEBU_NEW_TEST_EXEC(test_push_pop_print_funcs);
// DEBU_NEW_TEST_EXEC(test_adv_alloc);
//#ifdef __cplusplus
// DEBU_NEW_TEST_EXEC(test_operator_new);
// DEBU_NEW_TEST_EXEC(test_placement_new);
//#endif // __cplusplus
// DEBU_NEW_TEST_EXEC(test_debug_new_disabled);
//#ifdef malloc_msize
// DEBU_NEW_TEST_EXEC(test_debug_new_malloc_msize);
//#endif /* malloc_msize */
// DEBU_NEW_TEST_EXEC(test_aligned_malloc);
//
// if (failed_assertions != 0) {
//  DEBUG_NEW_API_PRINTF("\n\n\n"
//   "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
//   "!! FIXME:  %.3d assertions failed    !!\n"
//   "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",
//   failed_assertions);
// }
// 
//#ifdef __cplusplus
// DEBUG_NEW_API_PRINT("-- Tests executed in c++\n");
//#else
// DEBUG_NEW_API_PRINT("-- Tests executed in c\n");
//#endif
//
 unsigned char* buffer = (unsigned char*)malloc(55);


 return 0;
}

#ifdef _MSC_VER
#pragma warning(disable:4710) /*< 'xxx' : function not inlined */
#pragma warning(disable:4505) /*< 'xxx' : unreferenced local function has been removed */
#pragma warning(disable:4514) /*< 'xxx' : unreferenced inline function has been removed */
#endif

