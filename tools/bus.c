/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/bus.h"

#include "drivers/bus/stdio.c"


/* bus_init */
ukBus * bus_init(void)
{
	static ukBus * bus;

	bus = _stdio_bus_init();
	return bus;
}
