/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "drivers/pic.h"
#include "drivers/pic/i8259a.h"


/* private */
/* types */
typedef struct _ukPIC i8259aPIC;
typedef struct _ukPICDriver i8259aPICDriver;

struct _ukPICDriver
{
	ukBus * bus;
};


/* prototypes */
/* pic */
static i8259aPICDriver * _i8259a_pic_init(ukBus * bus, va_list ap);
static void _i8259a_pic_destroy(i8259aPIC * pic);


/* public */
/* variables */
/* pic */
const ukPICInterface i8259a_pic =
{
	"i8259a",
	_i8259a_pic_init,
	_i8259a_pic_destroy
};


/* functions */
/* pic */
/* i8259a_pic_init */
static i8259aPICDriver * _i8259a_pic_init(ukBus * bus, va_list ap)
{
	i8259aPICDriver * pic;
	uint8_t master_mask;
	uint8_t slave_mask;
	(void) ap;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((pic = malloc(sizeof(*pic))) == NULL)
		return NULL;
	pic->bus = bus;
	/* save the masks */
	bus->interface->read8(bus, I8259A_MASTER_DATA, &master_mask);
	bus->interface->read8(bus, I8259A_SLAVE_DATA, &slave_mask);
	/* initialize both PICs */
	bus->interface->write8(bus, I8259A_MASTER_COMMAND,
			I8259A_COMMAND_ICW1_INIT + I8259A_COMMAND_ICW1_ICW4);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->interface->write8(bus, I8259A_SLAVE_COMMAND,
			I8259A_COMMAND_ICW1_INIT + I8259A_COMMAND_ICW1_ICW4);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	/* set the vector offsets */
	bus->interface->write8(bus, I8259A_MASTER_DATA, 0x20);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->interface->write8(bus, I8259A_SLAVE_DATA, 0x28);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	/* configure cascading */
	bus->interface->write8(bus, I8259A_MASTER_DATA, 0x4);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->interface->write8(bus, I8259A_SLAVE_DATA, 0x2);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->interface->write8(bus, I8259A_MASTER_DATA, I8259A_COMMAND_ICW4_8086);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->interface->write8(bus, I8259A_SLAVE_DATA, I8259A_COMMAND_ICW4_8086);
	bus->interface->command(bus, BUS_COMMAND_WAIT, NULL);
	/* restore the masks */
	bus->interface->write8(bus, I8259A_MASTER_DATA, master_mask);
	bus->interface->write8(bus, I8259A_SLAVE_COMMAND, slave_mask);
	return pic;
}


/* i8259a_pic_destroy */
static void _i8259a_pic_destroy(i8259aPIC * pic)
{
	i8259aPICDriver * driver = pic->driver;
	ukBus * bus = driver->bus;

	bus->interface->write8(bus, I8259A_SLAVE_DATA, 0xff);
	bus->interface->write8(bus, I8259A_MASTER_DATA, 0xff);
}
