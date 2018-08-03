/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVER_H
# define UKERNEL_DRIVER_H


/* types */
typedef struct _ukBus ukBus;
typedef struct _ukDriver ukDriver;

typedef enum _ukDriverType
{
	UKDT_UNKNOWN = 0,
	UKDT_BUS,
	UKDT_CLOCK,
	UKDT_CONSOLE,
	UKDT_DISPLAY,
	UKDT_PIC
} ukDriverType;

#endif /* !UKERNEL_DRIVER_H */
