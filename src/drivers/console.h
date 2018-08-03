/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_CONSOLE_H
# define UKERNEL_DRIVERS_CONSOLE_H

# include <stdint.h>
# include "bus.h"


/* public */
/* types */
typedef struct _ukConsole ukConsole;
typedef struct _ukConsoleDriver ukConsoleDriver;
typedef struct _ukConsoleInterface ukConsoleInterface;

struct _ukConsole
{
	const ukConsoleInterface * interface;
	ukConsoleDriver * driver;
};

struct _ukConsoleInterface
{
	char name[16];

	ukConsoleDriver * (*init)(ukBus * bus, va_list ap);
	void (*destroy)(ukConsole * console);

	void (*clear)(ukConsole * console);

	void (*print)(ukConsole * console, char const * str, size_t len);
};


/* prototypes */
ukConsole * console_init(ukBus * bus, char const * name, ...);
void console_destroy(ukConsole * console);

/* accessors */
ukConsole * console_get_default(void);
char const * console_get_name(ukConsole * console);

/* helpers */
void console_clear(ukConsole * console);

void console_print(ukConsole * console, char const * str, size_t len);

#endif /* !UKERNEL_DRIVERS_CONSOLE_H */
