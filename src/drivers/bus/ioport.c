/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>


/* private */
/* prototypes */
extern int ioport_write8(Bus * bus, BusAddress * address, uint8_t value);
static int _ioport_write16(Bus * bus, BusAddress * address, uint16_t value);
static int _ioport_write32(Bus * bus, BusAddress * address, uint32_t value);


/* variables */
static Bus _ioport_bus =
{
	ioport_write8,
	_ioport_write16,
	_ioport_write32,
	NULL
};


/* public */
/* functions */
/* bus_init */
Bus * bus_init(void)
{
	return &_ioport_bus;
}


/* useful */
/* ioport_write16 */
static int _ioport_write16(Bus * bus, BusAddress * address, uint16_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}


/* ioport_write32 */
static int _ioport_write32(Bus * bus, BusAddress * address, uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}
