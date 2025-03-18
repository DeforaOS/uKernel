/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <limits.h>
# include <stdint.h>
# include <string.h>
# include <errno.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"


/* constants */
# define GDT_ENTRIES_MAX		8192
# define GDT_LIMIT_MAX			0x000fffff

/* access */
# define GDT_ACCESS_SET			0x01
# define GDT_ACCESS_RW			0x02
# define GDT_ACCESS_X			0x08
# define GDT_ACCESS_SEGMENT		0x10
# define GDT_ACCESS_RING(level)		((level) << 5)
# define GDT_ACCESS_PRESENT		0x80

/* flags */
# define GDT_FLAG_LONG_MODE		0x2	/* 64-bit code segment */
# define GDT_FLAG_PROTECTED_MODE	0x4	/* 32-bit protected mode
						   code segment */
# define GDT_FLAG_PAGE_GRANULARITY	0x8	/* 4 KB page size */


/* types */
#pragma pack(1)
typedef struct _GDTEntry
{
	uint8_t limit0;
	uint8_t limit1;
	uint8_t base0;
	uint8_t base1;
	uint8_t base2;
	uint8_t access;
	unsigned int limit2:4;
	unsigned int flags:4;
	uint8_t base3;
} GDTEntry;
#pragma pack()

struct _GDT
{
	GDTEntry entries[GDT_ENTRIES_MAX];
	size_t entries_cnt;
};


/* prototypes */
extern void __arch_setgdt(GDTEntry const * entries, size_t count);


/* variables */
static GDT _gdt;


/* functions */
/* gdt_init */
GDT * gdt_init(void)
{
	memset(&_gdt.entries[0], 0, sizeof(_gdt.entries[0]));
	_gdt.entries_cnt = 1;
	return &_gdt;
}


/* gdt_init_table */
int gdt_init_table(GDTTable const * table, size_t count)
{
	int ret = 0;
	GDT * gdt;
	size_t i;

	gdt = gdt_init();
	for(i = 0; i < count; i++)
		if((ret = gdt_append(gdt, table[i].base, table[i].size,
						table[i].prot)) != 0)
			return ret;
	gdt_apply(gdt);
	return 0;
}


/* useful */
/* gdt_append */
int gdt_append(GDT * gdt, vaddr_t base, size_t size, unsigned int prot)
{
	GDTEntry * entry;
	uint32_t limit;
	uint8_t access = GDT_ACCESS_PRESENT
		| GDT_ACCESS_SEGMENT | GDT_ACCESS_RING(0);
	uint8_t flags = GDT_FLAG_PROTECTED_MODE;

	/* check for errors */
	if(size == 0)
	{
		errno = ERANGE;
		return -1;
	}
	if(gdt->entries_cnt >= sizeof(gdt->entries) / sizeof(*gdt->entries)
			|| size > ULONG_MAX
			|| ULONG_MAX - size < base)
	{
		errno = ENOMEM;
		return -1;
	}
	if(prot != PROT_READ
			&& prot != (PROT_READ | PROT_WRITE)
			&& prot != (PROT_READ | PROT_EXEC))
	{
		errno = EPERM;
		return -1;
	}
	entry = &gdt->entries[gdt->entries_cnt];
	/* base */
	entry->base0 = base & 0xff;
	entry->base1 = (base & 0xff00) >> 8;
	entry->base2 = (base & 0xff0000) >> 16;
	entry->base3 = (base & 0xff000000) >> 24;
	/* limit */
	if(size - 1 > GDT_LIMIT_MAX)
	{
		limit = (size & 0xfff) == 0
			? (size >> 12)
			: (((size | 0xfff) + 1) >> 12);
		limit--;
		flags |= GDT_FLAG_PAGE_GRANULARITY;
	}
	else
		limit = size - 1;
	entry->limit0 = limit & 0xff;
	entry->limit1 = (limit & 0xff00) >> 8;
	entry->limit2 = (limit & 0xf0000) >> 16;
	/* access */
	if(prot == (PROT_READ | PROT_EXEC))
		/* code segment */
		access |= GDT_ACCESS_RW | GDT_ACCESS_X;
	else if(prot == (PROT_READ | PROT_WRITE))
		/* data segment (read/write) */
		access |= GDT_ACCESS_RW;
	else if(prot == PROT_READ)
		/* data segment (read-only) */
		access |= GDT_ACCESS_SET;
	entry->access = access;
	/* flags */
	entry->flags = flags;
	gdt->entries_cnt++;
	return 0;
}


/* gdt_apply */
void gdt_apply(GDT * gdt)
{
	__arch_setgdt(gdt->entries, gdt->entries_cnt);
}
#endif
