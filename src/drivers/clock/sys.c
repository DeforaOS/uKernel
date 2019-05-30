/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <time.h>
#include <kernel/drivers/clock.h>


/* private */
/* types */
typedef struct _ukClock SysClock;


/* prototypes */
/* console */
static SysClock * _sys_clock_init(ukBus * bus);

static int _sys_clock_get_time(SysClock * console, time_t * ptime);


/* public */
/* variables */
/* console */
SysClock sys_clock =
{
	"sys",
	_sys_clock_init,
	NULL,
	_sys_clock_get_time,
	NULL
};


/* functions */
/* console */
/* sys_clock_init */
static SysClock * _sys_clock_init(ukBus * bus)
{
	(void) bus;

	return &sys_clock;
}


/* sys_clock_get_time */
static int _sys_clock_get_time(SysClock * clock, time_t * ptime)
{
	time_t t;
	(void) clock;

	if(time(&t) == -1)
		return -1;
	if(ptime != NULL)
		*ptime = t;
	return 0;
}
