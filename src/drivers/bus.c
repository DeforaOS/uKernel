/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "bus.h"


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern ukBus cmos_bus;
extern ukBus ioport_bus;
#endif


/* functions */
/* bus_init */
ukBus * bus_init(ukBus * parent, char const * name)
{
	const ukBus * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&cmos_bus,
		&ioport_bus
#endif
	};
	size_t i;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			fprintf(stderr, "%s bus%s%s\n", name,
					(parent != NULL) ? " at " : "",
					(parent != NULL) ? parent->name : "");
			return drivers[i]->init(parent);
		}
	errno = ENODEV;
	return NULL;
}
