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

typedef struct _ukBusData
{
	ukBus * parent;
	ukBusAddress base;
} VGABusData;


/* prototypes */
static VGABus * _vga_bus_init(ukBus * parent);
static void _vga_bus_destroy(VGABus * bus);

static int _vga_bus_read8(VGABus * bus, ukBusAddress address, uint8_t * value);
static int _vga_bus_read16(VGABus * bus, ukBusAddress address,
		uint16_t * value);
static int _vga_bus_read32(VGABus * bus, ukBusAddress address,
		uint32_t * value);
static int _vga_bus_write8(VGABus * bus, ukBusAddress address, uint8_t value);
static int _vga_bus_write16(VGABus * bus, ukBusAddress address, uint16_t value);
static int _vga_bus_write32(VGABus * bus, ukBusAddress address, uint32_t value);


/* variables */
VGABus vga_bus =
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
	NULL,
	NULL
};


/* private */
/* functions */
/* bus */
/* vga_bus_init */
static VGABus * _vga_bus_init(ukBus * parent)
{
	VGABusData * data;

	if(parent == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((data = malloc(sizeof(*data))) == NULL)
		return NULL;
	data->parent = parent;
	data->base = 0x3d4;
	vga_bus.data = data;
	return &vga_bus;
}


/* vga_bus_destroy */
static void _vga_bus_destroy(VGABus * bus)
{
	free(bus->data);
}


/* vga_bus_read8 */
static int _vga_bus_read8(VGABus * bus, ukBusAddress address, uint8_t * value)
{
	int ret;
	ukBus * parent = bus->data->parent;

	ret = (parent->write8(parent, bus->data->base, address) == 0
			&& parent->read8(parent, bus->data->base + 1,
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


/* vga_write8 */
static int _vga_bus_write8(VGABus * bus, ukBusAddress address, uint8_t value)
{
	int ret;
	VGABusData * data = bus->data;

	ret = (data->parent->write8(data->parent, bus->data->base, address) == 0
			&& data->parent->write8(data->parent,
				bus->data->base + 1, value) == 0) ? 0 : -1;
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


/* vga_write32 */
static int _vga_bus_write32(VGABus * bus, ukBusAddress address, uint32_t value)
{
	(void) bus;
	(void) address;
	(void) value;

	errno = ENOTSUP;
	return -1;
}
#endif
