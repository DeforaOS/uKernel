/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* FIXME:
 * - re-use the VGA code if supported */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/drivers/display.h>
#include "vesa.h"


/* private */
/* types */
typedef struct _ukDisplay VESADisplay;

typedef struct _ukDisplayDriver
{
	ukBus * bus;
} VESADisplayDriver;


/* prototypes */
/* display */
static VESADisplayDriver * _vesa_display_init(ukBus * bus, va_list ap);

static void _vesa_display_clear(VESADisplay * display);

static void _vesa_display_print(VESADisplay * display, char const * str,
		size_t len);


/* variables */
static ukDisplayDriver _vesa_display_driver =
{
	NULL
};


/* public */
/* variables */
/* display */
const ukDisplayInterface vesa_display =
{
	"vesa",
	_vesa_display_init,
	NULL,
	NULL,
	_vesa_display_clear,
	_vesa_display_print
};


/* functions */
/* display */
/* vesa_display_init */
static ukDisplayDriver * _vesa_display_init(ukBus * bus, va_list ap)
{
	(void) ap;
	VESADisplay vesa;

	_vesa_display_driver.bus = bus;
	vesa.interface = &vesa_display;
	vesa.driver = &_vesa_display_driver;
	_vesa_display_clear(&vesa);
	return &_vesa_display_driver;
}


/* vesa_display_clear */
static void _vesa_display_clear(VESADisplay * display)
{
	VESADisplayDriver * vesa = display->driver;

	/* FIXME implement */
}


/* vesa_display_print */
static void _vesa_display_print(VESADisplay * display, char const * str,
		size_t len)
{
	VESADisplayDriver * vesa = display->driver;

	/* FIXME implement */
}
