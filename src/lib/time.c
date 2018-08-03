/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <time.h>
#include "drivers/clock.h"


/* functions */
/* time */
time_t time(time_t * time)
{
	ukClock * clock;
	time_t t;

	if((clock = clock_get_default()) == NULL)
		return -1;
	if(clock_get_time(clock, &t) != 0)
		return -1;
	if(time != NULL)
		*time = t;
	return t;
}
