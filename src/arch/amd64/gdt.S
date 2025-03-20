/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
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
	lea	gdt_descriptor, %rcx

	/* set the offset of the GDT */
	mov	%rsi, 0x2(%rcx)

	/* set the size of the GDT */
	shl	$0x3, %rdi
	dec	%rdi
	mov	%di, (%rcx)

	/* load the GDT */
	lgdt	(%rcx)

	/* apply the GDT */
	push	$0x8
	lea	gdt_flush, %rax
	push	%rax
	retf
gdt_flush:
	mov	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss

	ret


/* bss */
.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 8		/* offset */
