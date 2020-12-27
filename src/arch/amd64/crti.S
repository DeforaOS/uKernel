/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
.section .init
.global _init
#ifndef __clang__
.type _init, @function
#endif
_init:
	push	%rbp
	mov	%rsp, %rbp


/* fini */
.section .fini
.global _fini
#ifndef __clang__
.type _fini, @function
#endif
_fini:
	push	%rbp
	mov	%rsp, %rbp
