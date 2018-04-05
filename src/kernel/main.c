/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include "drivers/bus.h"
#include "drivers/console.h"

#ifndef MAIN_BUS
# define MAIN_BUS	"ioport"
#endif


/* public */
/* functions */
/* main */
int main(void)
{
	ukBus * bus;
	const char msg[] = "Starting DeforaOS...\n";

	bus = bus_init(MAIN_BUS);
	console_init(bus);
	puts(msg);
	return 0;
}
