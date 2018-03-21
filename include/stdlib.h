/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDLIB_H
# define UKERNEL_STDLIB_H

# include <stddef.h>
# include <stdint.h>


/* prototypes */
void abort(void);
uint32_t arc4random(void);
void exit(int status);

#endif /* !UKERNEL_STDLIB_H */
