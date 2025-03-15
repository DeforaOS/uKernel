/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__i386__)
# include <string.h>
# include <syslog.h>
# include <kernel/drivers/bus.h>
# include <kernel/drivers/console.h>
# include <kernel/drivers/clock.h>
# include <kernel/drivers/display.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"
# include "drivers/boot/multiboot.h"

# ifndef LOADER_CLOCK
#  define LOADER_CLOCK		"cmos"
# endif
# ifndef LOADER_CONSOLE
#  define LOADER_CONSOLE	"uart"
# endif
# ifndef LOADER_DISPLAY
#  define LOADER_DISPLAY	"vga"
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
	ukBus * ioportbus;
	ukBus * vgabus;
	ukBus * cmosbus;
	char const * clock = LOADER_CLOCK;
	char const * console = LOADER_CONSOLE;
	char const * display = LOADER_DISPLAY;
	ukMultibootMod * mod;
	unsigned char elfclass;
	vaddr_t entrypoint;
	ukMultibootInfo kmi;

	/* initialize the heap */
	multiboot_heap_reset(mi);

	/* initialize logging */
	openlog("uLoader", LOG_ODELAY, LOG_KERN);

	/* initialize the buses */
	ioportbus = bus_init(NULL, "ioport");
	vgabus = bus_init(ioportbus, "vga");
	cmosbus = bus_init(ioportbus, "cmos");

#ifdef notyet
	/* detect the video driver to use */
	if(mi->flags & BOOT_MULTIBOOT_INFO_HAS_VBE)
		display = "vesa";
#endif

	/* initialize the display */
	display_init(vgabus, display);

	/* initialize the console */
	console_init(ioportbus, console);

	/* initialize the clock */
	clock_init(cmosbus, clock);

	/* report information on the boot process */
	syslog(LOG_KERN | LOG_NOTICE, "%s", "DeforaOS Multiboot");
	if(mi->loader_name != NULL)
		syslog(LOG_KERN | LOG_INFO, "Loader: %s", mi->loader_name);
	if(mi->cmdline != NULL)
		syslog(LOG_KERN | LOG_INFO, "Command line: %s", mi->cmdline);
	syslog(LOG_KERN | LOG_INFO, "%u MB memory available",
			(mi->mem_upper - mi->mem_lower) / 1024);
	syslog(LOG_KERN | LOG_INFO, "Booted from %#x", mi->boot_device_drive);

	/* setup the GDT */
	if(_arch_setgdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
	{
		syslog(LOG_KERN | LOG_EMERG, "%s", "Could not setup the GDT");
		return 4;
	}

	/* load the kernel */
	if(!(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS))
	{
		syslog(LOG_KERN | LOG_NOTICE, "%s", "No modules provided");
		return 6;
	}
	if(mi->mods_count == 0)
	{
		syslog(LOG_KERN | LOG_ERR, "%s", "No kernel provided");
		return 7;
	}
	mod = &mi->mods_addr[0];
	syslog(LOG_KERN | LOG_NOTICE, "Loading kernel: %s", mod->cmdline);
	if(multiboot_load_module(mod, &elfclass, &entrypoint) != 0)
	{
		syslog(LOG_KERN | LOG_EMERG, "%s", "Could not load the kernel");
		return 8;
	}

	/* forward the Multiboot information */
	memcpy(&kmi, mi, sizeof(kmi));
	kmi.loader_name = "DeforaOS uLoader";
	kmi.cmdline = mod->cmdline;
	kmi.mods_count = mi->mods_count - 1;
	kmi.mods_addr = &mi->mods_addr[1];

	/* hand control over to the kernel */
#ifdef DEBUG
	syslog(LOG_KERN | LOG_DEBUG,
			"Jumping into the kernel at %#x (%u, %u, %#x)",
			entrypoint, mi->elfshdr_num, mi->elfshdr_size,
			mi->elfshdr_addr);
#endif
	switch(elfclass)
	{
		case ELFCLASS32:
			syslog(LOG_KERN | LOG_INFO, "%s",
					"Detected 32-bit kernel");
			return multiboot_boot_kernel32(&kmi, entrypoint);
		case ELFCLASS64:
			syslog(LOG_KERN | LOG_INFO, "%s",
					"Detected 64-bit kernel");
			return multiboot_boot_kernel64(&kmi, entrypoint);
	}

	syslog(LOG_KERN | LOG_EMERG, "%s",
			"Unsupported ELF class for the kernel");
	return 7;
}
#endif
