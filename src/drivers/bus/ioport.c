/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <errno.h>


/* private */
/* types */
struct _Bus
{
};


/* variables */
static Bus _ioport_bus;


/* public */
/* functions */
/* bus_init */
Bus * bus_init(void)
{
	return &_ioport_bus;
}


/* useful */
/* bus_write16 */
int bus_write16(Bus * bus, void * addr, uint16_t value)
{
	(void) bus;
	(void) addr;
	(void) value;

	return -ENOTSUP;
}


/* bus_write32 */
int bus_write32(Bus * bus, void * addr, uint32_t value)
{
	(void) bus;
	(void) addr;
	(void) value;

	return -ENOTSUP;
}
