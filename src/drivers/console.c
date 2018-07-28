/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "console.h"


/* private */
/* variables */
static ukConsole _console = { NULL, NULL };

static char _console_buf[1024];
static size_t _console_buf_cnt = 0;


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern const ukConsoleInterface uart_console;
#endif


/* functions */
/* console_init */
ukConsole * console_init(ukBus * bus, char const * name, ...)
{
	const ukConsoleInterface * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&uart_console,
#endif
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
			fprintf(stderr, "%s console%s%s%s\n", name,
					(bus != NULL) ? " at " : "",
					(bus != NULL) ? bus_get_name(bus) : "",
					(bus != NULL) ? " bus" : "");
			_console.interface = drivers[i];
			_console.driver = drivers[i]->init(bus, ap);
			break;
		}
	va_end(ap);
	if(_console.driver == NULL)
	{
		if(i == sizeof(drivers) / sizeof(*drivers))
			errno = ENODEV;
		return NULL;
	}
	_console.interface->print(&_console, _console_buf, _console_buf_cnt);
	return &_console;
}


/* console_destroy */
void console_destroy(ukConsole * console)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return;
	if(console->interface->destroy != NULL)
		console->interface->destroy(console);
}


/* accessors */
/* console_get_default */
ukConsole * console_get_default(void)
{
	if(_console.driver == NULL)
		errno = ENODEV;
	return &_console;
}


/* helpers */
/* console_clear */
void console_clear(ukConsole * console)
{
	if(console == NULL
			&& (console = console_get_default()) == NULL)
		return;
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
	else
		console->interface->print(console, str, len);
}
