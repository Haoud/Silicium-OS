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
#include <lib/log.h>
#include <arch/x86/cpu.h>
#include <arch/x86/interrupt.h>

_interrupt
void default_int(void)
{
	printk(KERN_CRIT "Interruption inconnue survenue\n");
}

_used _interrupt
void terminate_interrupt(cpu_kstate_t *state)
{

}

_used _naked
void common_int_handler()
{
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
					call interruption_manager		\n\
					jmp return_from_interrupt");
}

_used _interrupt
void interruption_manager(cpu_kstate_t *state)
{
	asm volatile("call %0" :: "m"(state->free_data));
}

_used _naked
void return_from_interrupt()
{
	asm volatile("	call terminate_interrupt		\n\
					add esp,4						\n\
					popw ss							\n\
					popw gs							\n\
					popw fs							\n\
					popw es							\n\
					popw ds							\n\
					add esp, 2						\n\
					popad							\n\
					add esp, 8						\n\
					iretd");
}

CREATE_INT_HANDLER(default_int_handler, default_int)