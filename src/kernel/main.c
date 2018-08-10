/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#include <time.h>
#include <errno.h>


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int i;
	time_t t;

	puts("Starting DeforaOS...");
	puts("Command line:");
	for(i = 0; i < argc; i++)
		printf("%s%s%s", (i > 0) ? " " : "", argv[i],
				(i + 1 == argc) ? "\n" : "");
	if((t = time(NULL)) == -1)
		printf("Could not get the current time (%d)\n", errno);
	else
		printf("Time: %lu\n", t);
	return 0;
}
