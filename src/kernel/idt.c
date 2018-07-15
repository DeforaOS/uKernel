/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdint.h>
# include <string.h>
# include <errno.h>
# include "arch/i386/idt.h"


/* prototypes */
extern void __arch_setidt(uint8_t * buf, size_t count);


/* variables */
/* FIXME correct the size */
static uint8_t _buf[65536];


/* functions */
/* arch_setidt */
int _arch_setidt(IDT const * idt, size_t count)
{
	uint8_t * buf = _buf;
	size_t i;
	IDT const * t;

	memset(&_buf, 0, sizeof(_buf));
	/* check for errors */
	if(/* count == 0 ||*/ count > sizeof(_buf) / sizeof(*idt))
	{
		errno = ERANGE;
		return -1;
	}
	for(i = 0; i < count; i++)
	{
		t = &idt[i];
		buf = &_buf[sizeof(t) * i];
		/* encode the address */
		buf[0] = (t->base >> 8) & 0xff;
		buf[1] = t->base & 0xff;
		buf[6] = (t->base >> 24) & 0xff;
		buf[7] = (t->base >> 16) & 0xff;
		/* selector */
		buf[2] = (t->selector >> 8) & 0xff;
		buf[3] = t->selector & 0xff;
		/* padding */
		buf[4] = 0x0;
		/* flags */
		buf[5] = t->flags;
	}
	__arch_setidt(_buf, count);
	return 0;
}
#endif
