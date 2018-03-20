/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_DRIVERS_CONSOLE_H
# define UKERNEL_DRIVERS_CONSOLE_H


/* public */
/* types */
typedef struct _Console Console;


/* prototypes */
Console * console_init(void);

void console_clear(Console * console);

#endif /* !UKERNEL_DRIVERS_CONSOLE_H */
