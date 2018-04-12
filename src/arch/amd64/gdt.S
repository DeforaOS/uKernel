/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* text */
.section .text
/* arch_setgdt */
.global __arch_setgdt
.type __arch_setgdt, @function
__arch_setgdt:
#if 0
	lea gdt_descriptor, %rcx
#else
	mov (gdt_descriptor), %rcx
#endif
	/* set the offset of the GDT */
	mov %rsi, 0x2(%rcx)
	/* set the size of the GDT */
	dec %rdi
	mov %di, (%rcx)
	/* load the GDT */
	lgdt (%rcx)
	ret


/* bss */
.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
