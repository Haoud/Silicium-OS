/**
 * Copyright (C) 2021 Romain CADILHAC
 *
 * This file is part of Silicium OS.
 *
 * Silicium OS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Silicium OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Silicium OS.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <stdint.h>
#include <stddef.h>

#define KERNEL_VBASE		0xC0000000		// TODO: Le déplacer hors de ce fichier

// FIXME: Terminer les assertions
#ifdef _DEBUG
#define assert(cond)										\
if(!(cond)){												\
	printk("[STOP] Assertion %s failed \n", #cond);			\
	for(;;);												\
}
#else
#define assert(cond)
#endif

typedef uint32_t native_t;
typedef uint32_t atomic_t;
typedef native_t ptr_t;

# define _always_inline    inline __attribute__ ((__always_inline__))
#define _no_optimizations	__attribute__((optimize("-O0")))
#define _fallthrough		__attribute__ ((fallthrough))
#define _deprecated		    __attribute__((deprecated))
#define _no_inline			__attribute__((noinline))
#define _noreturn	        __attribute__((noreturn))
#define _malloc         	__attribute__((malloc))
#define _packed		        __attribute__((packed))
#define _unused		        __attribute__((unused))
#define _naked		        __attribute__((naked))
#define _pure		        __attribute__((pure))
#define _used               __attribute__((used))
#define _cdecl              __attribute__((cdecl))

#define _align(al)			__attribute__((aligned(al)))
#define _section(name)      __attribute__((section(name)))
#define _unreachable()		__builtin_unreachable()

#define _init					__attribute__((section(".init.text")))
#define _initdata				__attribute__((section(".init.data")))
#define _initrodata				__attribute__((section(".init.rotdata")))

#define _irq                        _cdecl _asmlinkage
#define _asmlinkage                 __attribute__((regparm(0)))
#define likely(expr)		        __builtin_expect(!!(expr), 1)
#define unlikely(expr)		        __builtin_expect(!!(expr), 0)
#define assume_aligned(ptr, al)     __builtin_assume_aligned(ptr, al)

extern char _end;

#define container_of(ptr, type, member)      ((type *) ((char*)ptr - offsetof(type, member)))

void printk(char *fmt, ...);