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
	/* initialize the stack */
	mov $stack_top, %esp

#if defined(__SSP__)
	/* initialize SSP */
	call __stack_chk_setup
#endif

	/* call the global constructors */
	call _init

	/* start the kernel */
	call main

	/* exit the kernel */
	push %eax
	call exit

	hlt

.size _start, . - _start
