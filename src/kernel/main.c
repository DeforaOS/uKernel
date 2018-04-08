/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include "drivers/bus.h"
#include "drivers/console.h"

#ifndef KERNEL_BUS
# define KERNEL_BUS	"ioport"
#endif
#ifndef KERNEL_CONSOLE
# define KERNEL_CONSOLE	"vga"
#endif


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	ukBus * bus;
	int i;

	bus = bus_init(KERNEL_BUS);
	console_init(bus, KERNEL_CONSOLE);
	puts("Starting DeforaOS...");
	for(i = 0; i < argc; i++)
		printf("%s%s%s", (i > 0) ? " " : "", argv[i],
				(i + 1 == argc) ? "\n" : "");
	return 0;
}
