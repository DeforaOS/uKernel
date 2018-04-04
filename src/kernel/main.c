/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "../drivers/bus.h"
#include "../drivers/console.h"

#ifndef MAIN_BUS
# define MAIN_BUS	"ioport"
#endif


/* public */
/* functions */
/* main */
int main(void)
{
	ukBus * bus;
	ukConsole * console;
	const char msg[] = "Starting DeforaOS...\n";

	bus = bus_init(MAIN_BUS);
	console = console_init(bus);
	console_print(console, msg, sizeof(msg) - 1);
	return 0;
}
