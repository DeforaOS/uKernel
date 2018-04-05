/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include "drivers/bus.h"

#include "drivers/bus/tty.c"


/* bus_init */
ukBus * bus_init(char const * name)
{
	if(strcmp(name, "tty") == 0)
		return _tty_bus_init();
	errno = ENODEV;
	return NULL;
}
