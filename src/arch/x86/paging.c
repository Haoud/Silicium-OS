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
#include <lib/memory.h>
#include <arch/x86/cpu.h>
#include <arch/x86/idt.h>
#include <arch/x86/paging.h>
#include <arch/x86/interrupt.h>
#include <arch/x86/exception.h>

void page_fault(cpu_kstate_t *state)
{
#define page_present	(state->error_code & 0x01)
#define write_access	(state->error_code & 0x02)
#define user_access		(state->error_code & 0x04)

	vaddr_t cr2;
	fetch_cr2(cr2);
	panic("Faute de page: %08x", cr2);
}
CREATE_EXCPT_HANDLER(page_fault_handler, page_fault)

void paging_init(void)
{
	set_exception(EXCEPTION_PAGE_FAULT_ID, (uint32_t)&page_fault_handler, 0);
}

/**
 * @brief Ici, on démappe les pages qui ne sont pas directement utilisées
 * par le noyau. Pour rappel, le code de démarrage mappe le noyau comme ceci:
 * 
 * 				0xC0000000-0xFFC00000 --> 0x00000000-0x3FC00000
 * 
 * Pour simplifier le code de démarrage et économiser de la mémoire, le code
 * utilise l'extension PSE permettant de mapper en mémoire des pages de 4MO,
 * ce qui complique un peu les choses lorsque l'on souhaite enlever ce mapping
 */
void paging_remove_identity(void)
{
	const vaddr_t end_addr = PAGE_SALIGN((vaddr_t)&_end);
	for(vaddr_t addr = end_addr + PDE_COVER_SIZE; 
				addr < PAGING_MIRORRING_ADDR;
				addr += PAGE_SIZE) {
		pde_clear(addr);
	}

	// TODO: Allouer une table de page
	uint32_t pt = 0;
	memset((void *)pt, 0, PAGE_SIZE);

	for(vaddr_t addr = end_addr & ~(PDE_COVER_SIZE - 1); 
				addr < end_addr;
				addr += PAGE_SIZE) {
		pte_t *pte = (pte_t *)(pt + pt_offset(addr) * 4);
		pte_set_address(pte, addr - PAGE_OFFSET);
		pte->present = 1;
		pte->write = 1;
	}

	/**
	 * On change l'entrée du répertoire de page , on peut dire merci au TLB 
	 * qui nous permet de modifier à la volée l'entrée du répertoire de page
	 * utilisé actuellement par le noyau
	 */
	pde_t *pde = pd_entry(end_addr);
	pde_set_address(pde, paging_get_physical(pt));
	pde->pse = 0;

	flush_tlb();
}

paddr_t paging_get_physical(const vaddr_t addr)
{
	pde_t *pde = pd_entry(addr);
	if(!pde->present) return 0;
	
	pte_t *pte = pt_entry(addr);
	if(!pte->present) return 0;

	return pte_get_address(pte);
}

void paging_unmap(const vaddr_t vaddr)
{
	pde_t *pde = pd_entry(vaddr);
	pte_t *pte = pt_entry(vaddr);

	if(!pde->present || !pte->present)
		return;

	const paddr_t ptaddr = pde_get_address(pde);
	const paddr_t paddr = pte_get_address(pte);
	memset(&pte, 0, sizeof(pte_t));

	// TODO: Gérer la page physique et éventuellement la table de pages
	invlpg(vaddr);
}

void paging_map_page(const vaddr_t vaddr, const vaddr_t paddr, int rights)
{
	pde_t *pde = pd_entry(vaddr);
	pte_t *pte = pt_entry(vaddr);

	if(!pde->present) {
		// Allouer la table de page
		const paddr_t page = 0;			// TODO: Allouer une page physique

		memset(&pde, 0, sizeof(pde_t));

		pde->user = 1;
		pde->write = 1;
		pde->present = 1;
		pde_set_address(pde, page);

		memset(pt_entry(vaddr & 0xFFC00000), 0, PAGE_SIZE);
		invlpg(pte);
	}
	else if(!pte->present) {
		// Référencer la table de page
	}
	else {
		// Page déjà utilisé, on arrête tout
		panic("Mapping d'une page à l'adresse %08x déjà utilisé", vaddr);
	}

	memset(&pte, 0, sizeof(pte_t));

	if(rights & MAP_WRITE) {
		pte->write = 1;
	}
	if(rights & MAP_USER) {
		pte->user = 1;
	}
	
	pte->present = 1;
	pte_set_address(pte, paddr);
	invlpg(vaddr);
}