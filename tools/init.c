/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <kernel/platform.h>


/* init */
int init(void)
{
	platform_init();
	return 0;
}
