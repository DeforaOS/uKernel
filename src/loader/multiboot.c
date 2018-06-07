/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__i386__)
# include <stdint.h>
# include <stdio.h>
# include <string.h>
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
int multiboot(const ukMultibootInfo * mi)
{
	ukBus * bus;
	char const * console = LOADER_CONSOLE;
	size_t i;
	ukMultibootMod * mod;
	int res = -1;
	unsigned char elfclass;
	vaddr_t entrypoint;
	ukMultibootInfo kmi;

	memcpy(&kmi, mi, sizeof(kmi));

	/* initialize the root bus */
	bus = bus_init(LOADER_BUS);

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

	/* look for the kernel and modules */
	if(!(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS))
	{
		puts("No modules provided");
		return 2;
	}
	if(mi->mods_count == 0)
	{
		puts("No kernel provided");
		return 3;
	}
	if(_arch_setgdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
	{
		puts("Could not setup the GDT");
		return 4;
	}

	/* load the kernel and modules */
	kmi.mods_count = 0;
	kmi.mods_addr = NULL;
	for(i = 0; i < mi->mods_count; i++)
	{
		mod = &mi->mods_addr[i];
		printf("Loading %s: %s\n", (i == 0) ? "kernel" : "module",
				mod->cmdline);
		if(i == 0)
		{
			res = multiboot_load_module(mod, &elfclass,
					&entrypoint);
			kmi.mods_count = mi->mods_count - 1;
			kmi.mods_addr = &mi->mods_addr[1];
		}
		else
			multiboot_load_module(mod, NULL, NULL);
	}
	if(res != 0)
	{
		puts("Could not load the kernel");
		return 5;
	}

	/* hand control over to the kernel */
	printf("Jumping into the kernel at %#x (%u, %u, %#x) %p\n", entrypoint,
			mi->elfshdr_num, mi->elfshdr_size, mi->elfshdr_addr,
			multiboot);
	switch(elfclass)
	{
		case ELFCLASS32:
			puts("Detected 32-bit kernel");
			return multiboot_boot_kernel32(&kmi, entrypoint);
		case ELFCLASS64:
			puts("Detected 64-bit kernel");
			return multiboot_boot_kernel64(&kmi, entrypoint);
	}
	puts("Unsupported ELF class for the kernel");
	return 6;
}
#endif
