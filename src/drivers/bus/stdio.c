/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>


/* private */
/* types */
typedef struct _ukBus STDIOBus;

typedef struct _ukBusData
{
	int fd;
} STDIOBusData;


/* prototypes */
static STDIOBus * _stdio_bus_init(void);

/* FIXME pass fd as the address */
static int _stdio_bus_write8(STDIOBus * bus, ukBusAddress * address,
		uint8_t value);
static int _stdio_bus_write16(STDIOBus * bus, ukBusAddress * address,
		uint16_t value);
static int _stdio_bus_write32(STDIOBus * bus, ukBusAddress * address,
		uint32_t value);

/* XXX make this more elegant */
extern int write(int fd, char const * buf, size_t len);


/* variables */
static STDIOBusData _stdio_bus_data =
{
	1				/* standard output */
};


static STDIOBus _stdio_bus =
{
	_stdio_bus_init,
	_stdio_bus_write8,
	_stdio_bus_write16,
	_stdio_bus_write32,
	&_stdio_bus_data
};


/* private */
/* functions */
/* bus */
/* stdio_bus_init */
static STDIOBus * _stdio_bus_init(void)
{
	return &_stdio_bus;
}


/* stdio_bus_write8 */
static int _stdio_bus_write8(STDIOBus * bus, ukBusAddress * address,
		uint8_t value)
{
	STDIOBusData * data = bus->data;
	(void) address;

	return write(data->fd, &value, sizeof(value));
}


/* stdio_bus_write16 */
static int _stdio_bus_write16(STDIOBus * bus, ukBusAddress * address,
		uint16_t value)
{
	STDIOBusData * data = bus->data;
	(void) address;

	return write(data->fd, &value, sizeof(value));
}


/* stdio_bus_write32 */
static int _stdio_bus_write32(STDIOBus * bus, ukBusAddress * address,
		uint32_t value)
{
	STDIOBusData * data = bus->data;
	(void) address;

	return write(data->fd, &value, sizeof(value));
}
