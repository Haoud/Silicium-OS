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

struct cpu_kstate
{
	uint16_t ss;
	uint16_t gs;
	uint16_t fs;
	uint16_t es;
	uint16_t ds;
	uint16_t pad;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t pushad_esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t free_data;
	uint32_t error_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
}_packed;

struct cpu_ustate
{
	uint16_t ss;
	uint16_t gs;
	uint16_t fs;
	uint16_t es;
	uint16_t ds;
	uint16_t pad;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t pushad_esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t free_data;
	uint32_t error_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp3;
	uint16_t ss3;
}_packed;

typedef struct cpu_kstate cpu_kstate_t;
typedef struct cpu_ustate cpu_ustate_t;