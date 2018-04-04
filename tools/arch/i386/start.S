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


/* start */
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
#if defined(__ELF__)
	push	%edx
#endif

#if defined(__SSP__)
	/* initialize SSP */
	call	__stack_chk_setup
#endif

	/* run the userland kernel */
	call	main

	/* exit the userland kernel */
	mov	%ebp, %esp
	push	%eax
	call	exit

	hlt


/* write */
.global write
.type write, @function
write:
	mov	$0x4, %eax
	jmp	_syscall


#if defined(__NetBSD__)
/* NetBSD emulation */
# define __NetBSD_Version__	600000000

.section ".note.netbsd.ident", "a"
	.p2align 2

	.long	7
	.long	4
	.long	1
	.ascii	"NetBSD\0\0"
	.long	__NetBSD_Version__
	.previous
	.p2align 2
#elif defined(__OpenBSD__)
/* OpenBSD emulation */

.section ".note.openbsd.ident", "a"
	.p2align 2
	.long	8
	.long	4
	.long	1
	.ascii	"OpenBSD\0"
	.long	0
	.previous
#endif
