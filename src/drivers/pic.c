/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "pic.h"


/* private */
/* variables */
static ukPIC * _pic = NULL;


/* public */
/* variables */
#if defined(__user__)
#elif defined(__amd64__) || defined(__i386__)
extern const ukPICInterface i8259a_pic;
#endif


/* functions */
/* pic_init */
ukPIC * pic_init(ukBus * bus, char const * name, ...)
{
	const ukPICInterface * drivers[] = {
#if defined(__user__)
#elif defined(__amd64__) || defined(__i386__)
		&i8259a_pic
#endif
	};
	va_list ap;
	size_t i;
	ukPIC * pic;

	if((pic = malloc(sizeof(*pic))) == NULL)
		return NULL;
	va_start(ap, name);
	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			pic->interface = drivers[i];
			pic->driver = drivers[i]->init(bus, ap);
			break;
		}
	va_end(ap);
	if(pic->driver == NULL)
	{
		if(i == sizeof(drivers) / sizeof(*drivers))
			errno = ENODEV;
		free(pic);
		return NULL;
	}
	if(_pic == NULL)
		_pic = pic;
	return pic;
}


/* accessors */
/* pic_get_default */
ukPIC * pic_get_default(void)
{
	if(_pic == NULL)
		errno = ENODEV;
	return _pic;
}


/* pic_get_name */
char const * pic_get_name(ukPIC * pic)
{
	return pic->interface->name;
}
