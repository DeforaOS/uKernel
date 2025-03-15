/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <kernel/drivers/clock.h>
#include "cmos.h"


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
static time_t _get_time_seconds_per_month(uint8_t month, unsigned int year);
static time_t _get_time_seconds_per_year(unsigned int year);
static int _get_time_do(ukBus * bus, uint8_t * day, uint8_t * month,
		uint8_t * year, uint8_t * hours, uint8_t * minutes,
		uint8_t * seconds);
static void _time_do_decode(uint8_t * value);

static int _cmos_clock_get_time(CMOSClock * clock, time_t * time)
{
	const uint8_t tries = 3;
	const time_t seconds_per_day = 60 * 60 * 24;
	CMOSClockData * data = clock->data;
	uint8_t seconds, s;
	uint8_t minutes, m;
	uint8_t hours, h;
	uint8_t day, d;
	uint8_t month, M;
	uint8_t year, y;
	uint8_t i;
	uint8_t j;

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
		*time += _get_time_seconds_per_year(i + 1970);
	for(j = 1; j < month; j++)
		*time += _get_time_seconds_per_month(j, i + 1970);
	*time += (day - 1) * seconds_per_day + hours * 60 * 60 + minutes * 60
		+ seconds;
	return 0;
}

static time_t _get_time_seconds_per_month(uint8_t month, unsigned int year)
{
	const time_t seconds_per_day = 60 * 60 * 24;
	const time_t month28 = 28 * seconds_per_day;
	const time_t month29 = 29 * seconds_per_day;
	const time_t month30 = 30 * seconds_per_day;
	const time_t month31 = 31 * seconds_per_day;

	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return month31;
		case 2:
			if((year & 0x3) == 0 && (year % 400) != 0)
				return month29;
			return month28;
		case 4:
		case 6:
		case 9:
		case 11:
			return month30;
		default:
			return 0;
	}
}

static time_t _get_time_seconds_per_year(unsigned int year)
{
	const time_t seconds_per_day = 60 * 60 * 24;
	const time_t year365 = 365 * seconds_per_day;
	const time_t year366 = 366 * seconds_per_day;

	if((year & 0x3) == 0 && (year % 400) != 0)
		return year366;
	return year365;
}

static int _get_time_do(ukBus * bus, uint8_t * day, uint8_t * month,
		uint8_t * year, uint8_t * hours, uint8_t * minutes,
		uint8_t * seconds)
{
	const size_t tries = 3;
	size_t i;
	uint8_t status;

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

static void _time_do_decode(uint8_t * value)
{
	*value = (*value & 0xf) + (*value >> 4) * 10;
}
