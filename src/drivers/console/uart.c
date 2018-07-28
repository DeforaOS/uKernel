/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <errno.h>
#include "drivers/console.h"
#include "uart.h"


/* private */
/* types */
typedef struct _ukConsole UARTConsole;

typedef struct _ukConsoleDriver
{
	ukBus * bus;
	uint16_t port;
} UARTConsoleDriver;


/* prototypes */
/* console */
static UARTConsoleDriver * _uart_console_init(ukBus * bus, va_list ap);

static void _uart_console_print(UARTConsole * console, char const * str,
		size_t len);


/* variables */
static ukConsoleDriver _uart_console_driver =
{
	NULL,
	UART_ADDRESS_BASE_COM0
};


/* public */
/* variables */
/* console */
const ukConsoleInterface uart_console =
{
	"uart",
	_uart_console_init,
	NULL,
	NULL,
	_uart_console_print
};


/* functions */
/* console */
/* uart_console_init */
static UARTConsoleDriver * _uart_console_init(ukBus * bus, va_list ap)
{
	UARTConsoleDriver * uart = &_uart_console_driver;
	(void) ap;

	uart->bus = bus;
	bus->interface->write8(bus, (ukBusAddress)(uart->port
				+ UART_REGISTER_INTERRUPT_ENABLE), 0x00);
	bus->interface->write8(bus, (ukBusAddress)(uart->port
				+ UART_REGISTER_LINE_CONTROL), 0x80);

	/* set baud rate to 115200 bps */
	bus->interface->write8(bus, (ukBusAddress)(uart->port + 0x0), 0x01);
	bus->interface->write8(bus, (ukBusAddress)(uart->port + 0x1), 0x00);
	bus->interface->write8(bus, (ukBusAddress)(uart->port
			+ UART_REGISTER_LINE_CONTROL), 0x03);
	bus->interface->write8(bus, (ukBusAddress)(uart->port
			+ UART_REGISTER_FIFO_CONTROL), 0xc7);
	bus->interface->write8(bus, (ukBusAddress)(uart->port
			+ UART_REGISTER_MODEM_CONTROL), 0x0b);
	return uart;
}


/* uart_console_print */
static void _uart_console_print(UARTConsole * console, char const * str,
		size_t len)
{
	UARTConsoleDriver * uart = console->driver;
	ukBus * bus = uart->bus;
	size_t i;
	uint8_t u8;

	for(i = 0; i < len; i++)
	{
		while(bus->interface->read8(bus, (ukBusAddress)(uart->port
						+ UART_REGISTER_LINE_STATUS),
					&u8) == 0)
			if((u8 & 0x20) != 0)
				break;
		bus->interface->write8(bus, (ukBusAddress)(uart->port
					+ UART_REGISTER_DATA), (uint8_t)str[i]);
	}
}
