/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_DRIVERS_BUS_H
# define UKERNEL_DRIVERS_BUS_H

# include <stdint.h>


/* public */
/* types */
typedef struct _Bus Bus;


/* prototypes */
Bus * bus_init(void);

/* useful */
int bus_write8(Bus * bus, void * addr, uint8_t value);
int bus_write16(Bus * bus, void * addr, uint16_t value);
int bus_write32(Bus * bus, void * addr, uint32_t value);

#endif /* !UKERNEL_DRIVERS_BUS_H */
