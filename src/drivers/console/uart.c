/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include "drivers/console.h"
#include "uart.h"


/* private */
/* types */
typedef struct _ukConsole UARTConsole;

typedef struct _ukConsoleData
{
	uint16_t port;

	ukBus * bus;
} UARTConsoleData;


/* prototypes */
/* console */
static UARTConsole * _uart_console_init(ukBus * bus);

static void _uart_console_print(UARTConsole * console, char const * str,
		size_t len);


/* variables */
static ukConsoleData _uart_console_data =
{
	UART_ADDRESS_BASE,
	NULL
};


/* public */
/* variables */
/* console */
UARTConsole uart_console =
{
	_uart_console_init,
	NULL,
	NULL,
	_uart_console_print,
	&_uart_console_data
};


/* functions */
/* console */
/* uart_console_init */
static ukConsole * _uart_console_init(ukBus * bus)
{
	UARTConsoleData * data = &_uart_console_data;

	data->bus = bus;
	bus->write8(bus, (ukBusAddress)(data->port
				+ UART_REGISTER_INTERRUPT_ENABLE), 0x00);
	bus->write8(bus, (ukBusAddress)(data->port
				+ UART_REGISTER_LINE_CONTROL), 0x80);
	bus->write8(bus, (ukBusAddress)(data->port/* + 0x0 */), 0x01);
	bus->write8(bus, (ukBusAddress)(data->port + 0x1), 0x00);
	bus->write8(bus, (ukBusAddress)(data->port
			+ UART_REGISTER_LINE_CONTROL), 0x03);
	bus->write8(bus, (ukBusAddress)(data->port
			+ UART_REGISTER_FIFO_CONTROL), 0xc7);
	bus->write8(bus, (ukBusAddress)(data->port
			+ UART_REGISTER_MODEM_CONTROL), 0x0b);
	return &uart_console;
}


/* uart_console_print */
static void _uart_console_print(UARTConsole * console, char const * str,
		size_t len)
{
	UARTConsoleData * data = console->data;
	size_t i;
	uint8_t u8;

	for(i = 0; i < len; i++)
	{
		while(data->bus->read8(data->bus, (ukBusAddress)(data->port
						+ UART_REGISTER_LINE_STATUS),
					&u8) == 0)
			if((u8 & 0x20) == 0)
				break;
		data->bus->write8(data->bus, (ukBusAddress)(data->port
					+ UART_REGISTER_DATA), (uint8_t)str[i]);
	}
}
