/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>


/* mmap */
void * mmap(void * addr, size_t length, int prot, int flags, int fd,
		off_t offset)
{
	void * ret;

	/* enforce W^X */
	if((prot & (PROT_WRITE | PROT_EXEC)) == (PROT_WRITE | PROT_EXEC))
	{
		errno = EPERM;
		return MAP_FAILED;
	}
	/* flags */
	if((flags & MAP_FIXED) == MAP_FIXED)
	{
		/* XXX verify if the address is aligned and allowed */
		if((flags & MAP_ANONYMOUS) == MAP_ANONYMOUS)
		{
			errno = ENOTSUP;
			ret = MAP_FAILED;
		}
		ret = addr;
	}
	else if((flags & MAP_ANONYMOUS) == MAP_ANONYMOUS)
		ret = calloc(1, length);
	else
		ret = malloc(length);
	return (ret != NULL) ? ret : MAP_FAILED;
}


/* mprotect */
int mprotect(void * addr, size_t length, int prot)
{
	if(addr == NULL || length == 0)
	{
		errno = EINVAL;
		return -1;
	}
	/* enforce W^X */
	if((prot & (PROT_WRITE | PROT_EXEC)) == (PROT_WRITE | PROT_EXEC))
	{
		errno = EPERM;
		return MAP_FAILED;
	}
	/* FIXME really implement */
	return 0;
}


/* munmap */
int munmap(void * addr, size_t length)
{
	(void) length;

	if(length == 0)
	{
		errno = EINVAL;
		return -1;
	}
	/* FIXME check the length? */
	free(addr);
	return 0;
}
