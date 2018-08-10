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
static time_t _get_time_days_per_month(unsigned char month, unsigned int year);
static int _get_time_do(ukBus * bus, unsigned char * day, unsigned char * month,
		unsigned char * year, unsigned char * hours,
		unsigned char * minutes, unsigned char * seconds);
static void _time_do_decode(unsigned char * value);

static int _cmos_clock_get_time(CMOSClock * clock, time_t * time)
{
	const size_t tries = 3;
	const time_t seconds_per_day = 60 * 60 * 24;
	CMOSClockData * data = clock->data;
	unsigned char seconds, s;
	unsigned char minutes, m;
	unsigned char hours, h;
	unsigned char day, d;
	unsigned char month, M;
	unsigned char year, y;
	size_t i;
	size_t j;

	if(time == NULL)
	{
		errno = EINVAL;
		return -1;
	}
	for(i = 0; i < tries; i++)
		/* try to obtain the same results twice for consistency */
		if(_get_time_do(data->bus, &day, &month, &year,
					&hours, &minutes, &seconds) != 0
				|| _get_time_do(data->bus, &d, &M, &y,
					&h, &m, &s) != 0)
			return -1;
		else if(seconds == s && minutes == m && hours == h
				&& day == d && month == M && year == y)
			break;
	if(i == tries)
	{
		errno = EAGAIN;
		return -1;
	}
	/* FIXME this is not optimal nor fully accurate */
	*time = 0;
	for(i = 0; i < ((year >= 70) ? year - 70 : year + 30); i++)
		for(j = 1; j <= 12; j++)
			*time += _get_time_days_per_month(j, i + 1970)
				* seconds_per_day;
	for(j = 1; j < month; j++)
		*time += _get_time_days_per_month(j, i + 1970)
			* seconds_per_day;
	*time += (day - 1) * seconds_per_day + hours * 60 * 60 + minutes * 60
		+ seconds;
	return 0;
}

static time_t _get_time_days_per_month(unsigned char month, unsigned int year)
{
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 2:
			if((year & 0x3) == 0 && year != 2000)
				return 29;
			return 28;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		default:
			return 0;
	}
}

static int _get_time_do(ukBus * bus, unsigned char * day, unsigned char * month,
		unsigned char * year, unsigned char * hours,
		unsigned char * minutes, unsigned char * seconds)
{
	const size_t tries = 3;
	size_t i;
	unsigned char status;

	/* check if the time and date are available */
	for(i = 0; i < tries; i++)
		if(bus->read8(bus, CMOS_REGISTER_STATUS0, &status) != 0)
			return -1;
		else if((status & CMOS_STATUS0_UPDATING) == 0x00)
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
	if((status & CMOS_STATUS1_24HOUR) && (*hours & CMOS_HOUR_PM))
		*hours = ((*hours & ~CMOS_HOUR_PM) + 12) % 24;
	/* convert to decimal if necessary */
	if((status & CMOS_STATUS1_BINARY_MODE) != CMOS_STATUS1_BINARY_MODE)
	{
		_time_do_decode(seconds);
		_time_do_decode(minutes);
		_time_do_decode(hours);
		_time_do_decode(day);
		_time_do_decode(month);
		_time_do_decode(year);
	}
	return 0;
}

static void _time_do_decode(unsigned char * value)
{
	*value = (*value & 0xf) + (*value >> 4) * 10;
}
