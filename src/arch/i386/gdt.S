/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* text */
.section .text
/* arch_setgdt */
.global __arch_setgdt
.type __arch_setgdt,function
__arch_setgdt:
#if 0
	lea gdt_descriptor, %ecx
#else
	mov (gdt_descriptor), %ecx
#endif
	/* set the offset of the GDT */
	mov 0x4(%esp), %eax
	mov %eax, 0x2(%ecx)
	/* set the size of the GDT */
	mov 0x8(%esp), %eax
	dec %eax
	mov %ax, (%ecx)
	/* load the GDT */
	lgdt (%ecx)
	ret


/* bss */
.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
