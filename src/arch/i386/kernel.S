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
	jmp	1b


/* start */
.global _start
.type _start, @function
_start:
	/* disable interrupts */
	cli

	/* initialize the stack */
	mov	$stack_top, %esp
	xor	%ebp, %ebp

	push	%eax

	/* reset EFLAGS */
	pushl	$0x0
	popf

#if defined(__SSP__)
	/* initialize SSP */
	call	__stack_chk_setup
#endif

	/* call the global constructors */
	call	_init

	/* detect multiboot */
	pop	%eax
	cmp	$BOOT_MULTIBOOT_LOADER_MAGIC, %eax
	jne	1f
	push	%ebx
	call	multiboot
	add	$0x4, %esp

	/* set the interrupt descriptor */
	call	__arch_setidt

	/* start the kernel */
	push	$0x0
	push	$0x0
	mov	%ebx, %eax
	add	$0x10, %eax
	push	%eax
	push	$0x1
	call	main
	add	$0x10, %esp
	jmp	2f
1:

	/* set the interrupt descriptor */
	call	__arch_setidt

	/* FIXME setup paging */

	/* start the kernel */
	push	$0x0
	push	$0x0
	push	$0x0
	push	$0x0
	call	main
	add	$0x10, %esp

2:
	/* exit the kernel */
	push	%eax
	call	exit
	add	$0x4, %esp

	hlt
