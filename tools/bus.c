/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "drivers/bus/tty.c"


/* bus_init */
ukBus * bus_init(ukBus * parent, char const * name, ...)
{
	const ukBusInterface * drivers[] = {
		&tty_bus
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
			bus->interface = drivers[i];
			bus->driver = drivers[i]->init(parent, ap);
			break;
		}
	va_end(ap);
	if(bus == NULL)
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
