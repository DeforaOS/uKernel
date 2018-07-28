/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "kernel/intr.h"
#include "drivers/bus/cmos.h"


#if defined(__amd64__) || defined(__i386__)
/* private */
/* types */
typedef struct _ukBus CMOSBus;
typedef struct _ukBusDriver CMOSBusDriver;

struct _ukBusDriver
{
	ukBus * parent;
};


/* prototypes */
static CMOSBusDriver * _cmos_bus_init(ukBus * parent, va_list ap);
static void _cmos_bus_destroy(CMOSBus * bus);

static int _cmos_bus_read8(CMOSBus * bus, ukBusAddress address,
		uint8_t * value);
static int _cmos_bus_read16(CMOSBus * bus, ukBusAddress address,
		uint16_t * value);
static int _cmos_bus_read32(CMOSBus * bus, ukBusAddress address,
		uint32_t * value);
static int _cmos_bus_write8(CMOSBus * bus, ukBusAddress address, uint8_t value);
static int _cmos_bus_write16(CMOSBus * bus, ukBusAddress address,
		uint16_t value);
static int _cmos_bus_write32(CMOSBus * bus, ukBusAddress address,
		uint32_t value);

static int _cmos_bus_command(CMOSBus * bus, uint32_t command, va_list ap);


/* variables */
const ukBusInterface cmos_bus =
{
	"cmos",
	_cmos_bus_init,
	_cmos_bus_destroy,
	_cmos_bus_read8,
	_cmos_bus_read16,
	_cmos_bus_read32,
	_cmos_bus_write8,
	_cmos_bus_write16,
	_cmos_bus_write32,
	_cmos_bus_command
};


/* private */
/* functions */
/* bus */
/* cmos_bus_init */
static CMOSBusDriver * _cmos_bus_init(ukBus * parent, va_list ap)
{
	CMOSBusDriver * cmos;
	(void) ap;

	if(parent == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((cmos = malloc(sizeof(*cmos))) == NULL)
		return NULL;
	cmos->parent = parent;
	return cmos;
}


/* cmos_bus_destroy */
static void _cmos_bus_destroy(CMOSBus * bus)
{
	free(bus->driver);
}


/* cmos_bus_read8 */
static int _cmos_bus_read8(CMOSBus * bus, ukBusAddress address, uint8_t * value)
{
	int ret;
	ukBus * parent = bus->driver->parent;

	intr_disable();
	ret = (parent->interface->write8(parent, BUS_CMOS_REGISTER_ADDRESS,
				address) == 0
			&& parent->interface->command(parent, BUS_COMMAND_WAIT,
				NULL) == 0
			&& parent->interface->read8(parent,
				BUS_CMOS_REGISTER_DATA, value) == 0) ? 0 : -1;
	intr_enable();
	return ret;
}


/* cmos_bus_read16 */
static int _cmos_bus_read16(CMOSBus * bus, ukBusAddress address,
		uint16_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* cmos_bus_read32 */
static int _cmos_bus_read32(CMOSBus * bus, ukBusAddress address,
		uint32_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* cmos_write8 */
static int _cmos_bus_write8(CMOSBus * bus, ukBusAddress address,
		uint8_t value)
{
	int ret;
	ukBus * parent = bus->driver->parent;

	intr_disable();
	ret = (parent->interface->write8(parent, BUS_CMOS_REGISTER_ADDRESS,
				address) == 0
			&& parent->interface->write8(parent,
				BUS_CMOS_REGISTER_DATA, value) == 0) ? 0 : -1;
	intr_enable();
	return ret;
}


/* cmos_bus_write16 */
static int _cmos_bus_write16(CMOSBus * bus, ukBusAddress address,
		uint16_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* cmos_write32 */
static int _cmos_bus_write32(CMOSBus * bus, ukBusAddress address,
		uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* cmos_bus_command */
static int _cmos_bus_command(CMOSBus * bus, uint32_t command, va_list ap)
{
	ukBus * parent = bus->driver->parent;
	uint8_t u8;

	switch(command)
	{
		case BUS_CMOS_COMMAND_NMI_ENABLE:
			return bus->interface->read8(bus,
					BUS_CMOS_REGISTER_ADDRESS, &u8) == 0
				&& bus->interface->write8(bus,
						BUS_CMOS_REGISTER_ADDRESS,
						BUS_CMOS_COMMAND_NMI_ENABLE
						| (u8 & 0x7f)) == 0
				? 0 : -1;
		case BUS_CMOS_COMMAND_NMI_DISABLE:
			return bus->interface->read8(bus,
					BUS_CMOS_REGISTER_ADDRESS, &u8) == 0
				&& bus->interface->write8(bus,
						BUS_CMOS_REGISTER_ADDRESS,
						BUS_CMOS_COMMAND_NMI_DISABLE
						| (u8 & 0x7f)) == 0
				? 0 : -1;
		default:
			if(parent->interface->command == NULL)
			{
				errno = ENODEV;
				return -1;
			}
			return parent->interface->command(parent, command, ap);
	}
}
#endif
