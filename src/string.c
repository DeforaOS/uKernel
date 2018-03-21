/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>


/* public */
/* functions */
/* memcpy */
void * memcpy(void * dest, void const * src, size_t n)
{
	char * d = dest;
	char const * s = src;
	
	while(n--)
		*d++ = *s++;
	return dest;
}


/* memmove */
void * memmove(void * dest, void const * src, size_t n)
{
	char * d = dest;
	char const * s = src;

	if(n == 0 || dest == src)
		return dest;
	if(d + n < s || d > s + n)
		return memcpy(dest, src, n);
	if(s < d)
	{
		d += n - 1;
		s += n - 1;
		while(n--)
			*d-- = *s--;
		return dest;
	}
	return memcpy(dest, src, n);
}
