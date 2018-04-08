/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* variables */
#define ALIGN		(1 << 0)		/* align the modules loaded on */
						/* page boundaries */
#define MEMINFO		(1 << 1)		/* provide the memory map */
#define FLAGS		ALIGN | MEMINFO		/* multiboot flags */
#define MAGIC1		0x1badb002		/* magic number */
#define MAGIC2		0x2badb002		/* magic number */
#define CHECKSUM	-(MAGIC1 + FLAGS)	/* checksum for the above */


/* sections */
/* multiboot */
.section .multiboot
.align 4
.long MAGIC1
.long FLAGS
.long CHECKSUM
