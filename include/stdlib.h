/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDLIB_H
# define UKERNEL_STDLIB_H

# include <stddef.h>
# include <stdint.h>


/* prototypes */
void abort(void);
int abs(int x);
uint32_t arc4random(void);
void * calloc(size_t nmemb, size_t size);
void exit(int status);
void free(void * ptr);
long labs(long x);
long long llabs(long long x);
void * malloc(size_t size);

#endif /* !UKERNEL_STDLIB_H */
