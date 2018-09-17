/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <kernel/drivers/console.h>
#include <kernel/drivers/display.h>
#include <kernel/platform.h>


/* private */
/* variables */
static char const * _consolebus = NULL;
static char const * _console = NULL;
static char const * _displaybus = NULL;
static char const * _display = NULL;


/* prototypes */
static ukBus * _platform_get_bus(char const * bus);


/* public */
/* functions */
/* platform_init */
void platform_init(void)
{
	ukBus * bus;

	_platform_init_arch();
	bus = (_displaybus != NULL) ? _platform_get_bus(_displaybus) : NULL;
	display_init(bus, _display);
	bus = (_consolebus != NULL) ? _platform_get_bus(_consolebus) : NULL;
	console_init(bus, _console);
}


/* accessors */
/* platform_set_console */
void platform_set_console(char const * bus, char const * name)
{
	_consolebus = bus;
	_console = name;
}


/* platform_set_display */
void platform_set_display(char const * bus, char const * name)
{
	_displaybus = bus;
	_display = name;
}


/* private */
/* functions */
/* platform_get_bus */
static ukBus * _platform_get_bus(char const * bus)
{
	/* FIXME implement */
	return NULL;
}
