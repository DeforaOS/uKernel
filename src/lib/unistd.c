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
/* read */
ssize_t read(int fildes, void * buf, size_t count)
{
	if(fildes < 0)
	{
		errno = EINVAL;
		return -1;
	}
	if((size_t)fildes >= sizeof(_fildes) / sizeof(*_fildes))
	{
		errno = EBADF;
		return -1;
	}
	if(count == 0)
		return 0;
	errno = ENOSYS;
	return -1;
}


/* sbrk */
extern int _brk(void * addr);
extern void * end;

void * sbrk(intptr_t increment)
{
	static void * cur = &end;
	void * ptr;

	if(increment == 0)
		return cur;
	if(_brk(cur + increment) == -1)
		return (void *)-1;
	ptr = cur;
	cur += increment;
	return ptr;
}


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
