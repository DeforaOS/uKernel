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
	char tty[] = "tty";

	if(strncmp(name, tty, sizeof(tty) - 1) == 0)
		return tty_bus.init(parent);
	errno = ENODEV;
	return NULL;
}
