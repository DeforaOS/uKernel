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
		return 4;
	}
	return 0;
}
