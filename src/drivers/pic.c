/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <kernel/drivers/pic.h>


/* private */
/* variables */
static ukPIC * _pic = NULL;


/* public */
/* variables */
#if defined(__amd64__) || defined(__i386__)
extern ukPIC i8259a_pic;
#endif


/* functions */
/* pic_init */
ukPIC * pic_init(ukBus * bus, char const * name)
{
	const ukPIC * drivers[] = {
#if defined(__amd64__) || defined(__i386__)
		&i8259a_pic
#endif
	};
	size_t i;
	ukPIC * pic = NULL;

	for(i = 0; i < sizeof(drivers) / sizeof(*drivers); i++)
		if(strncmp(drivers[i]->name, name,
					strlen(drivers[i]->name)) == 0
				&& drivers[i]->init != NULL)
		{
			syslog(LOG_KERN | LOG_NOTICE, "%s pic%s%s%s", name,
					(bus != NULL) ? " at " : "",
					(bus != NULL) ? bus->name : "",
					(bus != NULL) ? " bus" : "");
			if((pic = drivers[i]->init(bus)) == NULL)
				return NULL;
			break;
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
