/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



#include "../i386/multiboot/header.S"
#include "../i386/idt.S"


/* sections */
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
	mov	$stack_top, %rsp
	xor	%rbp, %rbp

#if defined(__SSP__)
	/* initialize SSP */
	call	__stack_chk_setup
#endif

	/* call the global constructors */
	call	_init

	/* set the interrupt descriptor */
	call	__arch_setidt

	/* FIXME setup paging */

	/* start the kernel */
	xor	%rdi, %rdi
	call	main

	/* exit the kernel */
	mov	%rax, %rdi
	call	exit

	hlt
