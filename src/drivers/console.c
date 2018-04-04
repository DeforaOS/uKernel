/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include "console.h"


/* private */
/* variables */
static ukConsole * _console = NULL;


#if defined(__amd64__) || defined(__i386__)
# include "console/vga.c"

/* console_init */
ukConsole * console_init(ukBus * bus)
{
	if((_console = _vga_console_init(bus)) == NULL)
		return NULL;
	return _console;
}
#else
# warning Unsupported platform: missing console
/* console_init */
ukConsole * console_init(ukBus * bus)
{
	errno = ENODEV;
	return NULL;
}
#endif


/* functions */
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
