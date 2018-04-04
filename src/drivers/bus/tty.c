/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>


/* private */
/* types */
typedef struct _ukBus TTYBus;


/* prototypes */
static TTYBus * _tty_bus_init(void);

static int _tty_bus_write8(TTYBus * bus, ukBusAddress * address, uint8_t value);
static int _tty_bus_write16(TTYBus * bus, ukBusAddress * address,
		uint16_t value);
static int _tty_bus_write32(TTYBus * bus, ukBusAddress * address,
		uint32_t value);

/* XXX make this more elegant */
extern int write(int fd, char const * buf, size_t len);


/* variables */
static TTYBus _tty_bus =
{
	_tty_bus_init,
	_tty_bus_write8,
	_tty_bus_write16,
	_tty_bus_write32,
	NULL
};


/* private */
/* functions */
/* bus */
/* tty_bus_init */
static TTYBus * _tty_bus_init(void)
{
	return &_tty_bus;
}


/* tty_bus_write8 */
static int _tty_bus_write8(TTYBus * bus, ukBusAddress * address, uint8_t value)
{
	(void) bus;

	return write((int)address, (char const *)&value, sizeof(value));
}


/* tty_bus_write16 */
static int _tty_bus_write16(TTYBus * bus, ukBusAddress * address,
		uint16_t value)
{
	(void) bus;

	return write((int)address, (char const *)&value, sizeof(value));
}


/* tty_bus_write32 */
static int _tty_bus_write32(TTYBus * bus, ukBusAddress * address,
		uint32_t value)
{
	(void) bus;

	return write((int)address, (char const *)&value, sizeof(value));
}
