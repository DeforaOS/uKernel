/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include "vga.h"


/* private */
/* types */
struct _Console
{
	volatile uint16_t * buf;

	uint8_t color_bg;
	uint8_t color_fg;

	uint8_t pos_x;
	uint8_t pos_y;
};


/* variables */
static Console _vga_console;


/* prototypes */
static void _vga_print(Console * console, unsigned char c,
		size_t row, size_t column);


/* public */
/* functions */
/* console_init */
Console * console_init(void)
{
	_vga_console.buf = (uint16_t *)VGA_ADDRESS_BASE;
	_vga_console.color_bg = VGA_TEXT_COLOR_BLACK;
	_vga_console.color_fg = VGA_TEXT_COLOR_WHITE;
	_vga_console.pos_x = 0;
	_vga_console.pos_y = 0;
	console_clear(&_vga_console);
	return &_vga_console;
}


/* useful */
/* console_clear */
void console_clear(Console * console)
{
	size_t row;
	size_t col;

	for(row = 0; row < VGA_TEXT_ROWS; row++)
		for(col = 0; col < VGA_TEXT_COLUMNS; col++)
			_vga_print(console, ' ', row, col);
	console->pos_x = 0;
	console->pos_y = 0;
}


/* private */
/* functions */
/* vga_print */
static void _vga_print(Console * console, unsigned char c,
		size_t row, size_t column)
{
	uint8_t color;

	color = (console->color_bg << 4) | console->color_fg;
	console->buf[row * VGA_TEXT_COLUMNS + column] = c | (color << 8);
}
