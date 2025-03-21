/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ARCH_I386_IDT_H
# define UKERNEL_ARCH_I386_IDT_H

# include <stdint.h>


/* public */
/* types */
typedef struct _IDT IDT;

typedef struct _IDTEntry
{
	uint16_t offsetl;
	uint16_t selector;
	uint8_t __padding;
	uint8_t type:4;
	uint8_t flags:4;
	uint16_t offseth;
} IDTEntry;


/* constants */
# define IDT_FLAG_PRESENT	0x8
# define IDT_FLAG_RING(level)	((level) << 1)

# define IDT_TYPE_INT_GATE	0xe
# define IDT_TYPE_TRAP_GATE	0xf


/* prototypes */
IDT * idt_init(void);

#endif /* !UKERNEL_ARCH_I386_IDT_H */
