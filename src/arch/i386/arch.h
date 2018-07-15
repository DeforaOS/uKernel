/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_ARCH_I386_H
# define UKERNEL_ARCH_I386_H

# include <stdint.h>


/* public */
/* prototypes */
int inb(uint16_t addr, uint8_t * value);
int outb(uint16_t addr, uint8_t value);

int iowait(void);

#endif /* !UKERNEL_ARCH_I386_H */
