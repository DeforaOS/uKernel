/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* FIXME:
 * - re-use the VGA code if supported */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "drivers/console.h"
#include "vesa.h"


/* private */
/* types */
typedef struct _ukConsole VESAConsole;

typedef struct _ukConsoleData
{
	ukBus * bus;
} VESAConsoleData;


/* prototypes */
/* console */
static VESAConsole * _vesa_console_init(ukBus * bus);

static void _vesa_console_clear(VESAConsole * console);

static void _vesa_console_print(VESAConsole * console, char const * str,
		size_t len);


/* variables */
static ukConsoleData _vesa_console_data =
{
	NULL
};


/* public */
/* variables */
/* console */
VESAConsole vesa_console =
{
	"vesa",
	_vesa_console_init,
	NULL,
	_vesa_console_clear,
	_vesa_console_print,
	&_vesa_console_data
};


/* functions */
/* console */
/* vesa_console_init */
static ukConsole * _vesa_console_init(ukBus * bus)
{
	_vesa_console_data.bus = bus;
	_vesa_console_clear(&vesa_console);
	return &vesa_console;
}


/* vesa_console_clear */
static void _vesa_console_clear(VESAConsole * console)
{
	VESAConsoleData * data = console->data;

	/* FIXME implement */
}


/* vesa_console_print */
static void _vesa_console_print(VESAConsole * console, char const * str,
		size_t len)
{
	VESAConsoleData * data = console->data;

	/* FIXME implement */
}
