/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "bus.h"


#if defined(__amd64__) || defined(__i386__)
# include "bus/ioport.c"

/* bus_init */
ukBus * bus_init(void)
{
	static ukBus * bus;

	bus = _ioport_bus_init();
	return bus;
}
#endif
