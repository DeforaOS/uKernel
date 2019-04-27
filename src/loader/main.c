/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int i;

	puts("Failed to boot DeforaOS");
	printf("Command line:");
	for(i = 0; i < argc; i++)
		printf(" \"%s\"", argv[i]);
	printf("\n");
	return 0;
}
