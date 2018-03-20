/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* FIXME provide <stdint.h> or use a cross-compiler */
#if 0
# include <stdint.h>
#else
typedef short int uint16_t;
typedef long int size_t;
#endif


/* constants */
#define VGA_COLUMNS	80
#define VGA_ROWS	25


/* functions */
/* console_init */
static void _init_clear(void);

int console_init(void)
{
	_init_clear();
	return 0;
}

static void _init_clear(void)
{
	uint16_t * buf = (uint16_t *)0xb8000;
	size_t row;
	size_t col;

	for(row = 0; row < VGA_ROWS; row++)
		for(col = 0; col < VGA_COLUMNS; col++)
			buf[row * VGA_COLUMNS + col] = (' ' | (0x00 << 8));
}
