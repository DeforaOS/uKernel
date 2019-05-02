/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



#include "../i386/multiboot/header.S"


/* sections */
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
	xor	%rax, %rax
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
#ifdef __PIC__
	mov	stack_top@GOTPCREL(%rip), %rsp
	mov	(%rsp), %rsp
#else
	mov	stack_top, %rsp
#endif
	xor	%rbp, %rbp

#if defined(__SSP__)
	/* initialize SSP */
# ifdef __PIC__
	call	__stack_chk_setup@PLT
# else
	call	__stack_chk_setup
# endif
#endif

	/* call the global constructors */
#ifdef __PIC__
	call	_init@PLT
#else
	call	_init
#endif

	/* set the interrupt descriptor */
#if 0 /* FIXME rework */
# ifdef __PIC__
	call	__arch_setidt@PLT
# else
	call	__arch_setidt
# endif
#endif

	/* FIXME setup paging */

	/* start the kernel */
	xor	%rdi, %rdi
#ifdef __PIC__
	call	main@PLT
#else
	call	main
#endif

	/* exit the kernel */
	mov	%rax, %rdi
#ifdef __PIC__
	call	exit@PLT
#else
	call	exit
#endif

	hlt
