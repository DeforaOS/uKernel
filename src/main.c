/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/bus.h"
#include "drivers/console.h"


/* public */
/* functions */
/* main */
int main(void)
{
	Bus * bus;
	Console * console;
	const char msg[] = "Starting DeforaOS...\n";

	bus = bus_init();
	console = console_init(bus);
	console_print(console, msg, sizeof(msg) - 1);
	return 0;
}
