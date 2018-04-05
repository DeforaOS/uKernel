/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../drivers/boot/multiboot.h"
#include "../drivers/bus.h"
#include "../drivers/console.h"

#ifndef LOADER_BUS
# define LOADER_BUS	"ioport"
#endif
#ifndef LOADER_CONSOLE
# define LOADER_CONSOLE	"vga"
#endif


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
	const char msg_loading[] = "Loading module: ";
	const char msg_newline[] = "\n";
	ukMultibootMod * mod;

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
	if(mi->flags & BOOT_MULTIBOOT_HEADER_HAS_MODS)
	{
		puts(msg_modules);
		for(i = 0; i < mi->mods_count; i++)
		{
			puts(msg_loading);
			mod = &mi->mods_addr[i];
			puts(mod->string);
			puts(msg_newline);
		}
	}
	return 0;
}
