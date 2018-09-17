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

typedef struct _ukPICData ukPICData;

struct _ukPIC
{
	char name[16];

	ukPIC * (*init)(ukBus * bus);
	void (*destroy)(ukPIC * pic);

	ukPICData * data;
};


/* prototypes */
ukPIC * pic_init(ukBus * bus, char const * name);
void pic_destroy(ukPIC * pic);

/* accessors */
ukPIC * pic_get_default(void);

#endif /* !UKERNEL_DRIVERS_PIC_H */
