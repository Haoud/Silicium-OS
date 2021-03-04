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

#define MULTIBOOT_HEADER_MAGIC			0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC		0x2BADB002
#define MULTIBOOT_STACK_SIZE			0x4000

#define MULTIBOOT_INFO_MEMORY					0x00000001
#define MULTIBOOT_INFO_BOOTDEV					0x00000002
#define MULTIBOOT_INFO_CMDLINE					0x00000004
#define MULTIBOOT_INFO_MODS						0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS				0x00000010
#define MULTIBOOT_INFO_ELF_SHDR					0X00000020
#define MULTIBOOT_INFO_MEM_MAP					0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO				0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE				0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME			0x00000200
#define MULTIBOOT_INFO_APM_TABLE				0x00000400
#define MULTIBOOT_INFO_VBE_INFO					0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO			0x00001000

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED		0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB			1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT		2

#define MULTIBOOT_MEMORY_AVAILABLE				1
#define MULTIBOOT_MEMORY_RESERVED				2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE		3
#define MULTIBOOT_MEMORY_NVS					4
#define MULTIBOOT_MEMORY_BADRAM					5

struct multiboot_header
{
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;
	uint32_t header_addr;
	uint32_t load_addr;
	uint32_t load_end_addr;
	uint32_t bss_end_addr;
	uint32_t entry_addr;
}_packed;

struct multiboot_elf_table
{
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
}_packed;

struct multiboot_module
{
	uint32_t mod_start;
	uint32_t mod_end;
	uint32_t string;
	uint32_t reserved;
}_packed;

struct multiboot_mmap
{
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
}_packed;

struct multiboot_info
{
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;

	struct multiboot_elf_table elf_sec;

	uint32_t mmap_length;
	struct multiboot_mmap * mmap_addr;

	uint32_t drives_length;
	uint32_t drives_addr;

	uint32_t config_table;

	uint32_t boot_loader_name;

	uint32_t apm_table;

	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;

	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t framebuffer_bpp;
	uint8_t framebuffer_type;

	union
	{
		struct
		{
			uint32_t framebuffer_palette_addr;
			uint16_t framebuffer_palette_num_colors;
		}palette;

		struct
		{
			uint8_t framebuffer_red_field_position;
			uint8_t framebuffer_red_mask_size;
			uint8_t framebuffer_green_field_position;
			uint8_t framebuffer_green_mask_size;
			uint8_t framebuffer_blue_field_position;
			uint8_t framebuffer_blue_mask_size;
		}color_mask;

	}framebuffer_data;
}_packed;

static inline void * multiboot_translate_ptr(void * ptr)
{
	assert((ptr_t)(ptr) < 0x40000000);
	return (void *)((ptr_t)(ptr) + KERNEL_VBASE);
}

static inline void multiboot_translate_addr(ptr_t * ptr)
{
	assert(*ptr < 0x40000000);
	*ptr += KERNEL_VBASE;
}

static inline struct multiboot_info * translate_multiboot_info(struct multiboot_info *info)
{
	info = multiboot_translate_ptr(info);

	if(info->flags & MULTIBOOT_INFO_DRIVE_INFO) {
		multiboot_translate_addr(&info->drives_addr);
	}
	if(info->flags & MULTIBOOT_INFO_MEM_MAP) {
		info->mmap_addr = multiboot_translate_ptr(info->mmap_addr);
	}
	if(info->flags & MULTIBOOT_INFO_CMDLINE) {
		multiboot_translate_addr(&info->cmdline);
	}
	if(info->flags & MULTIBOOT_INFO_MODS) {
		multiboot_translate_addr(&info->mods_addr);
	}
	if(info->flags & MULTIBOOT_INFO_ELF_SHDR) {
		multiboot_translate_addr(&info->elf_sec.addr);
	}
	if(info->flags & MULTIBOOT_INFO_CONFIG_TABLE) {
		multiboot_translate_addr(&info->config_table);
	}
	if(info->flags & MULTIBOOT_INFO_APM_TABLE) {
		multiboot_translate_addr(&info->apm_table);
	}
	if(info->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME) {
		multiboot_translate_addr(&info->boot_loader_name);
	}
	if(info->flags & MULTIBOOT_INFO_VBE_INFO) {
		multiboot_translate_addr(&info->vbe_control_info);
		multiboot_translate_addr(&info->vbe_mode_info);
	}
	
	return info;
}