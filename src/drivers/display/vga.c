/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* TODO:
 * - document the values used */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "drivers/display.h"
#include "vga.h"

/* macros */
#define min(a, b) ((a) < (b) ? (a) : (b))


/* private */
/* types */
typedef struct _ukDisplay VGADisplay;

typedef struct _ukDisplayData
{
	volatile uint16_t * buf;

	uint8_t color_bg;
	uint8_t color_fg;

	bool cursor;

	uint8_t pos_x;
	uint8_t pos_y;

	ukBus * bus;
} VGADisplayData;


/* prototypes */
/* display */
static VGADisplay * _vga_display_init(ukBus * bus);

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
static ukDisplayData _vga_display_data =
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
VGADisplay vga_display =
{
	"vga",
	_vga_display_init,
	NULL,
	NULL,
	_vga_display_clear,
	_vga_display_print,
	&_vga_display_data
};


/* functions */
/* display */
/* vga_display_init */
static ukDisplay * _vga_display_init(ukBus * bus)
{
	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	_vga_display_data.bus = bus;
	_vga_display_clear(&vga_display);
	/* reset the cursor */
	_vga_cursor_set(&vga_display, !_vga_display_data.cursor, 0, 0);
	_vga_cursor_set(&vga_display, true, 0, 0);
	return &vga_display;
}


/* vga_display_clear */
static void _vga_display_clear(VGADisplay * display)
{
	VGADisplayData * data = display->data;
	size_t row;
	size_t col;

	for(row = 0; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(display, ' ', row, col);
	data->pos_x = 0;
	data->pos_y = 0;
}


/* vga_display_print */
static void _vga_display_print(VGADisplay * display, char const * str,
		size_t len)
{
	VGADisplayData * data = display->data;
	size_t i;

	for(i = 0; i < len; i++)
	{
		if(str[i] == '\n' || data->pos_x == VGA_TEXT_COLUMNS)
		{
			data->pos_x = 0;
			data->pos_y++;
			continue;
		}
		else if(str[i] == '\r')
		{
			data->pos_x = 0;
			continue;
		}
		if(data->pos_y == VGA_TEXT_ROWS)
			_vga_scroll(display, 1);
		_vga_print(display, str[i], data->pos_y, data->pos_x++);
	}
	_vga_cursor_set(display, data->cursor, data->pos_y, min(data->pos_x,
				VGA_TEXT_COLUMNS));
}


/* useful */
/* cursor */
/* vga_cursor_set */
static void _vga_cursor_set(VGADisplay * display, bool enabled,
		size_t row, size_t column)
{
	VGADisplayData * data = display->data;
	uint16_t pos = row * VGA_TEXT_COLUMNS + column;
	uint8_t u8;

	if(enabled == false)
	{
		/* disable the cursor if necessary */
		if(data->cursor == false)
			return;
		data->bus->read8(data->bus, VGA_REGISTER_CURSOR_START, &u8);
		data->bus->write8(data->bus, VGA_REGISTER_CURSOR_START,
				u8 | 0x20);
	}
	else if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	else
	{
		/* position the cursor */
		data->bus->write8(data->bus, VGA_REGISTER_CURSOR_LOCATION_LOW,
				pos & 0xff);
		data->bus->write8(data->bus, VGA_REGISTER_CURSOR_LOCATION_HIGH,
				pos >> 8);
		/* enable the cursor if necessary */
		if(data->cursor == false)
		{
			data->bus->read8(data->bus, VGA_REGISTER_CURSOR_START,
					&u8);
			data->bus->write8(data->bus, VGA_REGISTER_CURSOR_START,
					u8 & ~0x20);
		}
	}
	data->cursor = enabled;
}


/* vga_print */
static void _vga_print(VGADisplay * display, unsigned char c,
		size_t row, size_t column)
{
	VGADisplayData * data = display->data;
	uint8_t color;

	if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	color = (data->color_bg << 4) | data->color_fg;
	data->buf[row * VGA_TEXT_COLUMNS + column] = c | (color << 8);
}


/* vga_scroll */
static void _vga_scroll(VGADisplay * display, size_t rows)
{
	VGADisplayData * data = display->data;
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
	memmove(data->buf, &data->buf[s],
			sizeof(*data->buf) * VGA_TEXT_COLUMNS * VGA_TEXT_ROWS
			- (s * sizeof(*data->buf)));
	/* clear the last lines */
	for(row = VGA_TEXT_ROWS - rows; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(display, ' ', row, col);
	data->pos_x = 0;
	data->pos_y = VGA_TEXT_ROWS - rows;
}
