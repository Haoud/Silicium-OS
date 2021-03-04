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
					pushad							\n\
					sub esp, 2						\n\
					pushw ds						\n\
					pushw es						\n\
					pushw fs						\n\
					pushw gs						\n\
					pushw ss						\n\
					mov ax, 0x10					\n\
					mov ds, ax						\n\
					mov es, ax						\n\
					mov fs, ax						\n\
					mov gs, ax						\n\
					mov ax, 0x18					\n\
					mov ss, ax						\n\
					push esp						\n\
					call %0							\n\
					call terminate_interrupt		\n\
					add esp,4						\n\
					popw ss							\n\
					popw gs							\n\
					popw fs							\n\
					popw es							\n\
					popw ds							\n\
					add esp, 2						\n\
					popad							\n\
					add esp, 4						\n\
					iretd" :: "i"(fn));				\
}

#define CREATE_EXCPT_HANDLER(name, fn)				\
_no_inline _interrupt								\
void _naked name(void)								\
{													\
	asm volatile("	pushad							\n\
					sub esp, 2						\n\
					pushw ds						\n\
					pushw es						\n\
					pushw fs						\n\
					pushw gs						\n\
					pushw ss						\n\
					mov ax, 0x10					\n\
					mov ds, ax						\n\
					mov es, ax						\n\
					mov fs, ax						\n\
					mov gs, ax						\n\
					mov ax, 0x18					\n\
					mov ss, ax						\n\
					push esp						\n\
					call %0							\n\
					call terminate_interrupt		\n\
					add esp,4						\n\
					popw ss							\n\
					popw gs							\n\
					popw fs							\n\
					popw es							\n\
					popw ds							\n\
					add esp, 2						\n\
					popad							\n\
					add esp, 4						\n\
					iretd" :: "i"(fn));				\
}

void _interrupt default_int(void);
void _interrupt default_int_handler(void);
