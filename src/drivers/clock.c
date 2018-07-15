/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "clock.h"


/* private */
/* variables */
static ukClock * _clock = NULL;


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern ukClock cmos_clock;
#endif


/* functions */
/* clock_init */
ukClock * clock_init(ukBus * bus, char const * name)
{
	const ukClock * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&cmos_clock
#endif
	};
	size_t i;
	ukClock * clock = NULL;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			fprintf(stderr, "%s clock%s%s%s\n", name,
					(bus != NULL) ? " at " : "",
					(bus != NULL) ? bus->name : "",
					(bus != NULL) ? " bus" : "");
			clock = drivers[i]->init(bus);
		}
	if(clock == NULL)
		errno = ENODEV;
	else if(_clock == NULL)
		_clock = clock;
	return clock;
}


/* accessors */
/* clock_get_default */
ukClock * clock_get_default(void)
{
	if(_clock == NULL)
		errno = ENODEV;
	return _clock;
}


/* clock_get_time */
int clock_get_time(ukClock * clock, time_t * time)
{
	if(clock == NULL
			&& (clock = clock_get_default()) == NULL)
		return -1;
	return clock->get_time(clock, time);
}
