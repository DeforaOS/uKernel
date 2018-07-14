/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <errno.h>
#include "drivers/clock.h"
#include "drivers/clock/cmos.h"


/* private */
/* types */
typedef struct _ukClock CMOSClock;

typedef struct _ukClockData
{
	ukBus * bus;
} CMOSClockData;


/* prototypes */
/* clock */
static CMOSClock * _cmos_clock_init(ukBus * bus);
static void _cmos_clock_destroy(CMOSClock * clock);

static int _cmos_clock_get_time(CMOSClock * clock, time_t * time);


/* public */
/* variables */
/* clock */
CMOSClock cmos_clock =
{
	"cmos",
	_cmos_clock_init,
	_cmos_clock_destroy,
	_cmos_clock_get_time,
	NULL
};


/* functions */
/* clock */
/* cmos_clock_init */
static CMOSClock * _cmos_clock_init(ukBus * bus)
{
	CMOSClock * clock = &cmos_clock;
	CMOSClockData * data;

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((data = malloc(sizeof(*data))) == NULL)
		return NULL;
	data->bus = bus;
	clock->data = data;
	return clock;
}


/* cmos_clock_destroy */
static void _cmos_clock_destroy(CMOSClock * clock)
{
	free(clock->data);
}


/* cmos_clock_get_time */
static int _get_time_do(ukBus * bus, unsigned char * seconds,
		unsigned char * minutes, unsigned char * hours,
		unsigned char * day, unsigned char * month,
		unsigned char * year);
static int _cmos_clock_get_time(CMOSClock * clock, time_t * time)
{
	CMOSClockData * data = clock->data;
	unsigned char seconds, s;
	unsigned char minutes, m;
	unsigned char hours, h;
	unsigned char day, d;
	unsigned char month, M;
	unsigned char year, y;
	size_t i;

	if(time == NULL)
	{
		errno = EINVAL;
		return -1;
	}
	for(i = 0; i < 3; i++)
	{
		if(_get_time_do(data->bus, &seconds, &minutes, &hours, &day,
					&month, &year) != 0
				|| _get_time_do(data->bus, &s, &m, &h, &d, &M,
					&y) != 0)
			return -1;
		if(seconds == s && minutes == m && hours == h
				&& day == d && month == M && year == y)
			break;
	}
	if(i == 3)
	{
		errno = EAGAIN;
		return -1;
	}
	/* FIXME this is not correct */
	*time = seconds + (minutes * 60) + (hours * 60 * 24)
		+ (day * 60 * 24 * 30) + (month * 60 * 24 * 12)
		+ (((year >= 70) ? (year - 1970) : (year + 30))
				* 60 * 24 * 365);
	return 0;
}

static int _get_time_do(ukBus * bus, unsigned char * seconds,
		unsigned char * minutes, unsigned char * hours,
		unsigned char * day, unsigned char * month,
		unsigned char * year)
{
	/* FIXME check first if the data is available */
	if(bus->read8(bus, CMOS_REGISTER_SECONDS, seconds) != 0
			|| bus->read8(bus, CMOS_REGISTER_MINUTES, minutes) != 0
			|| bus->read8(bus, CMOS_REGISTER_HOURS, hours) != 0
			|| bus->read8(bus, CMOS_REGISTER_DAY, day) != 0
			|| bus->read8(bus, CMOS_REGISTER_MONTH, month) != 0
			|| bus->read8(bus, CMOS_REGISTER_YEAR, year) != 0)
		return -1;
	return 0;
}
