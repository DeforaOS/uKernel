/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "drivers/console.h"


/* private */
/* variables */
static ukConsole _console = { NULL, NULL };

static char _console_buf[1024];
static size_t _console_buf_cnt = 0;


/* public */
/* variables */
extern const ukConsoleInterface stdio_console;


/* functions */
/* console_init */
ukConsole * console_init(ukBus * bus, char const * name, ...)
{
	const ukConsoleInterface * drivers[] = {
		&stdio_console
	};
	va_list ap;
	size_t i;

	if(_console.driver != NULL)
		return &_console;
	va_start(ap, name);
	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			printf("%s%s%s\n", name, (bus != NULL) ? " at " : "",
					(bus != NULL) ? bus_get_name(bus) : "");
			_console.interface = drivers[i];
			_console.driver = drivers[i]->init(bus, ap);
			break;
		}
	va_end(ap);
	if(_console.driver == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	_console.interface->print(&_console, _console_buf, _console_buf_cnt);
	return &_console;
}


/* FIXME code duplication with src/drivers/console.c */
/* accessors */
/* console_get_default */
ukConsole * console_get_default(void)
{
	if(_console.driver == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	return &_console;
}


/* helpers */
/* console_clear */
void console_clear(ukConsole * console)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return;
	if(console->interface->clear != NULL)
		console->interface->clear(console);
}


/* console_print */
void console_print(ukConsole * console, char const * str, size_t len)
{
	size_t s;

	if(console == NULL && (console = console_get_default()) == NULL)
	{
		s = sizeof(_console_buf) - _console_buf_cnt;
		s = (s > len) ? len : s;
		strncpy(&_console_buf[_console_buf_cnt], str, s);
		_console_buf_cnt += s;
	}
	else if(console->interface->print != NULL)
		console->interface->print(console, str, len);
}
