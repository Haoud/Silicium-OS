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
#include <lib/spinlock.h>

#define VGA_BPC			2			// Byte per character
#define VGA_BUFFER		0xB8000		// Adresse physique du buffer vga
#define VGA_MAX_X		80
#define VGA_MAX_Y		25

#define VGA_COLOR0		0x00		// Noir (palette par défaut)
#define VGA_COLOR1		0x01		// Bleu (palette par défaut)
#define VGA_COLOR2		0x02		// Vert (palette par défaut)
#define VGA_COLOR3		0x03		// Cyan (palette par défaut)
#define VGA_COLOR4		0x04		// Rouge (palette par défaut)
#define VGA_COLOR5		0x05		// Magenta (palette par défaut)
#define VGA_COLOR6		0x06		// Marron (palette par défaut)
#define VGA_COLOR7		0x07		// Gris clair (palette par défaut)
#define VGA_BRIGHTNESS	0x08	

void vga_init(void);
void vga_clear(void);
void vga_put(const char c);
void vga_print(const char *s);
void vga_show_cursor(int show);
void vga_scrollup(const unsigned int lines);
void vga_set_cursor(unsigned int x, unsigned int y);
void vga_set_hwcursor(unsigned int x, unsigned int y);
void vga_set_attribute(const unsigned int font, const unsigned int back);
void vga_clear_area(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2);
