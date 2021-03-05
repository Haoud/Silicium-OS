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

/* Important: Ne concerne que le CPU qui exécute le code ! */
#define cli()		asm volatile("cli")
#define sti()		asm volatile("sti")
#define hlt()		asm volatile("hlt")
#define clts()		asm volatile("clts")

#define cpu_relax()						asm volatile("pause" ::: "memory")
#define clear_task_switched()			clts()
#define enable_interruption()			sti()
#define disable_interruption()			cli()

#define CPUID_GET_FEATURE			1
#define CPUID_GET_CAPABILITIES		0x80000007

#define CPUID_EDX_FEATURE_FPU		0x00000001
#define CPUID_EDX_FEATURE_VME		0x00000002
#define CPUID_EDX_FEATURE_DE		0x00000004
#define CPUID_EDX_FEATURE_PSE		0x00000008
#define CPUID_EDX_FEATURE_TSC		0x00000010
#define CPUID_EDX_FEATURE_MSR		0x00000020
#define CPUID_EDX_FEATURE_PAE		0x00000040
#define CPUID_EDX_FEATURE_MCE		0x00000080
#define CPUID_EDX_FEATURE_CX8		0x00000100
#define CPUID_EDX_FEATURE_APIC		0x00000200
#define CPUID_EDX_FEATURE_SEP		0x00000800
#define CPUID_EDX_FEATURE_MTRR		0x00001000
#define CPUID_EDX_FEATURE_PGE		0x00002000
#define CPUID_EDX_FEATURE_MCA		0x00008000
#define CPUID_EDX_FEATURE_CMOV		0x00010000
#define CPUID_EDX_FEATURE_PAT		0x00020000
#define CPUID_EDX_FEATURE_PSE36		0x00040000
#define CPUID_EDX_FEATURE_PSN		0x00080000
#define CPUID_EDX_FEATURE_CLF		0x00100000
#define CPUID_EDX_FEATURE_DTES		0x00200000
#define CPUID_EDX_FEATURE_ACPI		0x00400000
#define CPUID_EDX_FEATURE_MMX		0x00800000
#define CPUID_EDX_FEATURE_FXSR		0x01000000
#define CPUID_EDX_FEATURE_SSE		0x02000000
#define CPUID_EDX_FEATURE_SSE2		0x04000000
#define CPUID_EDX_FEATURE_SS		0x08000000
#define CPUID_EDX_FEATURE_HTT		0x10000000
#define CPUID_EDX_FEATURE_TM1		0x20000000
#define CPUID_EDX_FEATURE_IA64		0x40000000
#define CPUID_EDX_FEATURE_PBE		0x80000000

#define CPUID_EDX_CAPABILITIES_ITSC	0x80000100

static inline void set_task_switched(void)
{
	asm volatile("	mov eax, cr0	\n\
					or eax, 0x08	\n\
					mov cr0, eax" ::: "eax");
}

static inline void cpuid_count(native_t code, native_t count, 
							   native_t *eax, native_t *ebx,
							   native_t *ecx, native_t *edx)
{
	asm volatile("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) 
						 : "0"(code), "2"(count)
						 : "memory");
}

static inline void cpuid(native_t code,
						 native_t *eax, native_t *ebx,
						 native_t *ecx, native_t *edx)
{
	cpuid_count(code, 0, eax, ebx, ecx, edx);
}

static native_t cpuid_eax(native_t code)
{
	native_t eax, ebx, ecx, edx;
	cpuid(code, 0, &eax, &ebx, &ecx, &edx);
	return eax;
}

static native_t cpuid_ebx(native_t code)
{
	native_t eax, ebx, ecx, edx;
	cpuid(code, 0, &eax, &ebx, &ecx, &edx);
	return ebx;
}

static native_t cpuid_ecx(native_t code)
{
	native_t eax, ebx, ecx, edx;
	cpuid(code, 0, &eax, &ebx, &ecx, &edx);
	return ecx;
}

static native_t cpuid_exx(native_t code)
{
	native_t eax, ebx, ecx, edx;
	cpuid(code, 0, &eax, &ebx, &ecx, &edx);
	return edx;
}

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