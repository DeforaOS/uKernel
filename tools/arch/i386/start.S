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
	/* reset the stack */
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
auxv:
	cmpl	$0x0, (%edx)
	jz	auxv_done
	add	$0x4, %edx
	jmp	auxv
auxv_done:
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

	/* run the userland kernel */
	push	$0x0
	call	main
	add	$0x4, %esp

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
