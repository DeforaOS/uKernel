/* $Id$ */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* sections */
/* init */
.section .init
	popl %ebp
	ret


/* fini */
.section .fini
	popl %ebp
	ret
