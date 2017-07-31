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

#define DEBUG_NEW_CONFIG_LINK_STATICALLY     1
#include <debug_new.h>

// This file is used, to test the static linkage of debug_new
// >> When being compiled with "test.c", the linker will yield
//    error for declaration, that appear in both units and
//    should therefor be declared with static / inline

#ifdef _MSC_VER
#pragma warning(disable:4710) /*< 'xxx' : function not inlined */
#pragma warning(disable:4505) /*< 'xxx' : unreferenced local function has been removed */
#pragma warning(disable:4514) /*< 'xxx' : unreferenced inline function has been removed */
#endif



