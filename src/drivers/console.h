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


/* prototypes */
ukConsole * console_init(ukBus * bus);

/* useful */
void console_clear(ukConsole * console);
void console_print(ukConsole * console, char const * str, size_t len);

#endif /* !UKERNEL_DRIVERS_CONSOLE_H */
