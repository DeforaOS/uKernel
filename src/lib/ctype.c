/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "ctype.h"
#undef isalnum
#undef isalpha
#undef ispunct


/* functions */
/* isalnum */
int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
}


/* isalpha */
int isalpha(int c)
{
	return islower(c) || isupper(c);
}


/* isascii */
int isascii(int c)
{
	return c >= 0 && c <= 127;
}


/* isblank */
int isblank(int c)
{
	return c == ' ' || c == '\t';
}


/* iscntrl */
int iscntrl(int c)
{
	/* FIXME implement */
	return 0;
}


/* isdigit */
int isdigit(int c)
{
	return c >= '0' && c <= '9';
}


/* isgraph */
int isgraph(int c)
{
	return c > ' ' && c <= '~';
}


/* islower */
int islower(int c)
{
	return c >= 'a' && c <= 'z';
}


/* isprint */
int isprint(int c)
{
	return c >= ' ' && c <= '~';
}


/* ispunct */
int ispunct(int c)
{
	return isprint(c) && (!(isspace(c) || isalnum(c)));
}


/* isspace */
int isspace(int c)
{
	return c == ' ' || (c >= '\t' && c <= '\r');
}


/* isupper */
int isupper(int c)
{
	return c >= 'A' && c <= 'Z';
}


/* isxdigit */
int isxdigit(int c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
		|| (c >= 'A' && c <= 'F');
}


/* toascii */
int toascii(int c)
{
	return c & 0x7f;
}


/* tolower */
int tolower(int c)
{
	if(isupper(c))
		return c + 'a' - 'A';
	return c;
}


/* toupper */
int toupper(int c)
{
	if(islower(c))
		return c + 'A' - 'a';
	return c;
}
