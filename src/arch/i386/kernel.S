/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



#include "multiboot.S"

/* macros */
#ifdef DEBUG
# define DEBUG_STRING(s) \
	pusha; \
	push s; \
	call puts; \
	pop s; \
	popa;
#else
# define DEBUG_STRING(s)
#endif


/* bss */
.section .bss
.align 16
stack_bottom:
.skip 16384 /* 16 kB */
stack_top:


/* text */
.section .text
/* brk */
.global _brk
.type _brk, @function
_brk:
	xor	%eax, %eax
	mov	0x4(%esp), %ecx
	/* XXX limits the heap to the first 1 MB */
	cmp	$0x100000, %ecx
	jl	1f
	not	%eax
1:
	ret


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

	/* save %eax to detect multiboot */
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
	cmp	$0x0, %eax
	jne	2f

	/* prepare the arguments */
	push	$0x0
	push	$0x0
	mov	%ebx, %eax
	add	$0x10, %eax
	push	%eax
	push	$0x1
	jmp	2f

1:
	/* FIXME setup paging */

	/* prepare the arguments */
	push	$0x0
	push	$0x0
	push	$0x0
	push	$0x0

2:
	/* start the kernel */
	call	main
	add	$0x10, %esp

	/* exit the kernel */
	push	%eax
	call	exit
	add	$0x4, %esp

	hlt
