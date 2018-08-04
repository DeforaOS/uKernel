/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


.section .text
_syscall:
	int	$0x80
	jnc	.errnoret
.errno:
	mov	%eax, errno
	mov	$-1, %eax
.errnoret:
	ret


/* brk */
.global _brk
.type _brk, @function
_brk:
	/* FIXME this syscall may differ on different kernels */
#ifdef __linux__
	mov	$0x2d, %eax
	push	%ebx
	mov	0x8(%esp), %ebx
	int	$0x80
	pop	%ebx
	cmp	0x4(%esp), %eax
	jle	1f
	mov	$12, %eax
	mov	%eax, errno
	mov	$-1, %eax
	ret
1:
	mov	$0, %eax
	ret
#else
	mov	$0x11, %eax
	jmp	_syscall
#endif


/* exit */
.global _exit
.type _exit, @function
_exit:
	mov	$0x1, %eax
	jmp	_syscall


/* gettimeofday */
.global _gettimeofday
.type _gettimeofday, @function
_gettimeofday:
	mov	$0x1a2, %eax
	jmp	_syscall


/* read */
.global _read
.type _read, @function
_read:
	mov	$0x3, %eax
	jmp	_syscall


/* start */
.global _start
.type _start, @function
_start:
	/* initialize the stack */
	xor	%ebp, %ebp

	/* setup the environment */
	mov	(%esp), %eax	/* argc */
	mov	%esp, %ebx	/* argv */
	add	$0x4, %ebx
	mov	%eax, %ecx	/* envp */
	inc	%ecx
	shl	$2, %ecx
	add	%ebx, %ecx
	mov	%ecx, environ	/* environ */
#if defined(__ELF__)
	mov	%ecx, %edx	/* auxv */
1:
	cmpl	$0x0, (%edx)
	jz	2f
	add	$0x4, %edx
	jmp	1b
2:
	add	$0x4, %edx
	push	%edx
#endif
	push	%ecx
	push	%ebx
	push	%eax

#if defined(__SSP__)
	/* initialize SSP */
	call	__stack_chk_setup
#endif

	/* call the global constructors */
	call	_init

	/* run the userland kernel */
	call	main
#if defined(__ELF__)
	add	$0x10, %esp
#else
	add	$0xc, %esp
#endif

	/* exit the userland kernel */
	push	%eax
	call	exit

	hlt


/* time */
.global _time
.type _time, @function
_time:
	push	$0x0
	sub	$0xc, %esp
	push	%esp
	call	_gettimeofday
	cmp	$-1, %eax
	je	1f
	mov	0xc(%esp), %eax
	mov	0x8(%esp), %edx
1:
	add	$0x14, %esp
	ret


/* write */
.global _write
.type _write, @function
_write:
	mov	$0x4, %eax
	jmp	_syscall
