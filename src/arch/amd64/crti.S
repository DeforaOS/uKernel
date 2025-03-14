/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
#ifdef __APPLE__
.section __TEXT,__text
#else
.section .init
#endif
.global _init
#ifndef __clang__
.type _init, @function
#endif
_init:
	push	%rbp
	mov	%rsp, %rbp

#ifdef __APPLE__
.section __DATA,__mod_init_func,mod_init_funcs
.p2align 3
.quad _init
#endif


/* fini */
#ifdef __APPLE__
.section __TEXT,__text
#else
.section .fini
#endif
.global _fini
#ifndef __clang__
.type _fini, @function
#endif
_fini:
	push	%rbp
	mov	%rsp, %rbp

#ifdef __APPLE__
.section __DATA,__mod_term_func,mod_term_funcs
.p2align 3
.quad _fini
#endif
