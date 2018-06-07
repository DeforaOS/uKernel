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


/* exit */
.global _exit
.type _exit, @function
_exit:
	mov	$0x1, %eax
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
#endif
	push	%ebp
	push	%esp
	mov	%esp, %ebp
	push	%ecx
	push	%ebx
	push	%eax

#if defined(__SSP__)
	/* initialize SSP */
	call	__stack_chk_setup
#endif

	/* call the global constructors */
	call _init

	/* run the userland kernel */
	push	$0x0
	push	$0x0
	push	$0x0
	push	$0x0
	call	main
	add	$0x10, %esp

	/* exit the userland kernel */
	mov	%ebp, %esp
	push	%eax
	call	exit

	hlt


/* write */
.global _write
.type _write, @function
_write:
	mov	$0x4, %eax
	jmp	_syscall
