/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ARCH_I386_GDT_H
# define UKERNEL_ARCH_I386_GDT_H

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

#pragma pack(1)
typedef struct _TSS
{
	uint16_t link;
	uint16_t __padding0;
	uint32_t esp0;
	uint16_t ss0;
	uint16_t __padding1;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t __padding2;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t __padding3;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t __padding4;
	uint16_t cs;
	uint16_t __padding5;
	uint16_t ss;
	uint16_t __padding6;
	uint16_t ds;
	uint16_t __padding7;
	uint16_t fs;
	uint16_t __padding8;
	uint16_t gs;
	uint16_t __padding9;
	uint16_t ldtr;
	uint16_t __padding10;
	uint16_t __padding11;
	uint16_t iopb;
	uint32_t ssp;
} TSS;
#pragma pack()


/* constants */
# define GDT_PROT_READ		0x1
# define GDT_PROT_WRITE		0x2
# define GDT_PROT_EXEC		0x4

# define GDT_SYSTEM_TYPE_LDT	0x2
# define GDT_SYSTEM_TYPE_TSS	0x9


/* prototypes */
GDT * gdt_init(void);
int gdt_init_table(GDTTable const * table, size_t table_cnt, size_t tss_cnt);


/* useful */
int gdt_append(GDT * gdt, vaddr_t base, size_t size, unsigned int prot);
int gdt_append_system(GDT * gdt, void * base, size_t size, unsigned int type);

void gdt_apply(GDT * gdt);

#endif /* !UKERNEL_ARCH_I386_GDT_H */
