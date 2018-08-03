/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "display.h"


/* private */
/* variables */
static ukDisplay * _display = NULL;


/* public */
/* variables */
#if defined(__user__)
#elif defined(__amd64__) || defined(__i386__)
extern const ukDisplayInterface vesa_display;
extern const ukDisplayInterface vga_display;
#endif


/* functions */
/* display_init */
ukDisplay * display_init(ukBus * bus, char const * name, ...)
{
	const ukDisplayInterface * drivers[] = {
#if defined(__user__)
#elif defined(__amd64__) || defined(__i386__)
		&vesa_display,
		&vga_display
#endif
	};
	va_list ap;
	size_t i;
	ukDisplay * display;

	if((display = malloc(sizeof(*display))) == NULL)
		return NULL;
	va_start(ap, name);
	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			display->interface = drivers[i];
			display->driver = drivers[i]->init(bus, ap);
			break;
		}
	va_end(ap);
	if(display->driver == NULL)
	{
		if(i == sizeof(drivers) / sizeof(*drivers))
			errno = ENODEV;
		free(display);
		return NULL;
	}
	if(_display == NULL)
		_display = display;
	return display;
}


/* display_destroy */
void display_destroy(ukDisplay * display)
{
	if(display == NULL
			&& (display = display_get_default()) == NULL)
		return;
	if(display->interface->destroy != NULL)
		display->interface->destroy(display);
}


/* accessors */
/* display_get_default */
ukDisplay * display_get_default(void)
{
	if(_display == NULL)
		errno = ENODEV;
	return _display;
}


/* display_get_name */
char const * display_get_name(ukDisplay * display)
{
	return display->interface->name;
}


/* display_set_mode */
int display_set_mode(ukDisplay * display, ukDisplayMode mode,
		unsigned int width, unsigned int height, unsigned int depth)
{
	if(display->interface->set_mode == NULL)
	{
		errno = ENOTSUP;
		return -1;
	}
	return display->interface->set_mode(display, mode, width, height,
			depth);
}


/* helpers */
/* display_clear */
void display_clear(ukDisplay * display)
{
	if(display->interface->clear != NULL)
		display->interface->clear(display);
}


/* display_print */
void display_print(ukDisplay * display, char const * str, size_t len)
{
	if(display->interface->print != NULL)
		display->interface->print(display, str, len);
}
