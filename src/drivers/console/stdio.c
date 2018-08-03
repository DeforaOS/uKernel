/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <errno.h>
#include "drivers/console.h"
#include "stdio.h"


/* private */
/* types */
typedef struct _ukConsole STDIOConsole;

typedef struct _ukConsoleDriver
{
	ukBus * bus;
} STDIOConsoleDriver;


/* prototypes */
/* console */
static STDIOConsoleDriver * _stdio_console_init(ukBus * bus, va_list ap);

static void _stdio_console_clear(STDIOConsole * console);

static void _stdio_console_print(STDIOConsole * console, char const * str,
		size_t len);


/* variables */
static STDIOConsoleDriver _stdio_console_driver =
{
	NULL
};

const ukConsoleInterface stdio_console =
{
	"stdio",
	_stdio_console_init,
	NULL,
	_stdio_console_clear,
	_stdio_console_print
};


/* functions */
/* console */
/* stdio_console_init */
static STDIOConsoleDriver * _stdio_console_init(ukBus * bus, va_list ap)
{
	STDIOConsole stdio;
	(void) ap;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	_stdio_console_driver.bus = bus;
	stdio.interface = &stdio_console;
	stdio.driver = &_stdio_console_driver;
	_stdio_console_clear(&stdio);
	return &_stdio_console_driver;
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
	STDIOConsoleDriver * stdio = console->driver;
	ukBus * bus = stdio->bus;
	size_t i;

	for(i = 0; i < len; i++)
		bus->interface->write8(bus, STDIO_ADDRESS_STDOUT,
				(uint8_t)str[i]);
}
