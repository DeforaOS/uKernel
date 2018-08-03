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
# define UKDT_LAST	UKDT_PIC
# define UKDT_COUNT	(UKDT_LAST + 1)

#endif /* !UKERNEL_DRIVER_H */
