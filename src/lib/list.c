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
#include <lib/list.h>

int list_empty(struct list_head *list)
{
	return (list->prev == list) && (list->next == list);
}

void list_init(struct list_head *list)
{
	list->prev = list->next = list;
}

void list_entry_init(struct list_head *list)
{
	list->prev = list->next = NULL;
}

void list_remove(struct list_head *list)
{
	assert(list->prev);
	assert(list->next);
	list->prev->next = list->next;
	list->next->prev = list->prev;
	list->prev = list->next = NULL;
}

void list_add(struct list_head *list, struct list_head *entry)
{
	assert(list->prev);
	assert(list->next);

	entry->prev = list;
	entry->next = list->next;
	list->next->prev = entry;
	list->next = entry;
}