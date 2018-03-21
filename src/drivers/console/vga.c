/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>


/* types */
struct _Console
{
	volatile uint16_t * buf;
};


/* constants */
#define VGA_ADDRESS_BASE	0xb8000

#define VGA_TEXT_COLUMNS	80
#define VGA_TEXT_ROWS		25


/* variables */
static Console _vga_console;


/* functions */
/* console_init */
Console * console_init(void)
{
	_vga_console.buf = (uint16_t *)VGA_ADDRESS_BASE;
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
			console->buf[row * VGA_TEXT_COLUMNS + col]
				= (' ' | (0x00 << 8));
}
