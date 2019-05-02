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


/* gettimeofday */
.global _gettimeofday
.type _gettimeofday, @function
_gettimeofday:
	/* FIXME this syscall may differ on different kernels */
#ifdef __linux__
	mov	$0x4e, %eax
#else
	mov	$0x1a2, %eax
#endif
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
#ifdef __ELF__
	mov	%rdx, %rcx	/* auxv */
auxv:
	cmpq	$0x0, (%rcx)
	jz	auxv_done
	add	$0x8, %rcx
	jmp	auxv
auxv_done:
	add	$0x8, %rcx
	push	%rcx
#endif

#if defined(__SSP__)
	/* initialize SSP */
# ifdef __PIC__
	call	__stack_chk_setup@PLT
# else
	call	__stack_chk_setup
# endif
#endif

	/* run the userland kernel */
#ifdef __ELF__
	pop	%rcx
#else
	xor	%rcx, %rcx
#endif
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


/* time */
.global _time
.type _time, @function
_time:
	sub	$0x10, %rsp
	mov	%rsp, %rdi
	xor	%rsi, %rsi
	call	_gettimeofday
	cmp	$-1, %rax
	je	1f
	mov	0x8(%rsp), %rax
1:
	add	$0x10, %rsp
	ret


/* write */
.global _write
.type _write, @function
_write:
	mov	$0x4, %rax
	jmp	_syscall
