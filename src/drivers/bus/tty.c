/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include "../bus.h"


/* private */
/* types */
typedef struct _ukBus TTYBus;


/* prototypes */
static TTYBus * _tty_bus_init(ukBus * parent);

static int _tty_bus_read8(TTYBus * bus, ukBusAddress address,
		uint8_t * value);
static int _tty_bus_read16(TTYBus * bus, ukBusAddress address,
		uint16_t * value);
static int _tty_bus_read32(TTYBus * bus, ukBusAddress address,
		uint32_t * value);

static int _tty_bus_write8(TTYBus * bus, ukBusAddress address, uint8_t value);
static int _tty_bus_write16(TTYBus * bus, ukBusAddress address,
		uint16_t value);
static int _tty_bus_write32(TTYBus * bus, ukBusAddress address,
		uint32_t value);

/* XXX make this more elegant */
extern int _read(int fildes, void * buf, size_t count);
extern int _write(int fildes, void const * buf, size_t count);


/* variables */
TTYBus tty_bus =
{
	"tty",
	_tty_bus_init,
	NULL,
	_tty_bus_read8,
	_tty_bus_read16,
	_tty_bus_read32,
	_tty_bus_write8,
	_tty_bus_write16,
	_tty_bus_write32,
	NULL,
	NULL
};


/* private */
/* functions */
/* bus */
/* tty_bus_init */
static TTYBus * _tty_bus_init(ukBus * parent)
{
	(void) parent;

	return &tty_bus;
}


/* tty_bus_read8 */
static int _tty_bus_read8(TTYBus * bus, ukBusAddress address,
		uint8_t * value)
{
	(void) bus;

	return _read((int)address, (char *)value, sizeof(value));
}


/* tty_bus_read16 */
static int _tty_bus_read16(TTYBus * bus, ukBusAddress address,
		uint16_t * value)
{
	(void) bus;

	return _read((int)address, (char *)value, sizeof(value));
}


/* tty_bus_read32 */
static int _tty_bus_read32(TTYBus * bus, ukBusAddress address,
		uint32_t * value)
{
	(void) bus;

	return _read((int)address, (char *)value, sizeof(value));
}


/* tty_bus_write8 */
static int _tty_bus_write8(TTYBus * bus, ukBusAddress address, uint8_t value)
{
	(void) bus;

	return _write((int)address, (char const *)&value, sizeof(value));
}


/* tty_bus_write16 */
static int _tty_bus_write16(TTYBus * bus, ukBusAddress address,
		uint16_t value)
{
	(void) bus;

	return _write((int)address, (char const *)&value, sizeof(value));
}


/* tty_bus_write32 */
static int _tty_bus_write32(TTYBus * bus, ukBusAddress address,
		uint32_t value)
{
	(void) bus;

	return _write((int)address, (char const *)&value, sizeof(value));
}
