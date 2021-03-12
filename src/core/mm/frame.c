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
#include <lib/spinlock.h>
#include <core/mm/frame.h>

#define FRAME_INFO(addr)		frames[FRAME_INDEX(addr)]
#define FRAME_ARRAY_END			((vaddr_t)(&frames[FRAME_INDEX(last_frame)]) + sizeof(struct page))

#define stat_frame_reserved()	{free_frames--;}
#define stat_frame_free()		{free_frames++;used_frames--;}
#define stat_frame_used()		{free_frames--;used_frames++;}

static spinlock_t lock;
static paddr_t last_frame;
static struct page *frames;
static struct list_head dma_pages;			// Les cadres de page DMA libres 
static struct list_head free_pages;			// Les cadres de page libres
static struct list_head bios_pages;			// Les cadres de page BIOS libre
static struct list_head used_pages;			// Tous les cadres de page utilisées

static unsigned int reserved_frames;		// Nombre de cadres de pages réservés
static unsigned int total_frames;			// Nombre total de cadres de page
static unsigned int free_frames;			// Nombre de cadres de page libres
static unsigned int used_frames;			// Nombre de cadres de page utilisés

static void release_frame(const paddr_t frame)
{
	spin_lock(&lock);
	list_remove(&FRAME_INFO(frame).node);
	if(frame < BIOS_LIMIT) {
		list_add(&bios_pages, &FRAME_INFO(frame).node);
	}
	else if(frame < DMA_LIMIT) {
		list_add(&dma_pages, &FRAME_INFO(frame).node);
	}
	else {
		list_add(&free_pages, &FRAME_INFO(frame).node);
	}
	spin_unlock(&lock);
	stat_frame_free();
}

vaddr_t get_end_address(void)
{
	return FRAME_ARRAY_END;
}

void use_frame(const paddr_t frame)
{
	if(frame_exist(frame) && !frame_reserved(frame)) {
		if(!FRAME_INFO(frame).usage)
			panic("Référencement d'un cadre de page libre (%08x)", frame);
		FRAME_INFO(frame).usage++;
	}
}

void free_frame(const paddr_t frame)
{
	if(frame_exist(frame) && !frame_reserved(frame)) {
		if(!FRAME_INFO(frame).usage--)
			panic("Déréférencement d'un cadre de page libre (%08x)", frame);
	
		if(!FRAME_INFO(frame).usage)
			release_frame(frame);
	}
}

int frame_exist(const paddr_t frame)
{
	return (frame > last_frame) ? 0 : 1;
}

int frame_reserved(const paddr_t frame)
{
	if(!frame_exist(frame))
		return 1;
	return (FRAME_INFO(frame).reserved) ? 1 : 0;
}

void reserve_frame(const paddr_t frame)
{
	if(FRAME_INFO(frame).usage) {
		panic("Impossible de réserver la page %08x, elle est déjà utilisé", frame);
	}

	spin_lock(&lock);
	list_remove(&FRAME_INFO(frame).node);
	FRAME_INFO(frame).reserved = 1;
	stat_frame_reserved();
	spin_unlock(&lock);
}

paddr_t get_frame(const int flags)
{
	struct list_head *pool = &free_pages;
	spin_lock(&lock);

	if(flags & FRAME_DMA) {
		if(list_empty(&dma_pages)) {	// TODO: Aller piocher dans les pages "BIOS"
			spin_unlock(&lock);
			return 0;
		}
		pool = &dma_pages;
	}
	else if(flags & FRAME_BIOS) {
		if(list_empty(&bios_pages)) {
			spin_unlock(&lock);
			return 0;
		}
		pool = &bios_pages;
	}

	if(unlikely(list_empty(pool))) {
		pool = &dma_pages;
		if(unlikely(list_empty(pool))) {
			pool = &bios_pages;
			if(unlikely(list_empty(pool))) {
				panic("Plus de mémoire physique restante");
			}
		}
	}

	struct page *page_info = container_of(pool->next, struct page, node);
	assert(!page_info->reserved);
	assert(!page_info->usage);
	page_info->usage = 1;
	stat_frame_used();

	list_remove(&page_info->node);
	list_add(&used_pages, &page_info->node);
	spin_unlock(&lock);

	return page_info->base;
}

atomic_t get_frame_usage(const paddr_t frame)
{
	if(!frame_exist(frame)) {
		return 0;
	}

	return FRAME_INFO(frame).usage;
}

void page_frame_init(struct multiboot_info *minf)
{
	spin_init(&lock);
	list_init(&dma_pages);
	list_init(&free_pages);
	list_init(&used_pages);
	list_init(&bios_pages);

	if(!(minf->flags & MULTIBOOT_INFO_MEM_MAP))
		panic("Aucun information sur l'organisation de la mémoire");

	/**
	 * @brief On place le tableau de pages apràs le noyau. Comment le premier Go de mémoire
	 * est mappé à l'indentique lors de l'appel à cette fonction, on peut accéder simplement
	 * à la mémoire physique.
	 *
	 * TODO: Placer le tableau à un autre endroit ? Si beaucoup de mémoire est disponible,
	 * aucune page de DMA (sauf celle en dessous de 1MO) ne sera disponible ! Et s'il y a
	 * le "trou ISA" ?
	 */
	frames = (struct page *) PAGE_SALIGN((vaddr_t)(&_end));
	total_frames = 0;
	free_frames = 0;
	used_frames = 0;

	const vaddr_t mmmap_addr = (vaddr_t)(minf->mmap_addr) & 0xFFFFFFFF;
	const uint32_t mmmap_length = minf->mmap_length & 0xFFFFFFFF;

	/**
	 * @brief Ici, on va obtenir la dernière addresse physique de disponible
	 * pour pouvoir initialiser correctement le tableau frames
	 */
	for(struct multiboot_mmap *mminf = (void *)mmmap_addr;
		(vaddr_t)(mminf) - mmmap_addr < mmmap_length;
		mminf = (void *)((vaddr_t)(mminf) + mminf->size + sizeof(mminf->size))) {
		if(mminf->addr > UINT32_MAX)
			continue;
		if(mminf->len > UINT32_MAX)
			continue;

		const paddr_t base = mminf->addr & 0xFFFFFFFF;
		const uint32_t length = mminf->len & 0xFFFFFFFF;
		if(base + length < base)
			continue;
		if(mminf->type & MULTIBOOT_MEMORY_AVAILABLE) {
			if(last_frame < base + length) {
				last_frame = base + length;
			}
		}
	}

	/**
	 * @brief Initialise tous les cadres de pages avec des valeurs par défaut
	 */
	for(paddr_t addr = 0; addr < last_frame; addr += PAGE_SIZE) {
		FRAME_INFO(addr).usage = 0;
		FRAME_INFO(addr).base = addr;
		FRAME_INFO(addr).reserved = 1;

		if(addr < BIOS_LIMIT) {
			FRAME_INFO(addr).bios = 1;
		}
		if(addr < DMA_LIMIT) {
			FRAME_INFO(addr).dma = 1;
		}
		list_init(&FRAME_INFO(addr).node);
	}

	for(struct multiboot_mmap *mminf = (void *)mmmap_addr;
		(vaddr_t)(mminf) - mmmap_addr < mmmap_length;
		mminf = (void *)((vaddr_t)(mminf) + mminf->size + sizeof(mminf->size))) {

		if(mminf->addr > UINT32_MAX)
			continue;
		if(mminf->len > UINT32_MAX)
			continue;

		const paddr_t base = mminf->addr & 0xFFFFFFFF;
		const uint32_t length = mminf->len & 0xFFFFFFFF;
		if(base + length < base)
			continue;

		/**
		 * @brief On va traiter chaque cadre de page de la zone mémoire décrite et
		 * l'ajouter dans une des listes si besoin
		 * 
		 */
		for(paddr_t addr = base; addr < base + length; addr += PAGE_SIZE) {
			total_frames++;

			if(mminf->type == MULTIBOOT_MEMORY_AVAILABLE) {
				free_frames++;
				FRAME_INFO(addr).reserved = 0;

				if(addr < BIOS_LIMIT) {
					list_add(&bios_pages, &FRAME_INFO(addr).node);
				}
				else if(addr < DMA_LIMIT) {
					list_add(&dma_pages, &FRAME_INFO(addr).node);
				}
				else {
					list_add(&free_pages, &FRAME_INFO(addr).node);
				}
			}
		}
	}

	reserve_frame(0);

	/**
	 * @brief On réserve les zones utilisées par le noyau. Il faudrait trouver
	 * le moyen de rendre cette partie plus élégante avec moins de valeurs
	 * hard-codées.
	 */
	for(paddr_t addr = KERNEL_PBASE; addr < FRAME_ARRAY_END - KERNEL_VBASE; addr += PAGE_SIZE) {
		if(frame_reserved(addr) || get_frame_usage(addr))
			continue;
		free_frames--;
		used_frames++;
		FRAME_INFO(addr).usage++;
		list_remove(&FRAME_INFO(addr).node);
		list_add(&used_pages, &FRAME_INFO(addr).node);
	}

	for(paddr_t addr = 0xA0000; addr < 0xEFFFF; addr += PAGE_SIZE) {
		if(frame_reserved(addr) || get_frame_usage(addr))
			continue;
		free_frames--;
		used_frames++;
		FRAME_INFO(addr).usage++;
		list_remove(&FRAME_INFO(addr).node);
		list_add(&used_pages, &FRAME_INFO(addr).node);
	}
}
