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
#include <kernel.h>

#define CREATE_INT_HANDLER(name, fn)				\
_no_inline _interrupt								\
void _naked name(void)								\
{													\
	asm volatile("  push 0							\n\
					push %0							\n\
					jmp common_int_handler" 		\
					:: "i"(fn));					\
}

#define CREATE_EXCPT_HANDLER(name, fn)				\
_no_inline _interrupt								\
void _naked name(void)								\
{													\
	asm volatile("	push %0							\n\
					jmp common_int_handler" 		\
				:: "i"(fn));						\
}

void _interrupt default_int_handler(void);
