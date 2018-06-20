/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "drivers/console.h"

#include "drivers/console/stdio.c"


/* private */
/* variables */
static ukConsole * _console = NULL;

static char _console_buf[1024];
static size_t _console_buf_cnt = 0;


/* functions */
/* console_init */
ukConsole * console_init(ukBus * bus, char const * name)
{
	ukConsole * console = NULL;

	if(strcmp(name, "stdio") == 0)
	{
		printf("%s%s%s\n", name, (bus != NULL) ? " at " : "",
				(bus != NULL) ? bus->name : "");
		console = _stdio_console_init(bus);
	}
	else
	{
		errno = ENODEV;
		return NULL;
	}
	if(_console == NULL)
		_console = console;
	_console->print(_console, _console_buf, _console_buf_cnt);
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
	size_t s;

	if(console == NULL && (console = console_get_default()) == NULL)
	{
		s = sizeof(_console_buf) - _console_buf_cnt;
		s = (s > len) ? len : s;
		strncpy(&_console_buf[_console_buf_cnt], str, s);
		_console_buf_cnt += s;
	}
	else
		console->print(console, str, len);
}
