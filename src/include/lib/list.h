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

struct list_head
{
	struct list_head *prev;
	struct list_head *next;
};

#define list_entry(ptr, type, member) 								\
((type *) ((char *)(ptr) – (native_t)(&((type *)0)->member)))*

#define list_foreach(list, entry)								\
	for(struct list_head * entry = list->next);					\
		(entry) != (list);										\
		(entry) = (entry)->next)

#define list_foreach_d(list, entry)								\
	for(entry = list->next);									\
		(entry) != (list);										\
		(entry) = (entry)->next)

#define list_foreach_safe(list, entry)							\
	for(struct list_head * entry = (list)->next,				\
		*__next = (entry)->next;								\
		(entry) != (list);										\
		(entry) = __next, __next = (entry)->next)

#define list_foreach_safe_d(list, entry)						\
	entry = (list)->next;										\
	for(struct list_head *__next = (entry)->next;				\
		(entry) != (list);										\
		(entry) = __next, __next = (entry)->next)

int list_empty(struct list_head *list);
void list_init(struct list_head *list);
void list_remove(struct list_head *list);
void list_entry_init(struct list_head *list);
void list_add(struct list_head *list, struct list_head *entry);