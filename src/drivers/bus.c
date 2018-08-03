/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "bus.h"


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern const ukBusInterface cmos_bus;
extern const ukBusInterface ioport_bus;
extern const ukBusInterface vga_bus;
#endif


/* functions */
/* bus_init */
ukBus * bus_init(ukBus * parent, char const * name, ...)
{
	const ukBusInterface * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&cmos_bus,
		&ioport_bus,
		&vga_bus
#endif
	};
	ukBus * bus;
	va_list ap;
	size_t i;

	if((bus = malloc(sizeof(*bus))) == NULL)
		return NULL;
	va_start(ap, name);
	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			bus->driver = drivers[i]->init(parent, ap);
			break;
		}
	va_end(ap);
	if(bus->driver == NULL)
	{
		if(i == sizeof(drivers) / sizeof(*drivers))
			errno = ENODEV;
		free(bus);
		return NULL;
	}
	return bus;
}


/* accessors */
/* bus_get_name */
char const * bus_get_name(ukBus * bus)
{
	return bus->interface->name;
}
