/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_CLOCK_H
# define UKERNEL_DRIVERS_CLOCK_H

# include <time.h>
# include "bus.h"


/* public */
/* types */
typedef struct _ukClock ukClock;

typedef struct _ukClockData ukClockData;

struct _ukClock
{
	char name[16];

	ukClock * (*init)(ukBus * bus);
	void (*destroy)(ukClock * clock);

	int (*get_time)(ukClock * clock, time_t * time);

	ukClockData * data;
};


/* prototypes */
ukClock * clock_init(ukBus * bus, char const * name);
void clock_destroy(ukClock * clock);

/* accessors */
ukClock * clock_get_default(void);

int clock_get_time(ukClock * clock, time_t * time);

#endif /* !UKERNEL_DRIVERS_CLOCK_H */
