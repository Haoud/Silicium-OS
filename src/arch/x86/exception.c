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
#include <lib/panic.h>
#include <arch/x86/cpu.h>
#include <arch/x86/idt.h>
#include <arch/x86/exception.h>
#include <arch/x86/interrupt.h>

_interrupt void default_exception(cpu_kstate_t *state)
{
	panic("Exception inconnue");
}

_interrupt void divide_by_zero(cpu_kstate_t *state)
{
	panic("Division par zéro");
}

_interrupt void invalid_op(cpu_kstate_t *state)
{
	panic("Code d'opération invalide");
}

_interrupt void double_fault(cpu_kstate_t *state)
{
	panic("Double faute");
}

_interrupt void invalid_tss(cpu_kstate_t *state)
{
	panic("TSS invalide");
}

_interrupt void stack_segment_fault(cpu_kstate_t *state)
{
	panic("Erreur de segment de pile");
}

_interrupt void segment_not_present(cpu_kstate_t *state)
{
	panic("Segment non présent");
}

_interrupt void general_protection_fault(cpu_kstate_t *state)
{
	panic("Faute de protection générale");
}

CREATE_INT_HANDLER(default_exception_handler, default_exception)
CREATE_INT_HANDLER(divide_by_zero_handler, divide_by_zero)
CREATE_INT_HANDLER(invalid_op_handler, invalid_op)
CREATE_INT_HANDLER(invalid_tss_handler, invalid_tss)
CREATE_INT_HANDLER(double_fault_handler, double_fault)
CREATE_INT_HANDLER(stack_segment_fault_handler, stack_segment_fault)
CREATE_INT_HANDLER(segment_not_present_handler, segment_not_present)
CREATE_INT_HANDLER(general_protection_fault_handler, general_protection_fault)

/**
 * @brief Cette fonction permet d'initialiser les gestionnaires
 * par défaut des différentes exceptions de l'architecture x86
 */
void exceptions_init(void)
{
	for(int i = 0; i < EXCEPTION_NUMBER; i++)
		set_exception(i, (uint32_t)(&default_exception_handler), 0);

	/* 
	 * D'autres exceptions sont gérées dans d'autres fichiers,
	 * comme les fautes de pages ou l'exception "device not avaible"
	 */
	set_exception(EXCEPTION_INVALID_TSS_ID, (uint32_t)(&invalid_tss_handler), 0);
	set_exception(EXCEPTION_DOUBLE_FAULT_ID, (uint32_t)(&double_fault_handler), 0);
	set_exception(EXCEPTION_INVALID_OPCODE_ID, (uint32_t)(&invalid_op_handler), 0);
	set_exception(EXCEPTION_DIVIDE_BY_ZERO_ID, (uint32_t)(&divide_by_zero_handler), 0);
	set_exception(EXCEPTION_SEGMENT_NOT_PRESENT_ID, (uint32_t)(&segment_not_present_handler), 0);
	set_exception(EXCEPTION_STACK_SEGMENT_FAULT_ID, (uint32_t)(&stack_segment_fault_handler), 0);
	set_exception(EXCEPTION_GENERAL_PROTECTION_FAULT_ID, (uint32_t)(&general_protection_fault_handler), 0);
}