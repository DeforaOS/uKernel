/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_DRIVERS_BUS_H
# define UKERNEL_DRIVERS_BUS_H

# include <stdint.h>


/* public */
/* types */
typedef struct _Bus Bus;
typedef void * BusAddress;
typedef struct _BusData BusData;

struct _Bus
{
	int (*write8)(Bus * bus, BusAddress * address, uint8_t value);
	int (*write16)(Bus * bus, BusAddress * address, uint16_t value);
	int (*write32)(Bus * bus, BusAddress * address, uint32_t value);

	BusData * data;
};


/* prototypes */
Bus * bus_init(void);

#endif /* !UKERNEL_DRIVERS_BUS_H */
