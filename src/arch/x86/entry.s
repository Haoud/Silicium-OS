.set MULTIBOOT_ALIGN_FLAG,		0x01
.set MULTIBOOT_MEMINFO_FLAG,	0x02
.set MULTIBOOT_VIDEO_INFO,		0x04

.set MULTIBOOT_MAGIC,			0x1BADB002
.set KERNEL_VIRTUAL_BASE,		0xC0000000

.set MULTIBOOT_FLAGS,			MULTIBOOT_ALIGN_FLAG | MULTIBOOT_MEMINFO_FLAG | MULTIBOOT_VIDEO_INFO
.set MULTIBOOT_CHECKSUM,		-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.set CR0_PAGING_FLAG,				0x80000000		# Pour activer la pagination
.set CR0_WRITE_PROTECT_BIT,	 		0x00010000		# Respecte les pages marquées en lecture seule même en mode noyau
.set CR4_PAGE_SIZE_EXTENSION_BIT,	0x00000010		# Pour pouvoir avoir des pages de 4Mo et économiser de la mémoire

.global _entry
.extern boot_x86

.section .multiboot
.align 8

.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM
.long 0						# header_addr
.long 0						# load_addr
.long 0						# load_end_addr
.long 0						# bss_end_addr
.long 0						# entry_addr
.long 0						# Type de mode vidéo
.long 0						# Largeur vidéo préféré en pixel (0 = pas de préférence)
.long 0						# Hauteur vidéo préféré en pixel (0 = pas de préférence)
.long 0						# Pronfondeur vidéo préféré en bits (0 = pas de préférence)

.section .text
.align 4

_entry:
	cld
	xorl %edi, %edi
	xorl %ebp, %ebp
	movl $255, %ecx

	movl $(bootPD - KERNEL_VIRTUAL_BASE), %esi
	movl %edi, (%esi)
	orl $0x83,(%esi)									# Accessible en lecture / écriture en mode noyau + bit PSE

	addl $768*4, %esi
.L1:
	movl %edi, (%esi)
	orl $0x83,(%esi)									# Accessible en lecture / écriture en mode noyau + bit PSE

	addl $4, %esi
	addl $0x400000, %edi
	loop .L1

	# Met en place le mirrorring
	movl $(bootPD - KERNEL_VIRTUAL_BASE), (%esi)
	orl $0x03, (%esi)

	movl $(bootPD - KERNEL_VIRTUAL_BASE), %edx
	movl %edx, %cr3			# Met à jour le registre CR3 (adresse physique du répertoire de pages)

	movl %cr4, %edx
	orl $CR4_PAGE_SIZE_EXTENSION_BIT, %edx
	movl %edx, %cr4

	movl %cr0, %edx
	orl $(CR0_WRITE_PROTECT_BIT | CR0_PAGING_FLAG), %edx
	movl %edx, %cr0

	leal _lauch, %ecx		# Récupère l'adresse virtuelle du label
	jmpl *%ecx				# Saute au code du noyau (saut absolu et non pas relatif)

_lauch:
	movl $stack_top, %esp					# Met en place la pile

	pushl %ebx								# Adresse physique de la structure multiboot
	call boot_x86							# Appele la fonction qui va initialiser le noyau

.section .bss
.align 4096
bootPD:			.space 4096

stack_bottom:	.space 4096
stack_top:
