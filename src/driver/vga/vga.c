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
#include <arch/x86/io.h>
#include <lib/spinlock.h>
#include <driver/vga/vga.h>

static char *buffer;
static spinlock_t lock;
static uint8_t attribute;
static unsigned int max_x;
static unsigned int max_y;
static unsigned int cursor_x;
static unsigned int cursor_y;

void vga_init(void)
{
	spin_init(&lock);
	max_x = VGA_MAX_X;
	max_y = VGA_MAX_Y;
	cursor_x = cursor_y = 0;
	vga_set_attribute(VGA_COLOR7, VGA_COLOR0);
	buffer = (char *)(KERNEL_VBASE + VGA_BUFFER);
	
	vga_clear();
	vga_show_cursor(1);
}

void vga_clear(void)
{
	vga_clear_area(0, 0, max_x, max_y);
}

void vga_put(const char c)
{
	unsigned int pos = 0;
	if(cursor_y >= max_y)
		vga_scrollup(1);

	spin_lock(&lock);
	switch (c) {
		case '\n':
			cursor_y++;
			cursor_x = 0;
			if(cursor_y >= max_y)
				vga_scrollup(1);
			break;

		case '\r':
			cursor_x = 0;
			break;

		default:
			if(cursor_x >= max_x) {
				cursor_x = 0;
				cursor_y++;
			}
			pos = cursor_x++ * VGA_BPC + cursor_y * max_x * VGA_BPC;

			buffer[pos] = c;
			buffer[pos + 1] = attribute;
	}
	spin_unlock(&lock);
}

void vga_print(const char *s)
{
	while(*s)
 		vga_put(*s++);		// TODO: Optimiser le spinlocking ?
	vga_set_hwcursor(cursor_x, cursor_y);
}

void vga_show_cursor(int show)
{
	if(show) {
		outb(0x3D4, 0x09);
		outb(0x3D5, 0x0F);

		outb(0x3D4, 0x0B);
		outb(0x3D5, 0x0F);

		outb(0x3D4, 0x0A);
		outb(0x3D5, 0x0E);
	} 
	else {
		outb(0x3D4, 0x0A);
		outb(0x3D5, 0x2F);
	}
}

void vga_scrollup(const unsigned int lines)
{
	spin_lock(&lock);
	if(lines > cursor_y) {
		cursor_y = 0;
	} 
	else {
		cursor_y -= lines;
	}
	const unsigned int size = lines * max_x * VGA_BPC;
	memcpy(buffer, buffer + size, max_y * max_y * VGA_BPC - size);
	vga_clear_area(cursor_x, cursor_y, max_x, max_y);
	spin_unlock(&lock);
}

void vga_set_cursor(unsigned int x, unsigned int y)
{
	assert(cursor_x <= max_x);
	assert(cursor_y <= max_y);
	spin_lock(&lock);
	cursor_x = x;
	cursor_y = y;
	spin_unlock(&lock);
}

void vga_set_hwcursor(unsigned int x, unsigned int y)
{
	spin_lock(&lock);
	const unsigned int pos = x + y * max_x;
	outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
	spin_unlock(&lock);
}

void vga_set_attribute(const unsigned int font, const unsigned int back)
{
	spin_lock(&lock);
	attribute = (font & 0x0F) | ((back & 0xF0) << 4);
	spin_unlock(&lock);
}

void vga_clear_area(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
	assert(x1 <= max_x);
	assert(x2 <= max_x);
	assert(y1 <= max_y);
	assert(y2 <= max_y);
	assert(x1 <= x2);
	assert(y1 <= y2);

	// TODO: Utiliser un spinlock ?
	const unsigned int start = x1 * VGA_BPC + y1 * max_x * VGA_BPC;
	const unsigned int end = x2 * VGA_BPC + y2 * max_x * VGA_BPC;
	for(unsigned int i = start; i < end; i += 2) {
		buffer[i] = ' ';
		buffer[i + 1] = attribute;
	}
}