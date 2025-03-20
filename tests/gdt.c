/* $Id$ */
/* Copyright (c) 2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdio.h>
#ifndef vaddr_t
# define vaddr_t unsigned long
#endif
#include "../src/loader/gdt.c"


#if defined(__amd64__) || defined(__i386__)
/* variables */
static TSS tss;


/* functions */
/* __arch_setgdt */
void __arch_setgdt(GDTEntry const * entries, size_t count)
{
	size_t i;
	GDTEntry const * entry;

	printf("sizeof(*entries) => %zu\n", sizeof(*entries));
	for(i = 0; i < count; i++)
	{
		entry = &entries[i];
		printf("base=0x%02x%02x%02x%02x ",
				entry->base3, entry->base2,
				entry->base1, entry->base0);
		printf("limit=0x%x%02x%02x ",
				entry->limit2, entry->limit1, entry->limit0);
		printf("access=0x%02x ", entry->access);
		printf("flags=0x%0x\n", entry->flags);
	}
	printf("count=0x%zx => size=0x%zx\n", count, (count << 3) - 1);
}
#endif


/* main */
int main(int argc, char * argv[])
{
#if defined(__amd64__) || defined(__i386__)
	GDT * gdt;
	(void) argc;
	(void) argv;

	printf("sizeof(GDTEntry) = %zu (expected: %u)\n\n",
			sizeof(GDTEntry), 8);

	/* flat 4 GB */
	printf("Flat 4 GB:\n");
	gdt = gdt_init();
	gdt_append(gdt, 0x00000000, 0xffffffff, PROT_READ | PROT_EXEC);
	gdt_append(gdt, 0x00000000, 0xffffffff, PROT_READ | PROT_WRITE);
	gdt_append_system(gdt, &tss, sizeof(tss), GDT_SYSTEM_TYPE_TSS);
	gdt_apply(gdt);

	/* 4 MB code + 4 MB data (read-write) + 4 MB data (read-only) */
	printf("\n4 MB (rx) + 4 MB (rw) + 4 MB (ro):\n");
	gdt = gdt_init();
	gdt_append(gdt, 0x00400000, 0x00400000, PROT_READ | PROT_EXEC);
	gdt_append(gdt, 0x00800000, 0x00400000, PROT_READ | PROT_WRITE);
	gdt_append(gdt, 0x00c00000, 0x00400000, PROT_READ);
	gdt_append_system(gdt, &tss, sizeof(tss), GDT_SYSTEM_TYPE_TSS);
	gdt_apply(gdt);

#endif
	(void) argc;
	(void) argv;

	return 0;
}
