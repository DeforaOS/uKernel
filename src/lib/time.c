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
	time_t t;

	if(clock_get_time(NULL, &t) != 0)
		return -1;
	if(time != NULL)
		*time = t;
	return t;
}
