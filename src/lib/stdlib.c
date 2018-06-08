/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <unistd.h>
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
