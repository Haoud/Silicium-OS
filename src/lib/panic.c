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
#include <stdarg.h>
#include <lib/panic.h>
#include <lib/string.h>

void panic(char *fmt, ...)
{
	static char buffer[1024];
	va_list arg;

	va_start(arg, fmt);
	vsnprintf(buffer, 1024, fmt, arg);
	va_end(arg);

	printk("%s", buffer);

	/**
	 * @brief Ici, on arrête le processeur avec des instructions architectures
	 * dépendant.
	 * 
	 * TODO: Arrêter les autres processeurs
	 * 
	 */
	asm("1: cli; hlt; jmp 1l:");
	_unreachable();
}