/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <errno.h>
#include "drivers/console.h"

#include "drivers/console/stdio.c"


/* private */
/* variables */
static ukConsole * _console = NULL;


/* functions */
/* console_init */
ukConsole * console_init(ukBus * bus, char const * name)
{
	ukConsole * console = NULL;

	if(strcmp(name, "stdio") == 0)
		console = _stdio_console_init(bus);
	else
	{
		errno = ENODEV;
		return NULL;
	}
	if(_console == NULL)
		_console = console;
	return console;
}


/* FIXME code duplication with src/drivers/console.c */
/* accessors */
/* console_get_default */
ukConsole * console_get_default(void)
{
	return _console;
}


/* helpers */
/* console_clear */
void console_clear(ukConsole * console)
{
	if(console == NULL)
		console = console_get_default();
	console->clear(console);
}


/* console_print */
void console_print(ukConsole * console, char const * str, size_t len)
{
	if(console == NULL)
		console = console_get_default();
	console->print(console, str, len);
}
