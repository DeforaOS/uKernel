/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STRING_H
# define UKERNEL_STRING_H

# include <stddef.h>
# include <stdint.h>


/* prototypes */
int memcmp(void const * s1, void const * s2, size_t n);
void * memcpy(void * dest, void const * src, size_t n);
void * memmove(void * dest, void const * src, size_t n);
void * memset(void * dest, int c, size_t n);
int strcmp(char const * s1, char const * s2);
char * strcpy(char * dest, char const * src);
size_t strlen(char const * s);
int strncmp(char const * s1, char const * s2, size_t n);
char * strncpy(char * dest, char const * src, size_t n);

#endif /* !UKERNEL_STRING_H */
