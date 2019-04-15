/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
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
	printf("Command line:");
	for(i = 0; i < argc; i++)
		printf(" %s%s", argv[i], (i + 1 == argc) ? "\n" : "");
	if(argv[i] != NULL)
		puts("argv is not terminated properly!");
	if((t = time(NULL)) == -1)
		printf("Could not get the current time (%d)\n", errno);
	else
		printf("Time: %lu\n", t);
	return 0;
}
