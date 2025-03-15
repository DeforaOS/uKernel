/* $Id$ */
/* Copyright (c) 2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <sys/time.h>
#include <kernel/drivers/clock.h>


/* public */
/* functions */
/* gettimeofday */
int gettimeofday(struct timeval * tv, void * null)
{
	time_t t;

	if(clock_get_time(NULL, &t) != 0)
		return -1;
	tv->tv_sec = t;
	tv->tv_usec = 0;
	return 0;
}
