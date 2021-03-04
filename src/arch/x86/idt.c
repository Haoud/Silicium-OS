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
#include <arch/x86/idt.h>
#include <arch/x86/interrupt.h>

static struct idt_entry idt[IDT_MAX_ENTRY];
static struct idt_register idtr;

/**
 * @brief Cette fonction permet d'initialiser toutes les interuptions
 * avec un gestionnaire par défaut. Cette fonction est très simple et 
 * n'initialise pas correctement les exceptions et qui doivent être
 * initialisées à part.
 */
void idt_init(void)
{
	for(int i = 0; i < IDT_MAX_ENTRY; i++)
		idt_set_handler(i, (uint32_t)(default_int_handler), GDT_KCODE_SELECTOR, 0, IDT_INTERRUPT_GATE_32, 1);

    idtr.size = sizeof(struct idt_entry) * IDT_MAX_ENTRY;
	idtr.base = (uint32_t)(idt);

    asm volatile("lidt %0" :: "m"(idtr));
}

/**
 * @brief Cette fonction permet d'initialiser un descripteur 
 * d'interruption
 * 
 * @param entry Numéro de l'interruption à modifier
 * @param handler Adresse virtuelle de la fonction qui sera éxecutée 
 * lorsque l'interruption surviendrta
 * @param cs Le sélecteur de code qui sera automatiquement assigné à
 * cs avant l'appel de la fonction
 * @param dlp Niveau de privilège minimum que le descripteur de code
 * appelant doit avoir
 * @param type Type d'interruption
 * @param present Le descripteur d'interruption est t-il considéré
 * comme présent ou non ?
 */
void idt_set_handler(const uint32_t entry, const uint32_t handler,
					const uint32_t cs, const uint32_t dlp,
					const uint32_t type, const int present)
{
	assert(dlp <= 3);
	assert(entry < IDT_MAX_ENTRY);

	idt[entry].reserved = 0;
	idt[entry].selector = (uint16_t)(cs);
	idt[entry].offset0_15 = (handler & 0xFFFF);
	idt[entry].offset16_31 = ((handler >> 16) & 0xFFFF);
	idt[entry].flags = ((dlp & 3) << 5) | (uint8_t)(type) | ((present) ? 0x80 : 0x00);
}