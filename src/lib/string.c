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
#include <lib/string.h>

size_t strlen(const char * str)
{
	size_t len = 0;
	while(str[len]) len++;
	return len;
}

int strcmp(const char *s1, const char *s2)
{
	const unsigned char *str1 = (const unsigned char *)(s1);
	const unsigned char *str2 = (const unsigned char *)(s2);
	unsigned char c1, c2;

	do {
		c1 = *str1++;
		c2 = *str2++;
		if (!c1) break;
	} while (c1 == c2);
	return c1 - c2;
}

int strncmp(const char *s1, const char *s2, const size_t len)
{
	const unsigned char *str1 = (const unsigned char *)(s1);
	const unsigned char *str2 = (const unsigned char *)(s2);
	unsigned char c1, c2;
	size_t offset = 0;

	do {
		c1 = *str1++;
		c2 = *str2++;
		if (!c1) break;
	} while (c1 == c2 && ++offset < len);
	return c1 - c2;
}

int snprintf(char * str, size_t n, const char * fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	const int len = vsnprintf(str, n, fmt, arg);
	va_end(arg);
	return len;
}

#define VSNPRINTF_HASHTAG	0x01
#define VSNPRINTF_ZERO		0x02
#define VSNPRINTF_MINUS		0x04
#define VSNPRINTF_SPACE		0x08
#define VSNPRINTF_PLUS		0x10

#define VSNPRINTF_SIGN		0x1000
#define VSNPRINTF_LARGE		0x2000

char *number(char *buf, uint32_t number, size_t *remaning, size_t field, uint32_t base, uint32_t flags)
{
	const char * digits = "0123456789abcdef";
	size_t padding_size = 0;
	uint32_t offset = 0;
	char padding = ' ';
	char sign = 0;
	char array[64];

	if(flags & VSNPRINTF_SIGN && ((signed)number) < 0) {
		sign = '-', number = -(signed)number;
	}
	if(flags & VSNPRINTF_SPACE) {
		sign = ' ';
	}
	if(flags & VSNPRINTF_PLUS) {
		sign = '+';
	}
	if(flags & VSNPRINTF_ZERO){
		padding = '0';
	}
	if(flags & VSNPRINTF_LARGE){
		digits = "0123456789ABCDEF";
	}

	do {
		array[offset++] =  digits[number % base];
		number /= base;
	} while (number != 0);

	if(field && offset < field) {
		padding_size = field - offset;
	}

	if(sign && padding == '0' && *remaning) {
		*buf++ = sign;
		(*remaning)--;
	}

	while (*remaning && padding_size--) {
		*buf++ = padding;
		(*remaning)--;
	}

	if(sign && padding != '0'&& *remaning) {
		*buf++ = sign;
		(*remaning)--;
	}

	while (*remaning && offset) {
		*buf++ = array[--offset];
		(*remaning)--;
	}

	return buf;
}

int vsnprintf(char * str, size_t n, const char * fmt, va_list arg)
{
	size_t remaning = n - 1;
	uint32_t field = 0;
	uint32_t flags = 0;
	uint32_t value = 0;
	char *buf = NULL;
	int exit = 0;

	for(buf = str; *fmt && remaning; fmt++) {
		if(*fmt != '%') { 
			*(buf++) = *(fmt);
			 remaning--;
			 continue;
		}

		flags = 0;
		field = 0;
		while(!exit) {
			switch (*(++fmt)) {
				default:	exit = 1;					break;
				case '+':	flags |= VSNPRINTF_PLUS; 	break;
				case '0':	flags |= VSNPRINTF_ZERO; 	break;
				case '-':	flags |= VSNPRINTF_MINUS; 	break;
				case ' ':	flags |= VSNPRINTF_SPACE; 	break;
				case '#':	flags |= VSNPRINTF_HASHTAG; break;
			}
		}

		exit = 0;
		while(*fmt >= '0' && *fmt <= '9')
			field = field * 10 + (*(fmt++) - '0');

		switch (*fmt) {
			case 'd':
			case 'i':
				flags |= VSNPRINTF_SIGN;
				_fallthrough;
			case 'u':
				value = va_arg(arg, uint32_t);
				buf = number(buf, value, &remaning, field, 10, flags);
				break;

			case 'c':
				if(remaning)
					*buf++ = va_arg(arg, int), remaning--;
				break;

			case 's':
				for (const char *str = va_arg(arg, char *); *str != '\0' && remaning; remaning--)
					*buf++ = *str++;
				break;

			case 'o':
				value = va_arg(arg, uint32_t);
				buf = number(buf, value, &remaning, field, 8, flags);
				break;

			case 'X':
				flags |= VSNPRINTF_LARGE;
				_fallthrough;
			case 'x':
				value = va_arg(arg, uint32_t);
				buf = number(buf, value, &remaning, field, 16, flags);
				break;

			case 'b':
				value = va_arg(arg, uint32_t);
				buf = number(buf, value, &remaning, field, 2, flags);
				break;

			case '%':
				if(remaning)
					*buf++ = '%', remaning--;
				break;

			default: break;
		}
	}

	*buf++ = '\0';
	return n - remaning - 1;
}