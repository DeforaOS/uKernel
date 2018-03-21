/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STRING_H
# define UKERNEL_STRING_H

# include <stddef.h>
# include <stdint.h>


/* prototypes */
void * memcpy(void * dest, void const * src, size_t n);
void * memmove(void * dest, void const * src, size_t n);
void * memset(void * dest, int c, size_t n);

#endif /* !UKERNEL_STRING_H */
