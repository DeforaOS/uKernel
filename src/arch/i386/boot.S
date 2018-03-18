/* $Id$ */
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
.global _start
.type _start, @function
_start:
	/* initialize the stack */
	mov $stack_top, %esp

	/* start the kernel */
	call main

	/* infinite loop */
	cli
1:	hlt
	jmp 1b

.size _start, . - _start
