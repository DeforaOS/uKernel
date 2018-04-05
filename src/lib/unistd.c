/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include "drivers/console.h"


/* public */
/* variables */
char ** environ;


/* functions */
/* write */
ssize_t write(int fildes, const void * buf, size_t count)
{
	if(fildes != STDOUT_FILENO)
	{
		errno = EBADF;
		return -1;
	}
	console_print(NULL, buf, count);
	return count;
}
