/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* text */
.section .text
/* arch_setgdt */
.global __arch_setgdt
#ifndef __clang__
.type __arch_setgdt, @function
#endif
__arch_setgdt:
	lea	gdt_descriptor, %ecx

	/* set the offset of the GDT */
	mov	0x4(%esp), %eax
	mov	%eax, 0x2(%ecx)

	/* set the size of the GDT */
	mov	0x8(%esp), %eax
	shl	$0x3, %eax
	dec	%eax
	mov	%ax, (%ecx)

	/* load the GDT */
	lgdt	(gdt_descriptor)

	/* apply the GDT */
	mov	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss
	ljmp	$0x8, $gdt_flush
gdt_flush:

	ret


/* bss */
.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
