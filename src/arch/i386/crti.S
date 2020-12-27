/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
.section .init
.global _init
#ifndef __APPLE__
.type _init, @function
#endif
_init:
	push	%ebp
	mov	%esp, %ebp


/* fini */
.section .fini
.global _fini
#ifndef __APPLE__
.type _fini, @function
#endif
_fini:
	push	%ebp
	mov	%esp, %ebp
