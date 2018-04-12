/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>


/* public */
/* functions */
/* memcmp */
int memcmp(void const * s1, void const * s2, size_t n)
{
	unsigned char const * u1 = s1;
	unsigned char const * u2 = s2;

	if(n == 0)
		return 0;
	while(--n && *u1 == *u2)
	{
		u1++;
		u2++;
	}
	return *u1 - *u2;
}


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


/* memset */
void * memset(void * dest, int c, size_t n)
{
	unsigned char * d = dest;

	while(n--)
		*d++ = c;
	return dest;
}


/* strcmp */
int strcmp(char const * s1, char const * s2)
{
	unsigned char const * u1;
	unsigned char const * u2;

	u1 = (unsigned char const *)s1;
	u2 = (unsigned char const *)s2;
	while(*u1 && *u2 && *u1 == *u2)
	{
		u1++;
		u2++;
	}
	return *u1 - *u2;
}


/* strcpy */
char * strcpy(char * dest, char const * src)
{
	char * ret = dest;

	while((*dest++ = *src++));
	return ret;
}


/* strlen */
size_t strlen(char const * s)
{
	size_t len = 0;

	while(*s++)
		len++;
	return len;
}
