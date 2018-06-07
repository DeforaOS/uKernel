/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include "drivers/console.h"


/* private */
/* variables */
static ukConsole * _fildes[STDERR_FILENO + 1] = { NULL, NULL, NULL };


/* public */
/* variables */
char ** environ;


/* functions */
/* write */
ssize_t write(int fildes, const void * buf, size_t count)
{
	ukConsole * console;

	if(fildes < 0)
	{
		errno = EINVAL;
		return -1;
	}
	if((size_t)fildes < sizeof(_fildes) / sizeof(*_fildes))
		console = _fildes[fildes];
	else
	{
		errno = EBADF;
		return -1;
	}
	console_print(console, buf, count);
	return count;
}
