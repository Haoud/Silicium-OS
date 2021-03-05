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

#define EXCEPTION_NUMBER		32

#define EXCEPTION_DIVIDE_BY_ZERO_ID					0
#define EXCEPTION_DEBUG_ID							1
#define EXCEPTION_NMI_ID							2
#define EXCEPTION_BREAKPOINT_ID						3
#define EXCEPTION_OVERFLOW_ID						4
#define EXCEPTION_BOUND_RANGE_EXCEDEED_ID			5
#define EXCEPTION_INVALID_OPCODE_ID					6
#define EXCEPTION_DEVICE_NOT_AVAIBLE_ID				7
#define EXCEPTION_DOUBLE_FAULT_ID					8
#define EXCEPTION_COPROCESSOR_ERROR_ID				9
#define EXCEPTION_INVALID_TSS_ID					10
#define EXCEPTION_SEGMENT_NOT_PRESENT_ID			11
#define EXCEPTION_STACK_SEGMENT_FAULT_ID			12
#define EXCEPTION_GENERAL_PROTECTION_FAULT_ID		13
#define EXCEPTION_PAGE_FAULT_ID						14
#define EXCEPTION_INTEL_RESERVED_1_ID				15
#define EXCEPTION_FLOATING_POINT_ERROR_ID			16
#define EXCEPTION_ALIGNMENT_CHECK_ID 				17
#define EXCEPTION_MACHINE_CHECK_ID					18
#define EXCEPTION_INTEL_RESERVED_2_ID				19
#define EXCEPTION_INTEL_RESERVED_3_ID				20
#define EXCEPTION_INTEL_RESERVED_4_ID				21
#define EXCEPTION_INTEL_RESERVED_5_ID				22
#define EXCEPTION_INTEL_RESERVED_6_ID				23
#define EXCEPTION_INTEL_RESERVED_7_ID				24
#define EXCEPTION_INTEL_RESERVED_8_ID				25
#define EXCEPTION_INTEL_RESERVED_9_ID 				26
#define EXCEPTION_INTEL_RESERVED_10_ID				27
#define EXCEPTION_INTEL_ERSERVED_11_ID 				28
#define EXCEPTION_INTEL_RESERVED_12_ID				29
#define EXCEPTION_INTEL_RESERVED_13_ID				30
#define EXCEPTION_INTEL_RESERVED_14_ID				31

void exceptions_init(void);
