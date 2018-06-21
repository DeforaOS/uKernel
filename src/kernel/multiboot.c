/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdio.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"
# include "drivers/boot/multiboot.h"
# include "drivers/bus.h"
# include "drivers/console.h"

# ifndef LOADER_BUS
#  define LOADER_BUS		"ioport"
# endif
# ifndef LOADER_CONSOLE
#  define LOADER_CONSOLE	"vga"
# endif


/* private */
/* constants */
/* GDT: 4GB flat memory setup */
static const GDT _gdt_4gb[4] =
{
	{ 0x00000000, 0x00000000, 0x00 },
	{ 0x00000000, 0xffffffff, 0x9a },
	{ 0x00000000, 0xffffffff, 0x92 },
	{ 0x00000000, 0x00000000, 0x89 }
};


/* public */
/* functions */
/* multiboot */
int multiboot(ukMultibootInfo * mi)
{
	ukBus * bus;
	char const * console = LOADER_CONSOLE;
	size_t i;
	ukMultibootMod * mod;

	/* initialize the main bus */
	bus = bus_init(NULL, LOADER_BUS);

#ifdef notyet
	/* detect the video driver to use */
	if(mi->flags & BOOT_MULTIBOOT_INFO_HAS_VBE)
		console = "vesa";
#endif

	/* initialize the console */
	console_init(bus, console);

	/* report information on the boot process */
	puts("DeforaOS Multiboot");
	if(mi->loader_name != NULL)
		printf("Loader: %s\n", mi->loader_name);
	if(mi->cmdline != NULL)
		printf("Command line: %s\n", mi->cmdline);
	printf("%u MB memory available\n",
			(mi->mem_upper - mi->mem_lower) / 1024);
	printf("Booted from %#x\n", mi->boot_device_drive);

	/* load the modules */
#if defined(__amd64__)
	if(_arch_setgdt64(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#else
	if(_arch_setgdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#endif
	{
		puts("Could not setup the GDT");
		return 2;
	}

	if(!(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS))
		puts("No modules provided");
	else
	{
		puts("Loading modules...");
		for(i = 0; i < mi->mods_count; i++)
		{
			mod = &mi->mods_addr[i];
			multiboot_load_module(mod, NULL, NULL);
		}
	}
	return 0;
}
#endif
