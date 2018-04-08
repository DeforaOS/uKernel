/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



#include "multiboot.S"


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
	xor %ebp, %ebp

	push %eax

#if defined(__SSP__)
	/* initialize SSP */
	call __stack_chk_setup
#endif

	/* call the global constructors */
	call _init

	/* detect multiboot */
	pop %eax
	cmp $MAGIC2, %eax
	jne 1f
	push %ebx
	call multiboot
1:

	/* start the kernel */
	push $0x0
	call main
	add $0x4, %esp

	/* exit the kernel */
	push %eax
	call exit
	add $0x4, %esp

	hlt
