/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <kernel/drivers/clock.h>


/* private */
/* variables */
static ukClock * _clock = NULL;


/* public */
/* variables */
extern ukClock sys_clock;


/* functions */
/* clock_init */
ukClock * clock_init(ukBus * bus, char const * name)
{
	const ukClock * drivers[] = {
		&sys_clock
	};
	size_t i;
	ukClock * clock = NULL;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			printf("%s%s%s\n", name, (bus != NULL) ? " at " : "",
					(bus != NULL) ? bus->name : "");
			clock = drivers[i]->init(bus);
		}
	if(clock == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	else if(_clock == NULL)
		_clock = clock;
	return clock;
}


/* FIXME code duplication with src/drivers/clock.c */
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
	if(clock->get_time == NULL)
	{
		errno = ENOTSUP;
		return -1;
	}
	return clock->get_time(clock, time);
}
