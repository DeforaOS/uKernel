/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "vga.h"

/* macros */
#define min(a, b) ((a) < (b) ? (a) : (b))


/* private */
/* types */
struct _Console
{
	volatile uint16_t * buf;

	uint8_t color_bg;
	uint8_t color_fg;

	uint8_t pos_x;
	uint8_t pos_y;

	Bus * bus;
};


/* variables */
static Console _vga_console;


/* prototypes */
/* useful */
/* cursor */
static void _vga_cursor_set(Console * console, _Bool enabled,
		size_t row, size_t column);

static void _vga_print(Console * console, unsigned char c,
		size_t row, size_t column);
static void _vga_scroll(Console * console, size_t rows);


/* public */
/* functions */
/* console_init */
Console * console_init(Bus * bus)
{
	_vga_console.buf = (uint16_t *)VGA_ADDRESS_BASE;
	_vga_console.color_bg = VGA_TEXT_COLOR_BLACK;
	_vga_console.color_fg = VGA_TEXT_COLOR_WHITE;
	_vga_console.pos_x = 0;
	_vga_console.pos_y = 0;
	console_clear(&_vga_console);
	_vga_console.bus = bus;
	return &_vga_console;
}


/* useful */
/* console_clear */
void console_clear(Console * console)
{
	size_t row;
	size_t col;

	if(console == NULL)
		console = &_vga_console;
	for(row = 0; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(console, ' ', row, col);
	console->pos_x = 0;
	console->pos_y = 0;
}


/* console_print */
void console_print(Console * console, char const * str, size_t len)
{
	size_t i;

	if(console == NULL)
		console = &_vga_console;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '\n' || console->pos_x == VGA_TEXT_COLUMNS)
		{
			console->pos_x = 0;
			console->pos_y++;
			continue;
		}
		if(console->pos_y == VGA_TEXT_ROWS)
			_vga_scroll(console, 1);
		_vga_print(console, str[i], console->pos_y, console->pos_x++);
	}
	_vga_cursor_set(console, true, console->pos_y,
			min(console->pos_x, VGA_TEXT_COLUMNS));
}


/* private */
/* functions */
/* vga_cursor_set */
static void _vga_cursor_set(Console * console, bool enabled,
		size_t row, size_t column)
{
	uint16_t pos = row * VGA_TEXT_COLUMNS + column;

	if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	bus_write8(&console->bus, 0x3d4, 0x0f);
	bus_write8(&console->bus, 0x3d5, pos & 0xff);
	bus_write8(&console->bus, 0x3d4, 0x0e);
	bus_write8(&console->bus, 0x3d5, pos >> 8);
}


/* vga_print */
static void _vga_print(Console * console, unsigned char c,
		size_t row, size_t column)
{
	uint8_t color;

	if(row >= VGA_TEXT_ROWS || column >= VGA_TEXT_COLUMNS)
		return;
	color = (console->color_bg << 4) | console->color_fg;
	console->buf[row * VGA_TEXT_COLUMNS + column] = c | (color << 8);
}


/* vga_scroll */
static void _vga_scroll(Console * console, size_t rows)
{
	size_t s;

	if(rows == 0)
		return;
	if(rows >= VGA_TEXT_COLUMNS)
	{
		console_clear(console);
		return;
	}
	s = rows * VGA_TEXT_COLUMNS;
	memmove(console->buf, &console->buf[s],
			sizeof(console->buf) - (s * sizeof(*console->buf)));
	console->pos_x = 0;
	console->pos_y = VGA_TEXT_ROWS - rows;
}
