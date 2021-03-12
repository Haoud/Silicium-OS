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
#include <grub.h>
#include <kernel.h>
#include <lib/list.h>
#include <arch/x86/paging.h>

#define FRAME_INDEX(addr)			((paddr_t)(addr) >> PAGE_SHIFT)
#define BIOS_LIMIT					0x00100000
#define DMA_LIMIT					0x01000000

#define FRAME_NORMAL		0x00
#define FRAME_BIOS			0x01
#define FRAME_DMA			0x02

#define get_dma_frame()			get_frame(FRAME_DMA)
#define get_bios_frame()		get_frame(FRAME_BIOS)
#define get_free_frame()		get_frame(FRAME_NORMAL)

struct page
{
	paddr_t base;
	atomic_t usage;
	struct list_head node;

	unsigned int dma : 1;
	unsigned int bios : 1;
	unsigned int reserved : 1;
};

vaddr_t get_end_address(void);

paddr_t get_frame(const int flags);
void use_frame(const paddr_t frame);
void free_frame(const paddr_t frame);
int frame_exist(const paddr_t frame);
int frame_reserved(const paddr_t frame);
void reserve_frame(const paddr_t frame);
atomic_t get_frame_usage(const paddr_t frame);
void page_frame_init(struct multiboot_info *minf);
