/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


.section .text
_syscall:
	mov	%rcx, %r10
	syscall
	jnc	_syscall_return
.errno:
#ifdef __PIC__
	mov	errno@GOTPCREL(%rip), %rcx
	mov	%rax, (%rcx)
#else
	mov	%rax, errno
#endif
	mov	$-1, %rax
_syscall_return:
	ret


/* _brk */
.global _brk
.type _brk,@function
_brk:
	/* FIXME this syscall may differ on different kernels */
	mov	$0x11, %rax
	syscall
	jc	.errno
	ret


/* exit */
.global _exit
.type _exit, @function
_exit:
	mov	$0x1, %rax
	jmp	_syscall


/* read */
.global _read
.type _read, @function
_read:
	mov	$0x3, %rax
	jmp	_syscall


/* start */
.global _start
.type _start, @function
_start:
	/* reset the stack */
	xor	%rbp, %rbp

	/* setup the environment */
	mov	(%rsp), %rdi	/* argc */
	mov	%rsp, %rsi	/* argv */
	add	$0x8, %rsi
	mov	%rdi, %rdx	/* envp */
	shl	$3, %rdx
	add	%rsi, %rdx
	add	$0x8, %rdx
#ifdef __PIC__			/* environ	*/
	mov	environ@GOTPCREL(%rip), %rcx
	mov	%rdx, (%rcx)
#else
	mov	%rdx, environ
#endif
	push	%rdi
	push	%rsi
	push	%rdx

#if defined(__SSP__)
	/* initialize SSP */
# ifdef __PIC__
	call	__stack_chk_setup@PLT
# else
	call	__stack_chk_setup
# endif
#endif

	/* run the userland kernel */
	pop	%rdx
	pop	%rsi
	pop	%rdi
#ifdef __PIC__
	call	main@PLT
#else
	call	main
#endif

	/* exit the userland kernel */
	mov	%rax, %rdi
#ifdef __PIC__
	call	exit@PLT
#else
	call	exit
#endif

	hlt


/* write */
.global _write
.type _write, @function
_write:
	mov	$0x4, %rax
	jmp	_syscall
