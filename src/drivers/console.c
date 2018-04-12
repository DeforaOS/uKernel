/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <string.h>
#include <errno.h>
#include "console.h"


/* private */
/* variables */
static ukConsole * _console = NULL;


/* functions */
#if defined(__amd64__) || defined(__i386__)
extern ukConsole vesa_console;
extern ukConsole vga_console;

/* console_init */
ukConsole * console_init(ukBus * bus, char const * name)
{
	ukConsole * console;

	if(_console != NULL)
		return _console;
	if(strcmp(name, "vesa") == 0)
		console = &vesa_console;
	else if(strcmp(name, "vga") == 0)
		console = &vga_console;
	else
	{
		errno = ENODEV;
		return NULL;
	}
	console = console->init(bus);
	if(_console == NULL)
		_console = console;
	return console;
}
#else
# warning Unsupported platform: missing console
/* console_init */
ukConsole * console_init(ukBus * bus, char const * name)
{
	(void) bus;
	(void) name;

	errno = ENODEV;
	return NULL;
}
#endif


/* accessors */
/* console_get_default */
ukConsole * console_get_default(void)
{
	if(_console == NULL)
		errno = ENODEV;
	return _console;
}


/* console_set_mode */
int console_set_mode(ukConsole * console, ukConsoleMode mode,
		unsigned int width, unsigned int height, unsigned int depth)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return -1;
	if(console->set_mode == NULL)
	{
		errno = ENOTSUP;
		return -1;
	}
	return console->set_mode(console, mode, width, height, depth);
}


/* helpers */
/* console_clear */
void console_clear(ukConsole * console)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return;
	console->clear(console);
}


/* console_print */
void console_print(ukConsole * console, char const * str, size_t len)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return;
	console->print(console, str, len);
}
