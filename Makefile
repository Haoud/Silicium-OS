TARGET = KERNEL32.ELF
SYSROOT = $$HOME/osdev/sysroot

BIN_DIR = bin/
ISO_DIR = iso/
ISO_KERNEL_LOCATION = iso/boot/

export PATH := $(HOME)/osdev/cross/bin/:$(PATH)

all: $(TARGET) install-kernel makeiso

$(TARGET):
	make -C src all -j4

clean:
	make -C src clean

install-kernel:
	cp ./src/bin/$(TARGET) $(ISO_KERNEL_LOCATION)$(TARGET)

makeiso:
	grub-mkrescue -o $(BIN_DIR)SiliciumOS.iso iso

run: all lauch

lauch:
	$(BIN_DIR)bochs -f bochs.bxrc


