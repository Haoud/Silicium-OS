SRCDIR = $(shell pwd)/
OBJDIR = bin/

CC = i686-elf-gcc
CFLAGS = 
CFLAGS = -g -D_DEBUG
CFLAGS += -std=gnu17 -masm=intel
CFLAGS += -c -O2 -flto -mregparm=3 -march=i686
CFLAGS += -W -Wall -Wextra -Wpedantic -pedantic
CFLAGS += -ffreestanding -fexec-charset=cp850 -I include/
CFLAGS += -pedantic-errors -Wno-ignored-qualifiers -Wno-unused-parameter

LIBGCC = -lgcc

AS = i686-elf-as
ASFLAGS = -c -march=i686

LD = i686-elf-gcc
LDFLAGS = -T "$(SRCDIR)link.ld" -nostdlib

MKDIR_P = mkdir -p

SRC_C = $(shell find . -type f -name "*.c")
SRC_ASM = $(shell find . -type f -name "*.s")

SRC = $(SRC_C) $(SRC_ASM)
OBJ = $(SRC_ASM:.s=.o) $(SRC_C:.c=.o)

TARGET = KERNEL32.ELF

all: $(TARGET)

$(TARGET): $(OBJ)
	cd $(OBJDIR);\
	$(LD) $(LDFLAGS) $^ -o KERNEL32.ELF $(LIBGCC)

%.o: %.c
	$(MKDIR_P) $(OBJDIR)$(dir $@)
	$(CC) $(CFLAGS) $^ -o $(OBJDIR)$@

%.o: %.s
	$(MKDIR_P) $(OBJDIR)$(dir $@)
	$(AS) $(ASFLAGS) $^ -o $(OBJDIR)$@

clean:
	cd $(OBJDIR)				;\
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
