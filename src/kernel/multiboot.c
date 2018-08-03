/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdio.h>
# include "drivers/boot/multiboot.h"
# include "config.h"


/* public */
/* functions */
/* multiboot */
int multiboot(ukMultibootInfo * mi)
{
	char const * display = CONFIG_DEFAULT_DISPLAY;
	char const * bus;
	size_t i;
	ukMultibootMod * mod;

	boot_init(boot, mi->loader_name, mi->cmdline,
			mi->mem_lower * 1024, mi->mem_upper * 1024);

	/* detect the video driver to use */
	bus = CONFIG_DEFAULT_DISPLAY_BUS;
#ifdef notyet
	if(mi->flags & BOOT_MULTIBOOT_INFO_HAS_VBE)
	{
		display = "vesa";
		bus = "vga";
	}
#endif
	boot_set_display(boot, display, bus);

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
