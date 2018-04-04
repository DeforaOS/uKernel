/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include "bus.h"


#if defined(__amd64__) || defined(__i386__)
# include "bus/ioport.c"

/* bus_init */
ukBus * bus_init(char const * name)
{
	if(strcmp(name, "ioport") == 0)
		return _ioport_bus_init();
	errno = ENODEV;
	return NULL;
}
#endif
