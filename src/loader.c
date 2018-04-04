/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include <string.h>
#include "drivers/boot/multiboot.h"
#include "drivers/bus.h"
#include "drivers/console.h"

#ifndef MAIN_BUS
# define MAIN_BUS	"ioport"
#endif


/* public */
/* functions */
/* main */
int main(ukMultibootInfo * mi)
{
	ukBus * bus;
	ukConsole * console;
	const char msg_starting[] = "Booting DeforaOS...\n";
	const char msg_loader[] = "Loader: ";
	const char msg_cmdline[] = "Command line: ";
	const char msg_modules[] = "Loading modules...\n";
	size_t i;
	const char msg_loading[] = "Loading module: ";
	const char msg_newline[] = "\n";
	ukMultibootMod * mod;

	bus = bus_init(MAIN_BUS);
	console = console_init(bus);
	console_print(console, msg_starting, sizeof(msg_starting) - 1);
	if(mi->loader_name != NULL)
	{
		console_print(console, msg_loader, sizeof(msg_loader) - 1);
		console_print(console, mi->loader_name,
				strlen(mi->loader_name));
		console_print(console, msg_newline, sizeof(msg_newline) - 1);
	}
	if(mi->cmdline != NULL)
	{
		console_print(console, msg_cmdline, sizeof(msg_cmdline) - 1);
		console_print(console, mi->cmdline, strlen(mi->cmdline));
		console_print(console, msg_newline, sizeof(msg_newline) - 1);
	}
	if(mi->flags & BOOT_MULTIBOOT_HEADER_HAS_MODS)
	{
		console_print(console, msg_modules, sizeof(msg_modules) - 1);
		for(i = 0; i < mi->mods_count; i++)
		{
			console_print(console, msg_loading,
					sizeof(msg_loading) - 1);
			mod = &mi->mods_addr[i];
			console_print(console, mod->string,
					strlen(mod->string));
			console_print(console, msg_newline,
					sizeof(msg_newline) - 1);
		}
	}
	return 0;
}
