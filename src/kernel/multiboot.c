/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <syslog.h>
# include <kernel/drivers/bus.h>
# include <kernel/drivers/clock.h>
# include <kernel/drivers/console.h>
# include <kernel/drivers/display.h>
# include <kernel/drivers/pic.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"
# include "arch/i386/idt.h"
# include "drivers/boot/multiboot.h"

# ifndef KERNEL_CONSOLE
#  define KERNEL_CONSOLE	"uart"
# endif
# ifndef KERNEL_DISPLAY
#  define KERNEL_DISPLAY	"vga"
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

static const IDT _idt[] =
{
};


/* public */
/* functions */
/* multiboot */
int multiboot(const ukMultibootInfo * mi)
{
	ukBus * ioportbus;
	ukBus * vgabus;
	ukBus * cmosbus;
	char const * console = KERNEL_CONSOLE;
	char const * display = KERNEL_DISPLAY;
	uint32_t i;
	ukMultibootMod * mod;

	/* initialize the heap */
	multiboot_heap_reset(mi);

	/* initialize logging */
	openlog("uKernel", LOG_ODELAY, LOG_KERN);

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

	/* initialize the PIC */
	pic_init(ioportbus, "i8259a");

	/* initialize the clock */
	clock_init(cmosbus, "cmos");

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
#if defined(__amd64__)
	if(_arch_setgdt64(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#else
	if(_arch_setgdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#endif
	{
		syslog(LOG_KERN | LOG_EMERG, "%s", "Could not setup the GDT");
		return 4;
	}

	/* load the modules */
	if(!(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS))
		syslog(LOG_KERN | LOG_INFO, "%s", "No modules provided");
	else
	{
		syslog(LOG_KERN | LOG_INFO, "%s", "Loading modules...");
		for(i = 0; i < mi->mods_count; i++)
		{
			mod = &mi->mods_addr[i];
			multiboot_load_module(mod, NULL, NULL);
		}
	}

	/* setup the IDT */
	if(_arch_setidt(_idt, sizeof(_idt) / sizeof(*_idt)) != 0)
	{
		syslog(LOG_KERN | LOG_EMERG, "%s", "Could not setup the IDT");
		return 5;
	}

	return 0;
}
#endif
