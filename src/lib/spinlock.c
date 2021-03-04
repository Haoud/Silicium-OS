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
#include <lib/spinlock.h>

void spin_init(spinlock_t *spin)
{
	spin->lock = 0;
}

void spin_lock(spinlock_t *spin)
{
	while (__sync_lock_test_and_set(&spin->lock, 1)) {
		while (spin->lock) {
			asm volatile("pause" ::: "memory");
		}	
	}
}

void spin_unlock(spinlock_t *spin)
{
	__sync_lock_release(&spin->lock);
}