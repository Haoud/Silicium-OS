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

void *__memset(void * dst, uint8_t fill, size_t len);
void *memmove(void *dst, const void *src, size_t len);
int memcmp(const void *p1, const void *p2, size_t count);
void *aligned_memset(void *dst, uint32_t fill, size_t len);
void *__memcpy(void *restrict dst, const void *restrict src, size_t len);
void *aligned_memcpy(void *restrict dst, const void *restrict src, size_t len);

static inline void * memcpy(void *restrict dst, const void *restrict src, size_t len)
{
    if(!((uint32_t)dst & 3) && !((uint32_t)src & 3))
        return aligned_memcpy(dst, src, len);

    return __memcpy(dst, src, len);
}

static inline void * memset(void *dst, uint8_t fill, size_t len)
{
    uint32_t fill32 = 0;
    if(unlikely(fill))
        fill32 = fill | fill << 8 | fill << 16 | fill << 24;

    if(!((uint32_t)dst & 3))
        return aligned_memset(dst, fill32, len);

    uint32_t offset = ((uint32_t)dst & 3);
    __memset(dst, fill, 4 - offset);
    return aligned_memset((char *)dst + offset, fill32, len - offset);
}