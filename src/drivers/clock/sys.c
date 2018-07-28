/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <kernel/drivers/clock.h>


/* private */
/* types */
typedef struct _ukClock SysClock;

typedef struct _ukClockDriver SysClockDriver;


/* prototypes */
/* console */
static SysClockDriver * _sys_clock_init(ukBus * bus, va_list ap);

static int _sys_clock_get_time(SysClock * console, time_t * time);

extern time_t _time(time_t * t);


/* public */
/* variables */
/* console */
const ukClockInterface sys_clock =
{
	"sys",
	_sys_clock_init,
	NULL,
	_sys_clock_get_time
};


/* functions */
/* console */
/* sys_clock_init */
static SysClockDriver * _sys_clock_init(ukBus * bus, va_list ap)
{
	(void) bus;
	(void) ap;

	return (SysClockDriver *)&sys_clock;
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
