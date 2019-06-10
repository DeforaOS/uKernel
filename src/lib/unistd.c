/* $Id$ */
/* Copyright (c) 2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <kernel/drivers/console.h>


/* public */
/* functions */
/* write */
ssize_t write(int fildes, const void * buf, size_t count)
{
	switch(fildes)
	{
		case 0:
			errno = EPERM;
			return -1;
		case 1:
		case 2:
			console_print(NULL, buf, count);
			return count;
		default:
			errno = ENOSYS;
			return -1;
	}
}
