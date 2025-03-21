/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* text */
.section .text
/* arch_setidt */
.global __arch_setidt
#ifndef __clang__
.type __arch_setidt, @function
#endif
__arch_setidt:
	lea	idt_descriptor, %ecx

	/* set the offset of the IDT */
	mov	0x4(%esp), %eax
	mov	%eax, 0x2(%ecx)

	/* set the size of the IDT */
	mov	0x8(%esp), %eax
	dec	%eax
	mov	%ax, (%ecx)

	/* load the IDT */
	lidt	(idt_descriptor)

	ret


/* bss */
.section .bss
.align 16
idt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
