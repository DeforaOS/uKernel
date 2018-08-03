/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "console.h"


/* private */
/* types */
typedef struct _ukConsole BufferConsole;

/* prototypes */
static void _buffer_console_clear(BufferConsole * console);

static void _buffer_console_print(BufferConsole * console, char const * str,
		size_t len);


/* variables */
static ukConsoleInterface buffer_console =
{
	"buffer",
	NULL,
	NULL,
	_buffer_console_clear,
	_buffer_console_print
};

static ukConsole _console = { &buffer_console, NULL };

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
	_console.interface = drivers[i];
	return &_console;
}


/* console_destroy */
void console_destroy(ukConsole * console)
{
	if(console->interface->destroy != NULL)
		console->interface->destroy(console);
}


/* accessors */
/* console_get_default */
ukConsole * console_get_default(void)
{
	return &_console;
}


/* console_get_name */
char const * console_get_name(ukConsole * console)
{
	return console->interface->name;
}


/* helpers */
/* console_clear */
void console_clear(ukConsole * console)
{
	if(console->interface->clear != NULL)
		console->interface->clear(console);
}


/* console_print */
void console_print(ukConsole * console, char const * str, size_t len)
{
	if(console->interface->print != NULL)
		console->interface->print(console, str, len);
}


/* buffer_console */
/* buffer_console_clear */
static void _buffer_console_clear(BufferConsole * console)
{
	(void) console;

	_console_buf_cnt = 0;
}


/* buffer_console_print */
static void _buffer_console_print(BufferConsole * console, char const * str,
		size_t len)
{
	size_t s;
	(void) console;

	s = sizeof(_console_buf) - _console_buf_cnt;
	s = (s > len) ? len : s;
	strncpy(&_console_buf[_console_buf_cnt], str, s);
	_console_buf_cnt += s;
}
