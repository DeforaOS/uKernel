/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__NetBSD__)
/* NetBSD emulation */
# define __NetBSD_Version__	600000000

.section ".note.netbsd.ident", "a"
	.p2align 2

	.long	7
	.long	4
	.long	1
	.ascii	"NetBSD\0\0"
	.long	__NetBSD_Version__
	.previous
	.p2align 2
#elif defined(__OpenBSD__)
/* OpenBSD emulation */

.section ".note.openbsd.ident", "a"
	.p2align 2
	.long	8
	.long	4
	.long	1
	.ascii	"OpenBSD\0"
	.long	0
	.previous
#endif
