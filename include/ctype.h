/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_CTYPE_H
# define UKERNEL_CTYPE_H


/* function prototypes */
int isalnum(int c);
# define isalnum(c) (isalpha(c) || isdigit(c))
int isalpha(int c);
# define isalpha(c) (islower(c) || isupper(c))
int isascii(int c);
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
# define ispunct(c) (isprint(c) && (!(isspace(c) || isalnum(c))))
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int toascii(int c);
int tolower(int c);
# define _tolower(c) (c + 'a' - 'A')
int toupper(int c);
# define _toupper(c) (c + 'A' - 'a')

#endif /* !UKERNEL_CTYPE_H */
