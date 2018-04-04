/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/bus.h"

#include "drivers/bus/tty.c"


/* bus_init */
ukBus * bus_init(void)
{
	static ukBus * bus;

	bus = _tty_bus_init();
	return bus;
}
