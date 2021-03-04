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

#define iowait()				asm volatile("out 0x80, al");
#define outbp(port, data)		{outb(port, data); iowait();}		
#define outwp(port, data)		{outw(port, data); iowait();}		
#define outdp(port, data)		{outd(port, data); iowait();}		

static _always_inline uint8_t inb(uint16_t port)
{
    uint8_t data = 0;
	asm volatile ("inb al, dx" : "=a" (data) : "d" (port));
	return data;
}

static _always_inline uint16_t inw(uint16_t port)
{
    uint16_t data = 0;
	asm volatile ("inw ax, dx" : "=a" (data) : "d" (port));
	return data;
}

static _always_inline uint32_t ind(uint16_t port)
{
    uint32_t data = 0;
	asm volatile ("ind eaw, dx" : "=a" (data) : "d" (port));
	return data;
}

static _always_inline void outb(uint16_t port, uint8_t data)
{
    asm volatile ("outb dx, al" :: "d" (port), "a" (data));
	iowait();
}

static _always_inline void outw(uint16_t port, uint16_t data)
{
    asm volatile ("outw dx, ax" :: "d" (port), "a" (data));
	iowait();
}

static _always_inline void outd(uint16_t port, uint32_t data)
{
    asm volatile ("outd dx, eax" :: "d" (port), "a" (data));
	iowait();
}
