/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* TODO:
 * - document the values used */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <kernel/drivers/display.h>
#include "vga.h"

/* macros */
#define min(a, b) ((a) < (b) ? (a) : (b))


/* private */
/* types */
typedef struct _ukDisplay VGADisplay;

typedef struct _ukDisplayDriver
{
	volatile uint16_t * buf;

	uint8_t color_bg;
	uint8_t color_fg;

	bool cursor;

	uint8_t pos_x;
	uint8_t pos_y;

	ukBus * bus;
} VGADisplayDriver;


/* prototypes */
/* display */
static VGADisplayDriver * _vga_display_init(ukBus * bus, va_list ap);

static void _vga_display_clear(VGADisplay * display);

static void _vga_display_print(VGADisplay * display, char const * str,
		size_t len);

/* useful */
/* cursor */
static void _vga_cursor_set(VGADisplay * display, _Bool enabled,
		size_t row, size_t column);

static void _vga_print(VGADisplay * display, unsigned char c,
		size_t row, size_t column);
static void _vga_scroll(VGADisplay * display, size_t rows);


/* variables */
static VGADisplayDriver _vga_display_driver =
{
	(uint16_t *)VGA_ADDRESS_BASE,
	VGA_TEXT_COLOR_BLACK,
	VGA_TEXT_COLOR_WHITE,
	false,
	0,
	0,
	NULL
};


/* public */
/* variables */
/* display */
const ukDisplayInterface vga_display =
{
	"vga",
	_vga_display_init,
	NULL,
	NULL,
	_vga_display_clear,
	_vga_display_print
};


/* functions */
/* display */
/* vga_display_init */
static ukDisplayDriver * _vga_display_init(ukBus * bus, va_list ap)
{
	VGADisplay display = { &vga_display, &_vga_display_driver };
	(void) ap;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	_vga_display_driver.bus = bus;
	_vga_display_clear(&display);
	/* reset the cursor */
	_vga_cursor_set(&display, !_vga_display_driver.cursor, 0, 0);
	_vga_cursor_set(&display, true, 0, 0);
	return display.driver;
}


/* vga_display_clear */
static void _vga_display_clear(VGADisplay * display)
{
	VGADisplayDriver * vga = display->driver;
	size_t row;
	size_t col;

	for(row = 0; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(display, ' ', row, col);
	vga->pos_x = 0;
	vga->pos_y = 0;
}


/* vga_display_print */
static void _vga_display_print(VGADisplay * display, char const * str,
		size_t len)
{
	VGADisplayDriver * vga = display->driver;
	size_t i;

	for(i = 0; i < len; i++)
	{
		if(str[i] == '\n' || vga->pos_x == VGA_TEXT_COLUMNS)
		{
			vga->pos_x = 0;
			vga->pos_y++;
			continue;
		}
		else if(str[i] == '\r')
		{
			vga->pos_x = 0;
			continue;
		}
		if(vga->pos_y == VGA_TEXT_ROWS)
			_vga_scroll(display, 1);
		_vga_print(display, str[i], vga->pos_y, vga->pos_x++);
	}
	_vga_cursor_set(display, vga->cursor, vga->pos_y,
			min(vga->pos_x, VGA_TEXT_COLUMNS));
}


/* useful */
/* cursor */
/* vga_cursor_set */
static void _vga_cursor_set(VGADisplay * display, bool enabled,
		size_t row, size_t column)
{
	VGADisplayDriver * vga = display->driver;
	ukBus * bus = vga->bus;
	uint16_t pos = row * VGA_TEXT_COLUMNS + column;
	uint8_t u8;

	if(enabled == false)
	{
		/* disable the cursor if necessary */
		if(vga->cursor == false)
			return;
		bus->interface->read8(bus, VGA_REGISTER_CURSOR_START, &u8);
		bus->interface->write8(bus, VGA_REGISTER_CURSOR_START,
				u8 | 0x20);
	}
	else if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	else
	{
		/* position the cursor */
		bus->interface->write8(bus, VGA_REGISTER_CURSOR_LOCATION_LOW,
				pos & 0xff);
		bus->interface->write8(bus, VGA_REGISTER_CURSOR_LOCATION_HIGH,
				pos >> 8);
		/* enable the cursor if necessary */
		if(vga->cursor == false)
		{
			bus->interface->read8(bus, VGA_REGISTER_CURSOR_START,
					&u8);
			bus->interface->write8(bus, VGA_REGISTER_CURSOR_START,
					u8 & ~0x20);
		}
	}
	vga->cursor = enabled;
}


/* vga_print */
static void _vga_print(VGADisplay * display, unsigned char c,
		size_t row, size_t column)
{
	VGADisplayDriver * vga = display->driver;
	uint8_t color;

	if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	color = (vga->color_bg << 4) | vga->color_fg;
	vga->buf[row * VGA_TEXT_COLUMNS + column] = c | (color << 8);
}


/* vga_scroll */
static void _vga_scroll(VGADisplay * display, size_t rows)
{
	VGADisplayDriver * vga = display->driver;
	size_t s;
	size_t row;
	size_t col;

	if(rows == 0)
		return;
	if(rows >= VGA_TEXT_ROWS)
	{
		display_clear(display);
		return;
	}
	s = rows * VGA_TEXT_COLUMNS;
	memmove(vga->buf, &vga->buf[s], sizeof(*vga->buf) * VGA_TEXT_COLUMNS
			* VGA_TEXT_ROWS - (s * sizeof(*vga->buf)));
	/* clear the last lines */
	for(row = VGA_TEXT_ROWS - rows; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(display, ' ', row, col);
	vga->pos_x = 0;
	vga->pos_y = VGA_TEXT_ROWS - rows;
}
