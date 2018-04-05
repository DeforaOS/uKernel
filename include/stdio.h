/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDIO_H
# define UKERNEL_STDIO_H

# include <stddef.h>


/* types */
typedef struct _FILE FILE;

# ifndef va_list
#   define va_list va_list
typedef __builtin_va_list va_list;
# endif


/* constants */
/* size of <stdio.h> buffers */
# define BUFSIZ 8192

# define _IOFBF	0
# define _IOLBF	1
# define _IONBF	2

/* end-of-file return value */
# define EOF (-1)


/* variables */
/* standard input, output, and error streams */
extern FILE * stdin;
extern FILE * stdout;
extern FILE * stderr;


/* prototypes */
int fputc(int c, FILE * file);
size_t fwrite(void const * ptr, size_t size, size_t nb, FILE * file);
int printf(char const * format, ...);
int putc(int c, FILE * file);
int putchar(int c);
int puts(char const * string);
int vfprintf(FILE * file, char const * format, va_list arg);

#endif /* !UKERNEL_STDIO_H */
