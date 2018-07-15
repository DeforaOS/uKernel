/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
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
	mov	$0x0, %eax
	ret


/* iowait */
.global iowait
.type iowait, @function
iowait:
	mov	$0x0, %eax
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
	mov	$0x0, %eax
	ret
#endif
