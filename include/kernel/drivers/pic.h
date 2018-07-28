/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_PIC_H
# define UKERNEL_DRIVERS_PIC_H

# include <stdint.h>
# include "bus.h"


/* public */
/* types */
typedef struct _ukPIC ukPIC;
typedef struct _ukPICDriver ukPICDriver;
typedef struct _ukPICInterface ukPICInterface;

struct _ukPIC
{
	const ukPICInterface * interface;
	ukPICDriver * driver;
};

struct _ukPICInterface
{
	char name[16];

	ukPICDriver * (*init)(ukBus * bus, va_list ap);
	void (*destroy)(ukPIC * pic);
};


/* prototypes */
ukPIC * pic_init(ukBus * bus, char const * name, ...);
void pic_destroy(ukPIC * pic);

/* accessors */
ukPIC * pic_get_default(void);

#endif /* !UKERNEL_DRIVERS_PIC_H */
