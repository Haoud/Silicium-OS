ENTRY(_entry)
OUTPUT_ARCH("i386")
OUTPUT_FORMAT("elf32-i386")

SECTIONS
{
	. = 0xC0100000;
	_start = .;

    .text : AT(ADDR(.text) - 0xC0000000)
    {
		*(.multiboot*)
        *(.text*)
    }

    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data*)
    }

	.rodata : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata*)
	}

	.bss : AT(ADDR(.bss) - 0xC0000000)
	{
		*(.bss)
		*(COMMON)
	}

	/DISCARD/ :
	{
		*(.comment)
	}

	_end = .;
}
