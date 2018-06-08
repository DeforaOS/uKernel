/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include "stdio.h"


/* private */
/* types */
typedef struct _ukConsole STDIOConsole;

typedef struct _ukConsoleData
{
	ukBus * bus;
} STDIOConsoleData;


/* prototypes */
/* console */
static STDIOConsole * _stdio_console_init(ukBus * bus);

static void _stdio_console_clear(STDIOConsole * console);

static void _stdio_console_print(STDIOConsole * console, char const * str,
		size_t len);


/* variables */
static ukConsoleData _stdio_console_data =
{
	NULL
};

static STDIOConsole _stdio_console =
{
	"stdio",
	_stdio_console_init,
	NULL,
	_stdio_console_clear,
	_stdio_console_print,
	&_stdio_console_data
};


/* functions */
/* console */
/* stdio_console_init */
static ukConsole * _stdio_console_init(ukBus * bus)
{
	_stdio_console_data.bus = bus;
	_stdio_console_clear(&_stdio_console);
	return &_stdio_console;
}


/* stdio_console_clear */
static void _stdio_console_clear(STDIOConsole * console)
{
	/* FIXME implement */
}


/* stdio_console_print */
static void _stdio_console_print(STDIOConsole * console, char const * str,
		size_t len)
{
	STDIOConsoleData * data = console->data;
	size_t i;

	for(i = 0; i < len; i++)
		data->bus->write8(data->bus, STDIO_ADDRESS_STDOUT,
				(uint8_t)str[i]);
}
