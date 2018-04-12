/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* TODO:
 * - document the values used */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "drivers/console.h"
#include "vga.h"

/* macros */
#define min(a, b) ((a) < (b) ? (a) : (b))


/* private */
/* types */
typedef struct _ukConsole VGAConsole;

typedef struct _ukConsoleData
{
	volatile uint16_t * buf;

	uint8_t color_bg;
	uint8_t color_fg;

	bool cursor;

	uint8_t pos_x;
	uint8_t pos_y;

	ukBus * bus;
} VGAConsoleData;


/* prototypes */
/* console */
static VGAConsole * _vga_console_init(ukBus * bus);

static void _vga_console_clear(VGAConsole * console);

static void _vga_console_print(VGAConsole * console, char const * str,
		size_t len);

/* useful */
/* cursor */
static void _vga_cursor_set(VGAConsole * console, _Bool enabled,
		size_t row, size_t column);

static void _vga_print(VGAConsole * console, unsigned char c,
		size_t row, size_t column);
static void _vga_scroll(VGAConsole * console, size_t rows);


/* variables */
static ukConsoleData _vga_console_data =
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
/* console */
VGAConsole vga_console =
{
	_vga_console_init,
	NULL,
	_vga_console_clear,
	_vga_console_print,
	&_vga_console_data
};


/* functions */
/* console */
/* vga_console_init */
static ukConsole * _vga_console_init(ukBus * bus)
{
	_vga_console_data.bus = bus;
	_vga_console_clear(&vga_console);
	/* reset the cursor */
	_vga_cursor_set(&vga_console, !_vga_console_data.cursor, 0, 0);
	_vga_cursor_set(&vga_console, true, 0, 0);
	return &vga_console;
}


/* vga_console_clear */
static void _vga_console_clear(VGAConsole * console)
{
	VGAConsoleData * data = console->data;
	size_t row;
	size_t col;

	for(row = 0; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(console, ' ', row, col);
	data->pos_x = 0;
	data->pos_y = 0;
}


/* vga_console_print */
static void _vga_console_print(VGAConsole * console, char const * str,
		size_t len)
{
	VGAConsoleData * data = console->data;
	size_t i;

	for(i = 0; i < len; i++)
	{
		if(str[i] == '\n' || data->pos_x == VGA_TEXT_COLUMNS)
		{
			data->pos_x = 0;
			data->pos_y++;
			continue;
		}
		if(data->pos_y == VGA_TEXT_ROWS)
			_vga_scroll(console, 1);
		_vga_print(console, str[i], data->pos_y, data->pos_x++);
	}
	_vga_cursor_set(console, data->cursor, data->pos_y, min(data->pos_x,
				VGA_TEXT_COLUMNS));
}


/* useful */
/* cursor */
/* vga_cursor_set */
static void _vga_cursor_set(VGAConsole * console, bool enabled,
		size_t row, size_t column)
{
	VGAConsoleData * data = console->data;
	uint16_t pos = row * VGA_TEXT_COLUMNS + column;
	uint8_t u8;

	if(enabled == false)
	{
		/* disable the cursor if necessary */
		if(data->cursor == false)
			return;
		data->bus->write8(data->bus, (ukBusAddress *)0x3d4, 0x0a);
		data->bus->read8(data->bus, (ukBusAddress *)0x3d5, &u8);
		data->bus->write8(data->bus, (ukBusAddress *)0x3d5, u8 | 0x20);
	}
	else if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	else
	{
		/* position the cursor */
		data->bus->write8(data->bus, (ukBusAddress *)0x3d4, 0x0f);
		data->bus->write8(data->bus, (ukBusAddress *)0x3d5, pos & 0xff);
		data->bus->write8(data->bus, (ukBusAddress *)0x3d4, 0x0e);
		data->bus->write8(data->bus, (ukBusAddress *)0x3d5, pos >> 8);
		/* enable the cursor if necessary */
		if(data->cursor == false)
		{
			data->bus->write8(data->bus, (ukBusAddress *)0x3d4,
					0x0a);
			data->bus->read8(data->bus, (ukBusAddress *)0x3d5, &u8);
			data->bus->write8(data->bus, (ukBusAddress *)0x3d5,
					u8 & ~0x20);
		}
	}
	data->cursor = enabled;
}


/* vga_print */
static void _vga_print(VGAConsole * console, unsigned char c,
		size_t row, size_t column)
{
	VGAConsoleData * data = console->data;
	uint8_t color;

	if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	color = (data->color_bg << 4) | data->color_fg;
	data->buf[row * VGA_TEXT_COLUMNS + column] = c | (color << 8);
}


/* vga_scroll */
static void _vga_scroll(VGAConsole * console, size_t rows)
{
	VGAConsoleData * data = console->data;
	size_t s;
	size_t row;
	size_t col;

	if(rows == 0)
		return;
	if(rows >= VGA_TEXT_ROWS)
	{
		console_clear(console);
		return;
	}
	s = rows * VGA_TEXT_COLUMNS;
	memmove(data->buf, &data->buf[s],
			sizeof(*data->buf) * VGA_TEXT_COLUMNS * VGA_TEXT_ROWS
			- (s * sizeof(*data->buf)));
	/* clear the last lines */
	for(row = VGA_TEXT_ROWS - rows; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(console, ' ', row, col);
	data->pos_x = 0;
	data->pos_y = VGA_TEXT_ROWS - rows;
}
