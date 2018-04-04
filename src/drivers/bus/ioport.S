/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
/* functions */
.section .text
/* ioport_write8 */
.global ioport_write8
.type ioport_write8, @function
ioport_write8:
	mov	0x8(%esp), %dx		/* address */
	mov	0xc(%esp), %al		/* value */
	out	%al, %dx

	/* return 0 */
	mov	$0x0, %eax
	ret
#endif
