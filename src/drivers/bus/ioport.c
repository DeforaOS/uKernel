/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include <kernel/drivers/bus.h>
#include "arch/arch.h"


#if defined(__amd64__) || defined(__i386__)
/* private */
/* types */
typedef struct _ukBus IOPortBus;
typedef struct _ukBusDriver IOPortBusDriver;


/* prototypes */
static IOPortBusDriver * _ioport_bus_init(ukBus * parent, va_list ap);

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

static int _ioport_bus_command(IOPortBus * bus, uint32_t command, va_list ap);


/* variables */
const ukBusInterface ioport_bus =
{
	"ioport",
	_ioport_bus_init,
	NULL,
	_ioport_bus_read8,
	_ioport_bus_read16,
	_ioport_bus_read32,
	_ioport_bus_write8,
	_ioport_bus_write16,
	_ioport_bus_write32,
	_ioport_bus_command
};


/* private */
/* functions */
/* bus */
/* ioport_bus_init */
static IOPortBusDriver * _ioport_bus_init(ukBus * parent, va_list ap)
{
	(void) parent;
	(void) ap;

	return (IOPortBusDriver *)1;
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

	errno = ENOTSUP;
	return -1;
}


/* ioport_bus_read32 */
static int _ioport_bus_read32(IOPortBus * bus, ukBusAddress address,
		uint32_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
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

	errno = ENOTSUP;
	return -1;
}


/* ioport_write32 */
static int _ioport_bus_write32(IOPortBus * bus, ukBusAddress address,
		uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* ioport_bus_command */
static int _ioport_bus_command(IOPortBus * bus, uint32_t command, va_list ap)
{
	(void) bus;
	(void) ap;

	switch(command)
	{
		case BUS_COMMAND_WAIT:
			return iowait();
		default:
			errno = ENOTSUP;
			return -1;
	}
}
#endif
