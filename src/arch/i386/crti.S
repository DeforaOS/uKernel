/* $Id$ */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp


/* fini */
.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
