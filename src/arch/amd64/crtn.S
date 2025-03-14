/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
#ifdef __APPLE__
.section __TEXT,__text
.global _crtn_init
_crtn_init:
#else
.section .init
#endif
	pop %rbp
	ret

#ifdef __APPLE__
.section __DATA,__mod_init_func,mod_init_funcs
.p2align 3
.quad _crtn_init
#endif


/* fini */
#ifdef __APPLE__
.section __TEXT,__text
.global _crtn_fini
_crtn_fini:
#else
.section .fini
#endif
	pop %rbp
	ret

#ifdef __APPLE__
.section __DATA,__mod_term_func,mod_term_funcs
.p2align 3
.quad _crtn_fini
#endif
