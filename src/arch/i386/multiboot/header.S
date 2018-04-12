/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/boot/multiboot/constants.h"


/* variables */
#define FLAGS		(BOOT_MULTIBOOT_HEADER_MODS_ALIGNED \
		| BOOT_MULTIBOOT_HEADER_WANT_MEMORY \
		| BOOT_MULTIBOOT_HEADER_HAS_VBE)
#define MAGIC		BOOT_MULTIBOOT_HEADER_MAGIC
#define CHECKSUM	-(MAGIC + FLAGS)	/* checksum for the above */


/* sections */
/* multiboot */
.section .multiboot
.align 4

multiboot_header:
.long MAGIC
.long FLAGS
.long CHECKSUM

#ifdef __ELF__
.long 0
.long 0
.long 0
.long 0
.long 0
#else
.long multiboot_header
.long _multiboot_start
.long _edata
.long _end
.long _start
#endif

/* fail-safe video mode */
.long 0
.long 640
.long 480
.long 8


.section .text
/* multiboot_start */
.global _multiboot_start
.type _multiboot_start, @function
_multiboot_start:
	jmp _start
