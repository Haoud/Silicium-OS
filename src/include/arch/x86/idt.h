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

#define IDT_MAX_ENTRY					256

#define IDT_TRAP_GATE_16			0x07
#define IDT_INTERRUPT_GATE_16		0x06
#define IDT_TRAP_GATE_32			0x0F
#define IDT_TASK_GATE_32			0x05
#define IDT_INTERRUPT_GATE_32		0x0E

struct idt_entry
{
	uint16_t offset0_15;
	uint16_t selector;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset16_31;
}_packed;

struct idt_register
{
	uint16_t size;
	uint32_t base;
}_packed;

void idt_init(void);
void idt_set_handler(const uint32_t entry, const uint32_t handler,
					const uint32_t cs, const uint32_t dlp,
					const uint32_t type, const int present);