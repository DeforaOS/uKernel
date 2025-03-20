/* $Id$ */
/* Copyright (c) 2018-2025 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdio.h>
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
static const GDTTable _gdt_4gb[2] =
{
	{ 0x00000000, 0xffffffff, GDT_PROT_READ | GDT_PROT_EXEC },
	{ 0x00000000, 0xffffffff, GDT_PROT_READ | GDT_PROT_WRITE }
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
	puts("DeforaOS Multiboot");
	if(mi->loader_name != NULL)
		printf("Loader: %s\n", mi->loader_name);
	if(mi->cmdline != NULL)
		printf("Command line: %s\n", mi->cmdline);
	printf("%u MB memory available\n",
			(mi->mem_upper - mi->mem_lower) / 1024);
	printf("Booted from %#x\n", mi->boot_device_drive);

	/* setup the GDT */
#if defined(__amd64__)
	if(_arch_setgdt64(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#else
	if(gdt_init_table((const GDTTable *)&_gdt_4gb,
				sizeof(_gdt_4gb) / sizeof(*_gdt_4gb), 1) != 0)
#endif
	{
		puts("Could not setup the GDT");
		return 4;
	}

	/* load the modules */
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

	/* setup the IDT */
	if(_arch_setidt(_idt, sizeof(_idt) / sizeof(*_idt)) != 0)
	{
		puts("Could not setup the IDT");
		return 5;
	}

	return 0;
}
#endif
