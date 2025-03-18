/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ARCH_I386_GDT_H
# define UKERNEL_ARCH_I386_GDT_H

# include <sys/mman.h>
# include <sys/types.h>
# include <stdint.h>


/* public */
/* types */
typedef struct _GDT GDT;

typedef struct _GDTTable
{
	vaddr_t base;
	size_t size;
	unsigned int prot;
} GDTTable;


/* prototypes */
GDT * gdt_init(void);
int gdt_init_table(GDTTable const * table, size_t count);


/* useful */
int gdt_append(GDT * gdt, vaddr_t base, size_t size, unsigned int prot);

void gdt_apply(GDT * gdt);

#endif /* !UKERNEL_ARCH_I386_GDT_H */
