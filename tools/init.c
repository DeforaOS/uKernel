/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stddef.h>
#include <kernel/platform.h>


/* public */
/* functions */
/* init */
int _init(void)
{
	boot_init(boot, "user", NULL, 0, 0);
	platform_init(platform, boot);
	platform_start(platform);
	return 0;
}
