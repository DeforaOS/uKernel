/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <kernel/drivers/display.h>


/* private */
/* variables */
static ukDisplay * _display = NULL;


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern ukDisplay vesa_display;
extern ukDisplay vga_display;
#endif


/* functions */
/* display_init */
ukDisplay * display_init(ukBus * bus, char const * name)
{
	const ukDisplay * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&vesa_display,
		&vga_display
#endif
	};
	size_t i;
	ukDisplay * display = NULL;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			syslog(LOG_KERN | LOG_NOTICE, "%s display%s%s%s", name,
					(bus != NULL) ? " at " : "",
					(bus != NULL) ? bus->name : "",
					(bus != NULL) ? " bus" : "");
			if((_display = drivers[i]->init(bus)) == NULL)
				return NULL;
			break;
		}
	if(_display == NULL)
		errno = ENODEV;
	else if(_display == NULL)
		_display = display;
	return _display;
}


/* display_destroy */
void display_destroy(ukDisplay * display)
{
	if(display == NULL
			&& (display = display_get_default()) == NULL)
		return;
	if(display->destroy != NULL)
		display->destroy(display);
}


/* accessors */
/* display_get_default */
ukDisplay * display_get_default(void)
{
	if(_display == NULL)
		errno = ENODEV;
	return _display;
}


/* display_set_mode */
int display_set_mode(ukDisplay * display, ukDisplayMode mode,
		unsigned int width, unsigned int height, unsigned int depth)
{
	if(display == NULL
			&& (display = display_get_default()) == NULL)
		return -1;
	if(display->set_mode == NULL)
	{
		errno = ENOTSUP;
		return -1;
	}
	return display->set_mode(display, mode, width, height, depth);
}


/* helpers */
/* display_clear */
void display_clear(ukDisplay * display)
{
	if(display == NULL
			&& (display = display_get_default()) == NULL)
		return;
	display->clear(display);
}


/* display_print */
void display_print(ukDisplay * display, char const * str, size_t len)
{
	if(display == NULL
			&& (display = display_get_default()) == NULL)
		return;
	display->print(display, str, len);
}
