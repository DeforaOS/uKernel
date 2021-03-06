/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
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
#ifndef __clang__
.type _start, @function
#endif
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

	/* parse the command line */
	push	%ebp
	mov	%esp, %ebp
	push	$0x0		/* argc */

	/* FIXME mi->cmdline may be NULL */
	mov	%ebx, %ecx	/* ecx = mi->cmdline;	*/
	add	$0x10, %ecx
	mov	(%ecx), %ecx	/* ecx = *ecx;		*/
	mov	%ecx, %edx	/* edx = ecx;		*/
3:
	mov	(%edx), %al	/* al = *edx;		*/
	cmp	$0x0, %al	/* if(al == '\0')	*/
	je	4f		/*     goto 4f;		*/
	inc	%edx		/* edx++;		*/
	jmp	3b		/* goto 3b;		*/
4:
	push	$0x0		/* argv[] = NULL;	*/
5:
	cmp	%ecx, %edx	/* if(ecx == edx)	*/
	je	7f		/*     goto 7f;		*/
	mov	(%edx), %al	/* al = *edx;		*/
	cmp	$0x20, %al	/* if(al != ' ')	*/
	jne	6f		/*     goto 6f;		*/
	movb	$0x0, (%edx)	/* *edx = '\0';		*/
	inc	%edx
	push	%edx		/* argv[] = edx;	*/
	DEBUG_STRING(%edx)
	dec	%edx
	incl	-0x4(%ebp)	/* argc++;		*/
6:
	dec	%edx		/* edx--;		*/
	jmp	5b		/* goto 4b;		*/
7:
	push	%edx		/* argv[] = edx;	*/
	DEBUG_STRING(%edx)
	incl	-0x4(%ebp)	/* argc++;		*/

	/* prepare the arguments */
	mov	%esp, %eax	/* eax = &argv;		*/
	push	%esp
	push	%ebx		/* auxv			*/
	push	$0x0		/* envp			*/
	push	%eax		/* argv			*/
	push	-0x4(%ebp)	/* argc			*/

	jmp	2f

1:
	/* prepare the arguments */
	push	%ebp
	mov	%esp, %ebp
	push	$0x0
	push	$0x0
	push	$0x0
	push	$0x0

2:
	/* start the loader */
	call	main
	mov	%ebp, %esp
	pop	%ebp

	/* exit the loader */
	push	%eax
	call	_exit
	add	$0x4, %esp

	hlt

.size _start, . - _start
