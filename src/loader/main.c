/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int i;

	puts("Booting DeforaOS...");
	for(i = 0; i < argc; i++)
		printf("%s%s%s", (i > 0) ? " " : "", argv[i],
				(i + 1 == argc) ? "\n" : "");
	return 0;
}
