/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


.section .text
_syscall:
#ifdef __linux__
	push	%ebx
	push	%esi
	push	%edi
	mov	0x10(%esp), %ebx
	mov	0x14(%esp), %ecx
	mov	0x18(%esp), %edx
	mov	0x1c(%esp), %esi
	mov	0x20(%esp), %edi
	int	$0x80
	pop	%edi
	pop	%esi
	pop	%ebx
	cmp	$0, %eax
	jge	.errnoret
	neg	%eax
#else
	int	$0x80
	jnc	.errnoret
#endif
.errno:
	mov	%eax, errno
	mov	$-1, %eax
.errnoret:
	ret


/* gettimeofday */
.global _gettimeofday
.type _gettimeofday, @function
_gettimeofday:
	/* FIXME this syscall may differ on different kernels */
#ifdef __linux__
	mov	$0x60, %eax
#else
	mov	$0x1a2, %eax
#endif
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

	/* initialize the userland */
	call	init

	/* run the userland kernel */
	call	main
#if defined(__ELF__)
	add	$0x10, %esp
#else
	add	$0xc, %esp
#endif
	push	%eax

	/* call the global destructors */
	call	_fini

	/* exit the userland kernel */
	call	exit
	add	$0x4, %esp

	hlt


/* time */
.global _time
.type _time, @function
_time:
	sub	$0xc, %esp
	mov	%esp, %eax
	push	$0x0
	push	%eax
	call	_gettimeofday
	cmp	$-1, %eax
	je	1f
	mov	0x10(%esp), %eax
	mov	0xc(%esp), %edx
1:
	add	$0x14, %esp
	ret


/* write */
.global _write
.type _write, @function
_write:
	mov	$0x4, %eax
	jmp	_syscall
