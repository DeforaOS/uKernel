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

	if(bus == NULL)
	{
		errno = ENODEV;
		return NULL;
	}
	if((clock->data = malloc(sizeof(*clock->data))) == NULL)
		return NULL;
	clock->data->bus = bus;
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
	const size_t tries = 3;
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
	for(i = 0; i < tries; i++)
		/* try to obtain the same results twice for consistency */
		if(_get_time_do(data->bus, &seconds, &minutes, &hours, &day,
					&month, &year) != 0
				|| _get_time_do(data->bus, &s, &m, &h, &d, &M,
					&y) != 0)
			return -1;
		else if(seconds == s && minutes == m && hours == h
				&& day == d && month == M && year == y)
			break;
	if(i == tries)
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
	const size_t tries = 3;
	size_t i;
	unsigned char status;

	/* check if the time and date are available */
	for(i = 0; i < tries; i++)
		if(bus->read8(bus, CMOS_REGISTER_STATUS0, &status) != 0)
			return -1;
		else if((status & 0x80) == 0x00)
			break;
	if(i == tries)
		return -1;
	if(bus->read8(bus, CMOS_REGISTER_STATUS1, &status) != 0
			|| bus->read8(bus, CMOS_REGISTER_SECONDS, seconds) != 0
			|| bus->read8(bus, CMOS_REGISTER_MINUTES, minutes) != 0
			|| bus->read8(bus, CMOS_REGISTER_HOURS, hours) != 0
			|| bus->read8(bus, CMOS_REGISTER_DAY, day) != 0
			|| bus->read8(bus, CMOS_REGISTER_MONTH, month) != 0
			|| bus->read8(bus, CMOS_REGISTER_YEAR, year) != 0)
		return -1;
	/* convert to a 24-hour clock if necessary */
	if((status & 0x02) != 0x00 && (*hours & 0x80))
		*hours = ((*hours & 0x7f) + 12) % 24;
	return 0;
}
