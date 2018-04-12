/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */




#include "multiboot/header.S"


/* sections */
/* data */
.section .data
k_ptr:
.long 0x0


/* text */
/* multiboot_boot_kernel32 */
.global multiboot_boot_kernel32
.type multiboot_boot_kernel32, @function
multiboot_boot_kernel32:
	/* jump into the 32-bit kernel */
#if 0
	mov 0x4(%esp), %ebx
	mov 0x8(%esp), %eax
	call *%eax
	ret
#else
	push %ebp
	mov %esp, %ebp

	mov 0x8(%ebp), %ebx
	mov 0xc(%ebp), %eax
	jmp *%eax
#endif


.global multiboot_boot_kernel64
.type multiboot_boot_kernel64, @function
multiboot_boot_kernel64:
	/* check for 64-bit capability */
	pushf
	pop %eax
	mov %eax, %ecx
	xor $0x00200000, %eax
	push %eax
	popf
	pushf
	pop %eax
	cmp %ecx, %eax
	jz 1f			/* CPUID is not supported */
	push %ebx
	mov $0x80000001, %eax
	cpuid
	pop %ebx
	cmp $0x80000000, %eax
	jl 1f			/* 64-bit mode is not supported */

	/* jump into the 64-bit kernel */
	mov 0x8(%esp), %esi
	mov %esi, (k_ptr)

#if 0
	lgdt (_gdt64)

	mov _gdt64_data, %ax
	mov %ax, %ss
	mov %ax, %ds
	mov %ax, %es
	jmp _gdt64_code
.jmp_k:
	mov 0x4(%esp), %edi
	mov k_ptr, %eax
.long 0x0
	jmp *%eax
#endif

1:
	mov $-1, %eax
	ret
