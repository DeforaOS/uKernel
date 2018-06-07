/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
.section .init
.global _init
.type _init, @function
_init:
	push	%rbp
	mov	%rsp, %rbp


/* fini */
.section .fini
.global _fini
.type _fini, @function
_fini:
	push	%rbp
	mov	%rsp, %rbp
