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
#include <grub.h>
#include <kernel.h>
#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <driver/vga/vga.h>

/**
 * @brief Cette fonction est appelé par le point d'entrée (codé en assembleur). Cette
 * fonction se charge d'initialiser le matériel et passe ensuite la main à une fonction
 * plus générique qui va initialiser le noyau
 * 
 * @note Le processeur minimum supporté est le Pentium Pro (i686). Toute tentative de 
 * boot à partir d'une processeur moins récent se terminera rapidement par un échec car
 * le point d'entrée à besoin de l'extension PSE de CR4 pour initialiser la pagination
 * 
 */
_asmlinkage void boot_x86(struct multiboot_info *info)
{
	vga_init();
	printk("Booting from Silicium OS...\n");

	info = multiboot_translate_ptr(info);
	multiboot_translate_addr((ptr_t *)(&info->mmap_addr));

	gdt_init();
	idt_init();
	for(;;);
}