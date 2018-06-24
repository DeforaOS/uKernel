/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "arch/arch.h"
#include "kernel/intr.h"
#include "../bus.h"


#if defined(__amd64__) || defined(__i386__)
/* private */
/* types */
typedef struct _ukBus CMOSBus;

typedef struct _ukBusData
{
	ukBus * parent;
} CMOSBusData;


/* prototypes */
static CMOSBus * _cmos_bus_init(ukBus * parent);
static void _cmos_bus_destroy(CMOSBus * bus);

static int _cmos_bus_read8(CMOSBus * bus, ukBusAddress address,
		uint8_t * value);
static int _cmos_bus_read16(CMOSBus * bus, ukBusAddress address,
		uint16_t * value);
static int _cmos_bus_read32(CMOSBus * bus, ukBusAddress address,
		uint32_t * value);
static int _cmos_bus_write8(CMOSBus * bus, ukBusAddress address,
		uint8_t value);
static int _cmos_bus_write16(CMOSBus * bus, ukBusAddress address,
		uint16_t value);
static int _cmos_bus_write32(CMOSBus * bus, ukBusAddress address,
		uint32_t value);


/* constants */
#define BUS_CMOS_REGISTER_ADDRESS	0x70
#define BUS_CMOS_REGISTER_DATA		0x71


/* variables */
CMOSBus cmos_bus =
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
	NULL
};


/* private */
/* functions */
/* bus */
/* cmos_bus_init */
static CMOSBus * _cmos_bus_init(ukBus * parent)
{
	CMOSBusData * data;

	if(parent == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((data = malloc(sizeof(*data))) == NULL)
		return NULL;
	data->parent = parent;
	cmos_bus.data = data;
	return &cmos_bus;
}


/* cmos_bus_destroy */
static void _cmos_bus_destroy(CMOSBus * bus)
{
	free(bus->data);
}


/* cmos_bus_read8 */
static int _cmos_bus_read8(CMOSBus * bus, ukBusAddress address,
		uint8_t * value)
{
	int ret;
	CMOSBusData * data = bus->data;

	intr_disable();
	ret = (data->parent->write8(data->parent, BUS_CMOS_REGISTER_ADDRESS,
				address) == 0
			&& data->parent->read8(data->parent,
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
	CMOSBusData * data = bus->data;

	intr_disable();
	ret = (data->parent->write8(data->parent, BUS_CMOS_REGISTER_ADDRESS,
				address) == 0
			&& data->parent->write8(data->parent,
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
#endif
