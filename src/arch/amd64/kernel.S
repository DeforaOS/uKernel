/* $Id$ */
/* Copyright (c) 2018-2021 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



#include "../i386/multiboot/header.S"

/* macros */
#ifdef DEBUG
# ifdef __PIC__
#  define DEBUG_STRING(s) \
	pusha; \
	mov s, %rdi; \
	call puts@PLT; \
	popa;
# else
#  define DEBUG_STRING(s) \
	pusha; \
	mov s, %rdi; \
	call puts; \
	popa;
# endif
#else
# define DEBUG_STRING(s)
#endif


/* sections */
/* bss */
.section .bss
.align 16
stack_bottom:
.skip 16384 /* 16 kB */
stack_top:


/* text */
.section .text
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

	/* save %eax to detect multiboot */
	push	%rax

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

	/* detect multiboot */
	pop	%rax
	cmp	$BOOT_MULTIBOOT_LOADER_MAGIC, %eax
	jne	1f
	mov	%rbx, %rdi
#ifdef __PIC__
	call	multiboot@PLT
#else
	call	multiboot
#endif
	cmp	$0x0, %rax
	jne	1f

	/* TODO parse the command line */

	/* TODO prepare the arguments */
	xor	%rax, %rax
	push	%rax
	push	%rax
	push	%rax
	push	%rax

	jmp	2f

1:
	/* prepare the arguments */
	xor	%rax, %rax
	push	%rax
	push	%rax
	push	%rax
	push	%rax

2:
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
	pop	%rcx
	pop	%rdx
	pop	%rsi
	pop	%rdi

	/* run the program */
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
