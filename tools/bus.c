/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/bus.h"


/* types */
struct _Bus
{
};


/* variables */
static Bus _bus;


/* functions */
/* bus_init */
Bus * bus_init(void)
{
	return &_bus;
}
