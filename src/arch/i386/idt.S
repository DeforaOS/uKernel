/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* text */
.section .text
/* arch_setidt */
.global __arch_setidt
.type __arch_setidt, @function
__arch_setidt:
	lidt (idt)
	ret


/* bss */
.section .bss
.align 16
idt:
.short 0x0	/* size */
.long 0x0	/* offset */
