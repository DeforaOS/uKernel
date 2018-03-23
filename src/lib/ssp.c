/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include "../drivers/console.h"


#if defined(__SSP__)
/* variables */
/* default to the "terminator canary" */
long __stack_chk_guard[8] = { 0, 0, '\n', 255, 0, 0, 0, 0 };


/* functions */
/* stack_chk_fail */
void __stack_chk_fail(void)
{
	const char buf[] = "stack overflow detected: terminated\n";

	/* output on the standard error stream */
	console_print(NULL, buf, sizeof(buf) - 1);
	/* trigger a crash */
	abort();
}


/* stack_chk_setup */
void __stack_chk_setup(void)
{
	size_t i;

	if(__stack_chk_guard[0] == 0)
		for(i = 0; i < sizeof(__stack_chk_guard)
				/ sizeof(*__stack_chk_guard); i++)
			__stack_chk_guard[i] = arc4random();
}
#endif
