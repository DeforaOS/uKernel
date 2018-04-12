/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include "arch/arch.h"


/* private */
/* types */
typedef struct _ukBus IOPortBus;


/* prototypes */
static IOPortBus * _ioport_bus_init(void);
static int _ioport_bus_read8(IOPortBus * bus, ukBusAddress address,
		uint8_t * value);
static int _ioport_bus_read16(IOPortBus * bus, ukBusAddress address,
		uint16_t * value);
static int _ioport_bus_read32(IOPortBus * bus, ukBusAddress address,
		uint32_t * value);
static int _ioport_bus_write8(IOPortBus * bus, ukBusAddress address,
		uint8_t value);
static int _ioport_bus_write16(IOPortBus * bus, ukBusAddress address,
		uint16_t value);
static int _ioport_bus_write32(IOPortBus * bus, ukBusAddress address,
		uint32_t value);


/* variables */
static IOPortBus _ioport_bus =
{
	_ioport_bus_init,
	_ioport_bus_read8,
	_ioport_bus_read16,
	_ioport_bus_read32,
	_ioport_bus_write8,
	_ioport_bus_write16,
	_ioport_bus_write32,
	NULL
};


/* private */
/* functions */
/* bus */
/* ioport_bus_init */
static IOPortBus * _ioport_bus_init(void)
{
	return &_ioport_bus;
}


/* ioport_bus_read8 */
static int _ioport_bus_read8(IOPortBus * bus, ukBusAddress address,
		uint8_t * value)
{
	(void) bus;

	return inb((uint16_t)address, value);
}


/* ioport_bus_read16 */
static int _ioport_bus_read16(IOPortBus * bus, ukBusAddress address,
		uint16_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}


/* ioport_bus_read32 */
static int _ioport_bus_read32(IOPortBus * bus, ukBusAddress address,
		uint32_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}


/* ioport_write8 */
static int _ioport_bus_write8(IOPortBus * bus, ukBusAddress address,
		uint8_t value)
{
	(void) bus;

	return outb((uint16_t)address, value);
}


/* ioport_bus_write16 */
static int _ioport_bus_write16(IOPortBus * bus, ukBusAddress address,
		uint16_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}


/* ioport_write32 */
static int _ioport_bus_write32(IOPortBus * bus, ukBusAddress address,
		uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	return -ENOTSUP;
}
