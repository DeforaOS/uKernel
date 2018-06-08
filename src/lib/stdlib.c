/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


/* public */
/* functions */
/* abort */
void abort(void)
{
	_exit(125);
}


/* abs */
int abs(int x)
{
	return (x >= 0) ? x : -x;
}


/* arc4random */
uint32_t arc4random(void)
{
	/* FIXME really implement */
	return 0x41414141;
}


/* exit */
void exit(int status)
{
	_exit(status);
}


/* free */
void free(void * ptr)
{
}


/* labs */
long labs(long x)
{
	return (x >= 0) ? x : -x;
}


/* llabs */
long long llabs(long long x)
{
	return (x >= 0) ? x : -x;
}


/* malloc */
void * malloc(size_t size)
{
	if(size == 0)
		return NULL;
	errno = ENOMEM;
	return NULL;
}
