/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include "../bus.h"


/* private */
/* types */
typedef struct _ukBus TTYBus;
typedef struct _ukBusDriver TTYBusDriver;


/* prototypes */
static TTYBusDriver * _tty_bus_init(ukBus * parent, va_list);

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

static int _tty_bus_command(TTYBus * bus, uint32_t command, va_list ap);

/* XXX make this more elegant */
extern int _read(int fildes, void * buf, size_t count);
extern int _write(int fildes, void const * buf, size_t count);


/* variables */
const ukBusInterface tty_bus =
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
	NULL
};


/* private */
/* functions */
/* bus */
/* tty_bus_init */
static TTYBusDriver * _tty_bus_init(ukBus * parent, va_list ap)
{
	(void) parent;
	(void) ap;

	return (TTYBusDriver *)1;
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


/* tty_bus_command */
static int _tty_bus_command(TTYBus * bus, uint32_t command, va_list ap)
{
	(void) bus;
	(void) command;
	(void) ap;

	errno = ENOTSUP;
	return -1;
}
