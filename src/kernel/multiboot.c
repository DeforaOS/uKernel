/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdio.h>
# include "drivers/boot/multiboot.h"

# ifndef KERNEL_CONSOLE
#  define KERNEL_CONSOLE	"uart"
# endif
# ifndef KERNEL_CONSOLE_BUS
#  define KERNEL_CONSOLE_BUS	"ioport"
# endif
# ifndef KERNEL_DISPLAY
#  define KERNEL_DISPLAY	"vga"
# endif
# ifndef KERNEL_DISPLAY_BUS
#  define KERNEL_DISPLAY_BUS	"vga"
# endif


/* public */
/* functions */
/* multiboot */
int multiboot(ukMultibootInfo * mi)
{
	char const * console = KERNEL_CONSOLE;
	char const * display = KERNEL_DISPLAY;
	size_t i;
	ukMultibootMod * mod;

	boot_init(boot, mi->loader_name, mi->cmdline,
			mi->mem_lower * 1024, mi->mem_upper * 1024);

	boot_set_console(boot, console, KERNEL_CONSOLE_BUS);

#ifdef notyet
	/* detect the video driver to use */
	if(mi->flags & BOOT_MULTIBOOT_INFO_HAS_VBE)
		display = "vesa";
#endif
	boot_set_display(boot, display, KERNEL_DISPLAY_BUS);

#if 0
	/* report information on the boot process */
	puts("DeforaOS Multiboot");
	if(mi->loader_name != NULL)
		printf("Loader: %s\n", mi->loader_name);
	if(mi->cmdline != NULL)
		printf("Command line: %s\n", mi->cmdline);
	printf("%u MB memory available\n",
			(mi->mem_upper - mi->mem_lower) / 1024);
	printf("Booted from %#x\n", mi->boot_device_drive);
#endif

	/* register the modules */
	if(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS)
		for(i = 0; i < mi->mods_count; i++)
		{
			mod = &mi->mods_addr[i];
			boot_module_register(boot, NULL, mod->cmdline,
					mod->start, mod->end);
		}

	return 0;
}
#endif
