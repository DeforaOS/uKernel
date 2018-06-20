/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <errno.h>
#include "drivers/bus.h"

#include "drivers/bus/tty.c"


/* bus_init */
ukBus * bus_init(ukBus * parent, char const * name)
{
	ukBus * drivers[] = {
		&tty_bus
	};
	size_t i;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
			return drivers[i]->init(parent);
	errno = ENODEV;
	return NULL;
}
