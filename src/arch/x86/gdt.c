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
#include <arch/x86/gdt.h>

static struct gdt_entry gdt[GDT_MAX_ENTRY];
static struct gdt_register gdtr;

/**
 * @brief Permet d'initialiser la GDT. Cette fonction créé la GDT qui contient les
 * 3 sélecteurs du noyau (code, données, pile) mais aussi les 3 sélecteurs utilisateurs
 * qui seront utilisés par les programmes.
 */
void gdt_init(void)
{
	gdt_set_entry(0, 0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, GDT_IS_CODE_SEGMENT | GDT_SEGMENT_PRESENT | GDT_RING0,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);
	gdt_set_entry(2, 0, 0xFFFFFFFF, GDT_SEGMENT_PRESENT | GDT_DATA_CAN_WRITE | GDT_RING0,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);
	gdt_set_entry(3, 0, 0x00000000, GDT_SEGMENT_PRESENT | GDT_DATA_CAN_WRITE | GDT_DATA_GROW_DOWN | GDT_RING0,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);
	gdt_set_entry(4, 0, 0xFFFFFFFF, GDT_IS_CODE_SEGMENT | GDT_SEGMENT_PRESENT | GDT_RING3,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);
	gdt_set_entry(5, 0, 0xFFFFFFFF, GDT_SEGMENT_PRESENT | GDT_DATA_CAN_WRITE | GDT_RING3,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);
	gdt_set_entry(6, 0, 0x00000000, GDT_SEGMENT_PRESENT | GDT_DATA_CAN_WRITE | GDT_DATA_GROW_DOWN | GDT_RING3,
									GDT_BLOCK_SIZE_4_KO | GDT_SEGMENT_32BITS, 0);

	gdtr.size = sizeof(struct gdt_entry) * GDT_MAX_ENTRY;
	gdtr.base = (uint32_t)(gdt);

	/* Charge la GDT dans le registre approprié, puis recharge les sélecteurs */
    asm volatile("lgdt %0" :: "m"(gdtr));
	asm volatile (" mov ax, %0		\n\
					mov ss, ax		\n\
					mov ax, %1		\n\
				 	mov ds, ax		\n\
				 	mov es, ax		\n\
				 	mov fs, ax		\n\
				 	mov gs, ax		\n\
				 	jmp %2:1f		\n\
				 	1:" :: 	"i"(GDT_KSTACK_SELECTOR),
					 		"i"(GDT_KDATA_SELECTOR),
							"i"(GDT_KCODE_SELECTOR) 
						: "eax");

}

/**
 * @brief Permet de définir une entrée de la GDT
 * 
 * @param entry L'entrée qui va être modifiées
 * @param base Adresse du début du segment
 * @param limit L'adresse maximal du segment
 * @param access Droits d'accès du segment
 * @param flags Un ensemble d'options du segment
 * @param tss L'entrée décrit-elle un TSS ou non (booléen) ?
 */
void gdt_set_entry(	const unsigned int entry, const uint32_t base,
					const uint32_t limit, const uint32_t access,
					const uint32_t flags, const int tss)
{
	assert(entry < GDT_MAX_ENTRY);
	gdt[entry].base0_15 = (base & 0xFFFF);
	gdt[entry].base16_23 = ((base >> 16) & 0xFF);
	gdt[entry].base24_31 = ((base >> 24) & 0xFF);

	gdt[entry].limit0_15 = (limit & 0xFFFF);
	gdt[entry].limit16_19 = ((limit >> 16) & 0x0F);

	gdt[entry].flags = (flags & 0x0F);
	gdt[entry].access = (tss) ? (access) : (access | 0x10);
}