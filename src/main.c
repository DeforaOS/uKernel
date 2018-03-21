/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "drivers/console.h"


/* public */
/* functions */
/* main */
int main(void)
{
	Console * console;
	const char msg[] = "Starting DeforaOS...\n";

	console = console_init();
	console_print(console, msg, sizeof(msg) - 1);
	return 0;
}
