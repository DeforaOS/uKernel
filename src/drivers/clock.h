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
typedef struct _ukClockDriver ukClockDriver;
typedef struct _ukClockInterface ukClockInterface;

struct _ukClock
{
	const ukClockInterface * interface;
	ukClockDriver * driver;
};

typedef struct _ukClockInterface
{
	char name[16];

	ukClockDriver * (*init)(ukBus * bus, va_list ap);
	void (*destroy)(ukClock * clock);

	int (*get_time)(ukClock * clock, time_t * time);
} ukClockInterface;


/* prototypes */
ukClock * clock_init(ukBus * bus, char const * name, ...);
void clock_destroy(ukClock * clock);

/* accessors */
ukClock * clock_get_default(void);
char const * clock_get_name(ukClock * clock);

int clock_get_time(ukClock * clock, time_t * time);

#endif /* !UKERNEL_DRIVERS_CLOCK_H */
