/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* FIXME:
 * - re-use the VGA code if supported */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "drivers/display.h"
#include "vesa.h"


/* private */
/* types */
typedef struct _ukDisplay VESADisplay;

typedef struct _ukDisplayData
{
	ukBus * bus;
} VESADisplayData;


/* prototypes */
/* display */
static VESADisplay * _vesa_display_init(ukBus * bus);

static void _vesa_display_clear(VESADisplay * display);

static void _vesa_display_print(VESADisplay * display, char const * str,
		size_t len);


/* variables */
static ukDisplayData _vesa_display_data =
{
	NULL
};


/* public */
/* variables */
/* display */
VESADisplay vesa_display =
{
	"vesa",
	_vesa_display_init,
	NULL,
	NULL,
	_vesa_display_clear,
	_vesa_display_print,
	&_vesa_display_data
};


/* functions */
/* display */
/* vesa_display_init */
static ukDisplay * _vesa_display_init(ukBus * bus)
{
	_vesa_display_data.bus = bus;
	_vesa_display_clear(&vesa_display);
	return &vesa_display;
}


/* vesa_display_clear */
static void _vesa_display_clear(VESADisplay * display)
{
	VESADisplayData * data = display->data;

	/* FIXME implement */
}


/* vesa_display_print */
static void _vesa_display_print(VESADisplay * display, char const * str,
		size_t len)
{
	VESADisplayData * data = display->data;

	/* FIXME implement */
}
