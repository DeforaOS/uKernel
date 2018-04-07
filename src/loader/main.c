/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include "drivers/boot/multiboot.h"
#include "drivers/bus.h"
#include "drivers/console.h"

#ifndef LOADER_BUS
# define LOADER_BUS	"ioport"
#endif
#ifndef LOADER_CONSOLE
# define LOADER_CONSOLE	"vga"
#endif


/* private */
/* prototypes */
static vaddr_t _loader_kernel(ukMultibootMod * mod);
static int _loader_module(ukMultibootMod * mod);


/* platform-dependent */
#if defined(__i386__)
/* types */
typedef struct _GDT
{
	vaddr_t base;
	vaddr_t limit;
	uint8_t type;
} GDT;

/* prototypes */
static int _loader_gdt(GDT const * gdt, size_t size);
extern void _setgdt(uint8_t * buf, size_t size);

/* constants */
/* GDT: flat memory setup */
static const GDT _gdt_4gb[4] =
{
	{ 0x00000000, 0x00000000, 0x00 },
	{ 0x00000000, 0xffffffff, 0x9a },
	{ 0x00000000, 0xffffffff, 0x92 },
	{ 0x00000000, 0x00000000, 0x89 }
};

/* variables */
static uint8_t _buf[65536];

/* functions */
/* loader_gdt */
static int _loader_gdt(GDT const * gdt, size_t size)
{
	uint8_t * buf;
	size_t i;
	GDT g;

	memset(&_buf, 0, sizeof(_buf));
	if(size > sizeof(_buf) / sizeof(*gdt))
		return -1;
	for(i = 0; i < size; i++)
	{
		g = gdt[i];
		buf = &_buf[sizeof(g) * i];
		if(g.limit > 65536)
		{
			/* make sure the limit can be encoded */
			if((g.limit & 0xfff) != 0xfff)
				return -1;
			g.limit = g.limit >> 12;
			buf[6] = 0xc0;
		}
		else
			buf[6] = 0x40;
		//encode the limit
		buf[0] = g.limit & 0xff;
		buf[1] = (g.limit >> 8) & 0xff;
		buf[6] |= (g.limit >> 16) & 0xf;
		//encode the base
		buf[2] = g.base & 0xff;
		buf[3] = (g.base >> 8) & 0xff;
		buf[4] = (g.base >> 16) & 0xff;
		buf[7] = (g.base >> 24) & 0xff;
		//encode the type
		buf[5] = g.type;
	}
	_setgdt(buf, size);
	return 0;
}
#endif


/* functions */
/* loader_kernel */
static vaddr_t _loader_kernel(ukMultibootMod * mod)
{
	Elf32_Ehdr ehdr;

	printf("Loading kernel: %s\n", mod->cmdline);
	if(mod->end < mod->start || mod->end - mod->start < sizeof(ehdr))
	{
		puts("Could not load kernel: Invalid format");
		return 0x0;
	}
	memcpy(&ehdr, (void *)mod->start, sizeof(ehdr));
	switch(ehdr.e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			puts("Detected 32-bit kernel");
			break;
		case ELFCLASS64:
			puts("Detected 64-bit kernel");
			return 0x0;
		default:
			puts("Could not load kernel: Invalid class");
			return 0x0;
	}
	return ehdr.e_entry;
}


/* loader_module */
static int _loader_module(ukMultibootMod * mod)
{
	const char msg_loading[] = "Loading module: ";

	puts(msg_loading);
	puts(mod->cmdline);
	return 0;
}


/* public */
/* functions */
/* main */
int main(ukMultibootInfo * mi)
{
	ukBus * bus;
	size_t i;
	const char msg_failed2[] = "No modules provided";
	const char msg_failed3[] = "No kernel provided";
	ukMultibootMod * mod;
	vaddr_t entrypoint = 0x0;

	bus = bus_init(LOADER_BUS);
	console_init(bus, LOADER_CONSOLE);
	puts("Booting DeforaOS...");
	if(mi->loader_name != NULL)
		printf("Loader: %s\n", mi->loader_name);
	if(mi->cmdline != NULL)
		printf("Command line: %s\n", mi->cmdline);
	if(!(mi->flags & BOOT_MULTIBOOT_HEADER_HAS_MODS))
	{
		puts(msg_failed2);
		return 2;
	}
	if(mi->mods_count == 0)
	{
		puts(msg_failed3);
		return 3;
	}
#if defined(__i386__)
	if(_loader_gdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
	{
		puts("Could not setup the GDT");
		return 4;
	}
#endif
	puts("Loading modules...");
	for(i = 0; i < mi->mods_count; i++)
	{
		mod = &mi->mods_addr[i];
		if(i == 0)
			entrypoint = _loader_kernel(mod);
		else
			_loader_module(mod);
	}
	if(entrypoint == 0x0)
	{
		puts("Could not load the kernel");
		return 5;
	}
	return 0;
}
