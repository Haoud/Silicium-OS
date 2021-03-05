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
#include <lib/memory.h>
#include <arch/x86/gdt.h>
#include <arch/x86/tss.h>

#define TSS_GDT_ACCESS		GDT_ACCESSED | GDT_IS_CODE_SEGMENT | GDT_SEGMENT_PRESENT
#define TSS_GDT_FLAGS		GDT_SEGMENT_32BITS

static struct tss tss;

void tss_install(void)
{
	memset(&tss, 0, sizeof(struct tss));
	tss.iopb = sizeof(tss);

	gdt_set_entry(TSS_GDT_ENTRY, (uint32_t)(&tss), sizeof(struct tss), TSS_GDT_ACCESS, TSS_GDT_FLAGS, 1);
	asm volatile("ltr ax" :: "a"(TSS_SELECTOR));
}

void tss_set_kernel_stack(ptr_t stack)
{
	tss.ss0 = GDT_KSTACK_SELECTOR;
	tss.esp0 = stack;
}