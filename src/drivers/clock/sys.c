/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "drivers/clock.h"


/* private */
/* types */
typedef struct _ukClock SysClock;

typedef struct _ukClockDriver
{
	ukBus * bus;
} SysClockDriver;


/* prototypes */
/* console */
static SysClockDriver * _sys_clock_init(ukBus * bus, va_list ap);
static void _sys_clock_destroy(SysClock * clock);

static int _sys_clock_get_time(SysClock * console, time_t * time);

extern time_t _time(time_t * t);


/* public */
/* variables */
/* console */
const ukClockInterface sys_clock =
{
	"sys",
	_sys_clock_init,
	_sys_clock_destroy,
	_sys_clock_get_time
};


/* functions */
/* console */
/* sys_clock_init */
static SysClockDriver * _sys_clock_init(ukBus * bus, va_list ap)
{
	SysClockDriver * clock;
	(void) ap;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((clock = malloc(sizeof(*clock))) == NULL)
		return NULL;
	clock->bus = bus;
	return clock;
}


/* sys_clock_destroy */
static void _sys_clock_destroy(SysClock * clock)
{
	free(clock->driver);
}


/* sys_clock_get_time */
static int _sys_clock_get_time(SysClock * clock, time_t * time)
{
	time_t t;
	(void) clock;

	if(_time(&t) == -1)
		return -1;
	if(time != NULL)
		*time = t;
	return 0;
}
