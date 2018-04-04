/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>


/* private */
/* prototypes */
extern int ioport_write8(ukBus * bus, ukBusAddress * address, uint8_t value);
static int _ioport_write16(ukBus * bus, ukBusAddress * address, uint16_t value);
static int _ioport_write32(ukBus * bus, ukBusAddress * address, uint32_t value);


/* variables */
static ukBus _ioport_bus =
{
	ioport_write8,
	_ioport_write16,
	_ioport_write32,
	NULL
};


/* public */
/* functions */
/* ukbus_init */
ukBus * bus_init(void)
{
	return &_ioport_bus;
}


/* private */
/* useful */
/* ioport_write16 */
static int _ioport_write16(ukBus * bus, ukBusAddress * address, uint16_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}


/* ioport_write32 */
static int _ioport_write32(ukBus * bus, ukBusAddress * address, uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}
