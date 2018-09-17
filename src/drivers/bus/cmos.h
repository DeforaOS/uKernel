/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_BUS_CMOS_H
# define UKERNEL_DRIVERS_BUS_CMOS_H

# include <kernel/drivers/bus.h>


/* types */
typedef enum _CMOSBusCommand
{
	BUS_CMOS_COMMAND_NMI_ENABLE = BUS_COMMAND_COUNT,
	BUS_CMOS_COMMAND_NMI_DISABLE
} CMOSBusCommand;


/* constants */
#define BUS_CMOS_REGISTER_ADDRESS	0x70
#define BUS_CMOS_REGISTER_DATA		0x71

#define BUS_CMOS_NMI_ENABLE		0x00
#define BUS_CMOS_NMI_DISABLE		0x80

#endif /* !UKERNEL_DRIVERS_BUS_CMOS_H */
