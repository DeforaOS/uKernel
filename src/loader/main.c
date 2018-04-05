/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include "../drivers/boot/multiboot.h"
#include "../drivers/bus.h"
#include "../drivers/console.h"

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
	const char msg_loading[] = "Loading kernel: ";
	const char msg_format[] = "Could not load kernel: Invalid format";
	const char msg_class[] = "Could not load kernel: Invalid class";
	const char msg_class32[] = "Detected 32-bit kernel";
	const char msg_class64[] = "Detected 64-bit kernel";
	Elf32_Ehdr ehdr;

	puts(msg_loading);
	puts(mod->cmdline);
	if(mod->end < mod->start || mod->end - mod->start < sizeof(ehdr))
	{
		puts(msg_format);
		return 0x0;
	}
	memcpy(&ehdr, (void *)mod->start, sizeof(ehdr));
	switch(ehdr.e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			puts(msg_class32);
			break;
		case ELFCLASS64:
			puts(msg_class64);
			return 0x0;
		default:
			puts(msg_class);
			return 0x0;
	}
	return ehdr.e_entry;
}


/* loader_module */
static int _loader_module(ukMultibootMod * mod)
{
	const char msg_loading[] = "Loading module: ";
	const char msg_newline[] = "\n";

	puts(msg_loading);
	puts(mod->cmdline);
	puts(msg_newline);
	return 0;
}


/* public */
/* functions */
/* main */
int main(ukMultibootInfo * mi)
{
	ukBus * bus;
	const char msg_starting[] = "Booting DeforaOS...\n";
	const char msg_loader[] = "Loader: ";
	const char msg_cmdline[] = "Command line: ";
	const char msg_modules[] = "Loading modules...\n";
	size_t i;
	const char msg_failed2[] = "No modules provided\n";
	const char msg_failed3[] = "No kernel provided\n";
	const char msg_failed4[] = "Could not load the kernel";
	const char msg_newline[] = "\n";
	ukMultibootMod * mod;
	vaddr_t entrypoint = 0x0;

	bus = bus_init(LOADER_BUS);
	console_init(bus, LOADER_CONSOLE);
	puts(msg_starting);
	if(mi->loader_name != NULL)
	{
		puts(msg_loader);
		puts(mi->loader_name);
		puts(msg_newline);
	}
	if(mi->cmdline != NULL)
	{
		puts(msg_cmdline);
		puts(mi->cmdline);
		puts(msg_newline);
	}
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
	puts(msg_modules);
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
		puts(msg_failed4);
		return 4;
	}
	return 0;
}
