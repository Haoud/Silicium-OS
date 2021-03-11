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

#define PAGING_MIRORRING_ADDR			0xFFC00000
#define PAGING_ENTRY_PER_PD				1024
#define PAGING_ENTRY_PER_PT				1024

#define PAGE_SIZE						4096
#define PAGE_MASK						(~(PAGE_SIZE - 1))
#define PAGE_SHIFT						12
#define PAGE_OFFSET						0xC0000000
#define PDE_COVER_SIZE					(PAGE_SIZE*PAGE_SIZE)
#define PAGE_ALIGN(addr)				((addr) & PAGE_MASK)
#define PAGE_SALIGN(addr)				(((addr) + PAGE_SIZE) & PAGE_MASK)
#define PAGE_ALIGNED(addr)				(!((addr) & (PAGE_SIZE - 1)))

#define MAP_ATOMIC						0x01
#define MAP_READ						0x02
#define MAP_WRITE						0x04
#define MAP_EXECUTE						0x08
#define MAP_USER						0x10

#define fetch_cr2(x)					asm volatile("mov cr2, %0" : "=r"(x))
#define set_current_pd(pd)				asm volatile("mov cr3, %0" :: "r"(pd) : "memory")
#define invlpg(pg)						asm volatile("invlpg [%0]" :: "r"(pg) : "memory")

#define pde_set_address(pde, vaddr)		pde->address = ((vaddr) >> PAGE_SHIFT)
#define pte_set_address(pte, vaddr)		pte->address = ((vaddr) >> PAGE_SHIFT)
#define pde_get_address(pde)			(pde->address << PAGE_SHIFT)
#define pte_get_address(pte)			(pte->address << PAGE_SHIFT)
#define pde_clear(vaddr)				(*(uint32_t *)pd_entry(vaddr)) = 0
#define pte_clear(vaddr)				(*(uint32_t *)pt_entry(vaddr)) = 0

#define pd_offset(vaddr)				(((vaddr) & 0xFFC00000) >> 22)
#define pt_offset(vaddr)				(((vaddr) & 0x003FF000) >> 12)
#define pd_entry(vaddr)					__pd_entry(vaddr)
#define pt_entry(vaddr)					__pt_entry(vaddr)

typedef uint32_t vaddr_t;
typedef uint32_t paddr_t;

struct pde
{
	uint32_t present : 1;
	uint32_t write : 1;
	uint32_t user : 1;
	uint32_t write_throught : 1;
	uint32_t disable_cache : 1;
	uint32_t accessed : 1;
	uint32_t zero : 1;
	uint32_t pse : 1;
	uint32_t ignored : 1;
	uint32_t avaible : 3;
	uint32_t address : 20;
}_packed;

struct pte
{
	uint32_t present : 1;
	uint32_t write : 1;
	uint32_t user : 1;
	uint32_t write_throught : 1;
	uint32_t disable_cache : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t zero : 1;
	uint32_t global : 1;
	uint32_t avaible : 3;
	uint32_t address : 20;
}_packed;

typedef struct pde pde_t;
typedef struct pte pte_t;

_always_inline pde_t *__pd_entry(vaddr_t addr)
{
	return (pde_t *) (PAGING_MIRORRING_ADDR + (pd_offset(PAGING_MIRORRING_ADDR) << PAGE_SHIFT) + pd_offset(addr) * 4);
}

_always_inline pte_t *__pt_entry(vaddr_t addr)
{
	return (pte_t *) (PAGING_MIRORRING_ADDR + (pd_offset(addr) << PAGE_SHIFT) + pd_offset(addr) * 4);
}

_always_inline void flush_tlb(void)
{
	asm volatile("	mov eax, cr3	\n\
					mov cr3, eax" ::: "eax", "memory");
}

void paging_init(void);
void paging_remove_identity(void);
void paging_unmap(const vaddr_t vaddr);
paddr_t paging_get_physical(const vaddr_t addr);
void paging_map_page(const vaddr_t vaddr, const vaddr_t paddr, int rights);
