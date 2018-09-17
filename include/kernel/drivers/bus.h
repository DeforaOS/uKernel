/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_DRIVERS_BUS_H
# define UKERNEL_DRIVERS_BUS_H

# include <stdarg.h>
# include <stdint.h>


/* public */
/* types */
typedef struct _ukBus ukBus;
typedef uintptr_t ukBusAddress;
typedef struct _ukBusData ukBusData;

struct _ukBus
{
	char const name[16];

	ukBus * (*init)(ukBus * parent);
	void (*destroy)(ukBus * bus);

	int (*read8)(ukBus * bus, ukBusAddress address, uint8_t * value);
	int (*read16)(ukBus * bus, ukBusAddress address, uint16_t * value);
	int (*read32)(ukBus * bus, ukBusAddress address, uint32_t * value);

	int (*write8)(ukBus * bus, ukBusAddress address, uint8_t value);
	int (*write16)(ukBus * bus, ukBusAddress address, uint16_t value);
	int (*write32)(ukBus * bus, ukBusAddress address, uint32_t value);

	int (*command)(ukBus * bus, uint32_t command, va_list ap);

	ukBusData * data;
};


/* constants */
typedef enum _ukBusCommand
{
	BUS_COMMAND_WAIT = 0
} ukBusCommand;
# define BUS_COMMAND_LAST BUS_COMMAND_WAIT
# define BUS_COMMAND_COUNT (BUS_COMMAND_LAST + 1)


/* prototypes */
ukBus * bus_init(ukBus * parent, char const * name);

#endif /* !UKERNEL_DRIVERS_BUS_H */
