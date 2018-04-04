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
typedef struct _ukConsoleData ukConsoleData;

struct _ukConsole
{
	ukConsole * (*init)(ukBus * bus);

	void (*clear)(ukConsole * console);

	void (*print)(ukConsole * console, char const * str, size_t len);

	ukConsoleData * data;
};


/* prototypes */
ukConsole * console_init(ukBus * bus);

/* accessors */
ukConsole * console_get_default(void);

/* helpers */
void console_clear(ukConsole * console);

void console_print(ukConsole * console, char const * str, size_t len);

#endif /* !UKERNEL_DRIVERS_CONSOLE_H */
