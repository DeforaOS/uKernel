/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ARCH_I386_GDT_H
# define UKERNEL_ARCH_I386_GDT_H

# include <sys/types.h>
# include <stdint.h>


/* public */
/* types */
typedef struct _GDT
{
	vaddr_t base;
	vaddr_t limit;
	uint8_t type;
} GDT;


/* prototypes */
int _arch_setgdt(GDT const * gdt, size_t count);

#endif /* !UKERNEL_ARCH_I386_GDT_H */
