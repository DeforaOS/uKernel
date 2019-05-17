/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* functions */
.section .text
/* inb */
.global inb
.type inb, @function
inb:
	mov	0x4(%esp), %dx		/* address */
	movl	0x8(%esp), %ecx		/* return address */
	inb	%dx, %al
	mov	%al, (%ecx)

	/* return 0 */
	xor	%eax, %eax
	ret


/* iowait */
.global iowait
.type iowait, @function
iowait:
	xor	%eax, %eax
	outb	%al, $0x80

	/* return 0 */
	ret


/* outb */
.global outb
.type outb, @function
outb:
	mov	0x4(%esp), %dx		/* address */
	mov	0x8(%esp), %al		/* value */
	out	%al, %dx

	/* return 0 */
	xor	%eax, %eax
	ret
