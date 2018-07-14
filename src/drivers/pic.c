/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "pic.h"


/* private */
/* variables */
static ukPIC * _pic = NULL;


/* functions */
/* pic_init */
ukPIC * pic_init(ukBus * bus, char const * name)
{
	const ukPIC * drivers[] = {
	};
	size_t i;
	ukPIC * pic = NULL;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			fprintf(stderr, "%s PIC%s%s\n", name, (bus != NULL)
					? " at " : "",
					(bus != NULL) ? bus->name : "");
			pic = drivers[i]->init(bus);
		}
	if(pic == NULL)
		errno = ENODEV;
	else if(_pic == NULL)
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
