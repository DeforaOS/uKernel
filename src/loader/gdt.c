/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdint.h>
# include <string.h>
# include <errno.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"


/* prototypes */
extern void __arch_setgdt(uint8_t * buf, size_t count);


/* variables */
static uint8_t _buf[65536];


/* functions */
/* arch_setgdt */
int _arch_setgdt(GDT const * gdt, size_t count)
{
	uint8_t * buf = _buf;
	size_t i;
	GDT g;

	memset(&_buf, 0, sizeof(_buf));
	/* check for errors */
	if(count == 0 || count > sizeof(_buf) / sizeof(*gdt))
	{
		errno = ERANGE;
		return -1;
	}
	for(i = 0; i < count; i++)
	{
		g = gdt[i];
		buf = &_buf[sizeof(g) * i];
		if(g.limit > 65536)
		{
			/* make sure the limit can be encoded */
			if((g.limit & 0xfff) != 0xfff)
				return -1;
			g.limit = g.limit >> 12;
			buf[6] = 0xc0;
		}
		else
			buf[6] = 0x40;
		//encode the limit
		buf[0] = g.limit & 0xff;
		buf[1] = (g.limit >> 8) & 0xff;
		buf[6] |= (g.limit >> 16) & 0xf;
		//encode the base
		buf[2] = g.base & 0xff;
		buf[3] = (g.base >> 8) & 0xff;
		buf[4] = (g.base >> 16) & 0xff;
		buf[7] = (g.base >> 24) & 0xff;
		//encode the type
		buf[5] = g.type;
	}
	__arch_setgdt(_buf, count);
	return 0;
}


/* arch_setgdt64 */
int _arch_setgdt64(GDT const * gdt, size_t count)
{
	uint8_t * buf;
	size_t i;
	GDT g;

	memset(&_buf, 0, sizeof(_buf));
	/* check for errors */
	if(count == 0 || count > sizeof(_buf) / sizeof(*gdt))
	{
		errno = ERANGE;
		return -1;
	}
	for(i = 0; i < count; i++)
	{
		g = gdt[i];
		buf = &_buf[sizeof(g) * i];
		if(g.limit > 65536)
		{
			/* make sure the limit can be encoded */
			if((g.limit & 0xfff) != 0xfff)
				return -1;
			g.limit = g.limit >> 12;
			buf[6] = 0xa0;
		}
		else
			buf[6] = 0x20;
		//encode the limit
		buf[0] = g.limit & 0xff;
		buf[1] = (g.limit >> 8) & 0xff;
		buf[6] |= (g.limit >> 16) & 0xf;
		//encode the base
		buf[2] = g.base & 0xff;
		buf[3] = (g.base >> 8) & 0xff;
		buf[4] = (g.base >> 16) & 0xff;
		buf[7] = (g.base >> 24) & 0xff;
		//encode the type
		buf[5] = g.type;
	}
	__arch_setgdt(_buf, count);
	return 0;
}
#endif
