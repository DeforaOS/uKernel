/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <stdio.h>
#include "drivers/console.h"


/* public */
/* functions */
/* puts */
int puts(char const * string)
{
	size_t len;

	len = strlen(string);
	/* XXX assumes the whole string was output */
	console_print(NULL, string, len);
	return len;
}
