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

#define TSS_GDT_ENTRY		0x08
#define TSS_SELECTOR		0x40

struct tss
{
	uint16_t reserved_link, link;
	uint32_t esp0;
	uint16_t ss0, ___ss0_reserved;
	uint32_t esp1;
	uint16_t ss1, ___ss1_reserved;
	uint32_t esp2;
	uint16_t ss2, ___ss2_reserved;

	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	uint16_t es, ___es_reserved;
	uint16_t cs, ___cs_reserved;
	uint16_t ss, ___ss_reserved;
	uint16_t ds, ___ds_reserved;
	uint16_t fs, ___fs_reserved;
	uint16_t gs, ___gs_reserved;
	uint16_t ldt, ___ldt_reserved;
	uint32_t debug, iopb;
};

void tss_install(void);
void tss_set_kernel_stack(ptr_t stack);