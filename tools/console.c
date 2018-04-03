/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <unistd.h>
#include <stdint.h>
#include "drivers/console.h"


/* types */
struct _Console
{
	int fd;
};


/* prototypes */
int _write(int fd, char const * buf, size_t len);


/* variables */
static Console _console;


/* functions */
/* console_init */
Console * console_init(Bus * bus)
{
	_console.fd = STDOUT_FILENO;
	return &_console;
}


/* useful */
/* console_clear */
void console_clear(Console * console)
{
}


/* console_print */
void console_print(Console * console, char const * text, size_t len)
{
	_write(console->fd, text, len);
}
