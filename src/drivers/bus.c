/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include "bus.h"


#if defined(__amd64__) || defined(__i386__)
# include "bus/ioport.c"
#endif


/* bus_init */
ukBus * bus_init(ukBus * parent, char const * name)
{
	ukBus * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&ioport_bus
#endif
	};
	size_t i;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			printf("%s%s%s\n", name, (parent != NULL) ? " at " : "",
					(parent != NULL) ? parent->name : "");
			return drivers[i]->init(parent);
		}
	errno = ENODEV;
	return NULL;
}
