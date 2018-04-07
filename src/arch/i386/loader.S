/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* variables */
#define ALIGN		(1 << 0)		/* align the modules loaded on */
						/* page boundaries */
#define MEMINFO		(1 << 1)		/* provide the memory map */
#define FLAGS		ALIGN | MEMINFO		/* multiboot flags */
#define MAGIC		0x1badb002		/* magic number */
#define CHECKSUM	-(MAGIC + FLAGS)	/* checksum for the above */


/* sections */
/* multiboot */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/* bss */
.section .bss
.align 16
stack_bottom:
.skip 16384 /* 16 kB */
stack_top:


/* text */
.section .text
/* exit */
.global _exit
.type _exit, @function
_exit:
	/* infinite loop */
	cli
1:	hlt
	jmp 1b


/* start */
.global _start
.type _start, @function
_start:
	/* disable interrupts */
	cli

	/* initialize the stack */
	mov $stack_top, %esp

#if defined(__SSP__)
	/* initialize SSP */
	call __stack_chk_setup
#endif

	/* call the global constructors */
	call _init

	/* start the loader */
	push %ebx
	call main

	/* exit the kernel */
	call _exit

	hlt

.size _start, . - _start


.global _setgdt
.type _setgdt,function
_setgdt:
	lea gdt_descriptor, %ecx
	/* set the offset of the GDT */
	mov 0x4(%esp), %eax
	mov %eax, 0x2(%ecx)
	/* set the size of the GDT */
	mov 0x8(%esp), %eax
	mov %eax, (%ecx)
	/* load the GDT */
	lgdt (%ecx)
	ret


.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
