/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
/* functions */
.section .text
/* bus_write8 */
.global bus_write8
.type bus_write8, @function
bus_write8:
	mov 0x8(%esp), %dx
	mov 0xc(%esp), %al
	out %al, %dx
	ret
#endif
