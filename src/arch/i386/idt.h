/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ARCH_I386_IDT_H
# define UKERNEL_ARCH_I386_IDT_H

# include <stdint.h>


/* public */
/* types */
typedef struct _IDT
{
	uint32_t base;
	uint16_t selector;
	uint8_t flags;
} IDT;


/* prototypes */
int _arch_setidt(IDT const * idt, size_t count);

#endif /* !UKERNEL_ARCH_I386_IDT_H */
