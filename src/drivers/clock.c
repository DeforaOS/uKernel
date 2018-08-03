/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
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
extern const ukClockInterface cmos_clock;
#endif


/* functions */
/* clock_init */
ukClock * clock_init(ukBus * bus, char const * name, ...)
{
	const ukClockInterface * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&cmos_clock
#endif
	};
	ukClock * clock;
	va_list ap;
	size_t i;

	if((clock = malloc(sizeof(*clock))) == NULL)
		return NULL;
	va_start(ap, name);
	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			fprintf(stderr, "%s clock%s%s%s\n", name,
					(bus != NULL) ? " at " : "",
					(bus != NULL)
					? bus->interface->name : "",
					(bus != NULL) ? " bus" : "");
			clock->driver = drivers[i]->init(bus, ap);
			break;
		}
	va_end(ap);
	if(clock->driver == NULL)
	{
		if(i == sizeof(drivers) / sizeof(*drivers))
			errno = ENODEV;
		free(clock);
		return NULL;
	}
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


/* clock_get_name */
char const * clock_get_name(ukClock * clock)
{
	return clock->interface->name;
}


/* clock_get_time */
int clock_get_time(ukClock * clock, time_t * time)
{
	return clock->interface->get_time(clock, time);
}
