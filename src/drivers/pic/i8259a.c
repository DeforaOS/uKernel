/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include <kernel/drivers/pic.h>
#include "i8259a.h"


/* private */
/* types */
typedef struct _ukPIC i8259aPIC;

typedef struct _ukPICData
{
	ukBus * bus;
} i8259aPICData;


/* prototypes */
/* pic */
static i8259aPIC * _i8259a_pic_init(ukBus * bus);
static void _i8259a_pic_destroy(i8259aPIC * pic);


/* public */
/* variables */
/* pic */
i8259aPIC i8259a_pic =
{
	"i8259a",
	_i8259a_pic_init,
	_i8259a_pic_destroy,
	NULL
};


/* functions */
/* pic */
/* i8259a_pic_init */
static i8259aPIC * _i8259a_pic_init(ukBus * bus)
{
	i8259aPIC * pic = &i8259a_pic;
	uint8_t master_mask;
	uint8_t slave_mask;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((pic->data = malloc(sizeof(*pic->data))) == NULL)
		return NULL;
	pic->data->bus = bus;
	/* save the masks */
	bus->read8(bus, I8259A_MASTER_DATA, &master_mask);
	bus->read8(bus, I8259A_SLAVE_DATA, &slave_mask);
	/* initialize both PICs */
	bus->write8(bus, I8259A_MASTER_COMMAND,
			I8259A_COMMAND_ICW1_INIT + I8259A_COMMAND_ICW1_ICW4);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->write8(bus, I8259A_SLAVE_COMMAND,
			I8259A_COMMAND_ICW1_INIT + I8259A_COMMAND_ICW1_ICW4);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	/* set the vector offsets */
	bus->write8(bus, I8259A_MASTER_DATA, 0x20);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->write8(bus, I8259A_SLAVE_DATA, 0x28);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	/* configure cascading */
	bus->write8(bus, I8259A_MASTER_DATA, 0x4);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->write8(bus, I8259A_SLAVE_DATA, 0x2);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->write8(bus, I8259A_MASTER_DATA, I8259A_COMMAND_ICW4_8086);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	bus->write8(bus, I8259A_SLAVE_DATA, I8259A_COMMAND_ICW4_8086);
	bus->command(bus, BUS_COMMAND_WAIT, NULL);
	/* restore the masks */
	bus->write8(bus, I8259A_MASTER_DATA, master_mask);
	bus->write8(bus, I8259A_SLAVE_COMMAND, slave_mask);
	return pic;
}


/* i8259a_pic_destroy */
static void _i8259a_pic_destroy(i8259aPIC * pic)
{
	i8259aPICData * data = pic->data;

	data->bus->write8(data->bus, I8259A_SLAVE_DATA, 0xff);
	data->bus->write8(data->bus, I8259A_MASTER_DATA, 0xff);
}
