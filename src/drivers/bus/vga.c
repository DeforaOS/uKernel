/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "kernel/intr.h"
#include "drivers/bus.h"


#if defined(__amd64__) || defined(__i386__)
/* private */
/* types */
typedef struct _ukBus VGABus;
typedef struct _ukBusDriver VGABusDriver;

typedef struct _ukBusDriver
{
	ukBus * parent;
	ukBusAddress base;
} VGABusDriver;


/* prototypes */
static VGABusDriver * _vga_bus_init(ukBus * parent, va_list ap);
static void _vga_bus_destroy(VGABus * bus);

static int _vga_bus_read8(VGABus * bus, ukBusAddress address, uint8_t * value);
static int _vga_bus_read16(VGABus * bus, ukBusAddress address,
		uint16_t * value);
static int _vga_bus_read32(VGABus * bus, ukBusAddress address,
		uint32_t * value);
static int _vga_bus_write8(VGABus * bus, ukBusAddress address, uint8_t value);
static int _vga_bus_write16(VGABus * bus, ukBusAddress address, uint16_t value);
static int _vga_bus_write32(VGABus * bus, ukBusAddress address, uint32_t value);

static int _vga_bus_command(VGABus * bus, uint32_t command, va_list ap);


/* variables */
const ukBusInterface vga_bus =
{
	"vga",
	_vga_bus_init,
	_vga_bus_destroy,
	_vga_bus_read8,
	_vga_bus_read16,
	_vga_bus_read32,
	_vga_bus_write8,
	_vga_bus_write16,
	_vga_bus_write32,
	_vga_bus_command
};


/* private */
/* functions */
/* bus */
/* vga_bus_init */
static VGABusDriver * _vga_bus_init(ukBus * parent, va_list ap)
{
	VGABusDriver * vga;
	(void) ap;

	if(parent == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((vga = malloc(sizeof(*vga))) == NULL)
		return NULL;
	vga->parent = parent;
	vga->base = 0x3d4;
	return vga;
}


/* vga_bus_destroy */
static void _vga_bus_destroy(VGABus * bus)
{
	free(bus->driver);
}


/* vga_bus_read8 */
static int _vga_bus_read8(VGABus * bus, ukBusAddress address, uint8_t * value)
{
	int ret;
	VGABusDriver * vga = bus->driver;
	ukBus * parent = bus->driver->parent;

	ret = (parent->interface->write8(parent, vga->base, address) == 0
			&& parent->interface->read8(parent, vga->base + 1,
				value) == 0) ? 0 : -1;
	return ret;
}


/* vga_bus_read16 */
static int _vga_bus_read16(VGABus * bus, ukBusAddress address, uint16_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* vga_bus_read32 */
static int _vga_bus_read32(VGABus * bus, ukBusAddress address, uint32_t * value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* vga_bus_write8 */
static int _vga_bus_write8(VGABus * bus, ukBusAddress address, uint8_t value)
{
	int ret;
	VGABusDriver * vga = bus->driver;
	ukBus * parent = vga->parent;

	ret = (parent->interface->write8(parent, vga->base, address) == 0
			&& parent->interface->write8(parent, vga->base + 1,
				value) == 0) ? 0 : -1;
	return ret;
}


/* vga_bus_write16 */
static int _vga_bus_write16(VGABus * bus, ukBusAddress address, uint16_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* vga_bus_write32 */
static int _vga_bus_write32(VGABus * bus, ukBusAddress address, uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}


/* vga_bus_command */
static int _vga_bus_command(VGABus * bus, uint32_t command, va_list ap)
{
	VGABusDriver * driver = bus->driver;
	ukBus * parent = driver->parent;

	return parent->interface->command(parent, command, ap);
}
#endif
