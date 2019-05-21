/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */




#include "multiboot/header.S"

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


/* variables */
.section .bss
k_ptr:
.skip 4


/* functions */
.section .text
/* multiboot_boot_kernel32 */
.global multiboot_boot_kernel32
.type multiboot_boot_kernel32, @function
multiboot_boot_kernel32:
	/* jump into the 32-bit kernel */
	mov	0x4(%esp), %ebx
	mov	0x8(%esp), %ecx
#ifdef DEBUG
	mov	0x10(%ebx), %eax
	DEBUG_STRING(%eax)
	mov	0x40(%ebx), %eax
	DEBUG_STRING(%eax)
#endif
	mov	$BOOT_MULTIBOOT_LOADER_MAGIC, %eax
	jmp	*%ecx


.global multiboot_boot_kernel64
.type multiboot_boot_kernel64, @function
multiboot_boot_kernel64:
	/* check for 64-bit capability */
	pushf
	pop	%eax
	mov	%eax, %ecx
	xor	$0x00200000, %eax
	push	%eax
	popf
	pushf
	pop	%eax
	cmp	%ecx, %eax
	jz	1f			/* CPUID is not supported */
	push	%ebx
	mov	$0x80000001, %eax
	cpuid
	pop	%ebx
	cmp	$0x80000000, %eax
	jl	1f			/* 64-bit mode is not supported */

	/* jump into the 64-bit kernel */
	mov	0x8(%esp), %esi
	mov	%esi, (k_ptr)

#if 0
	lgdt	(_gdt64)

	mov	_gdt64_data, %ax
	mov	%ax, %ss
	mov	%ax, %ds
	mov	%ax, %es
	jmp	_gdt64_code
.jmp_k:
	mov	0x4(%esp), %edi
	mov	k_ptr, %eax
.long 0x0
	jmp	*%eax
#endif

1:
	mov	$-1, %eax
	ret
