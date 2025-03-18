/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__i386__)
# include <stdio.h>
# include <string.h>
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
static const GDTTable _gdt_4gb[2] =
{
	{ 0x00000000, 0xffffffff, PROT_READ | PROT_EXEC },
	{ 0x00000000, 0xffffffff, PROT_READ | PROT_WRITE }
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
	puts("DeforaOS Multiboot");
	if(mi->loader_name != NULL)
		printf("Loader: %s\n", mi->loader_name);
	if(mi->cmdline != NULL)
		printf("Command line: %s\n", mi->cmdline);
	printf("%u MB memory available\n",
			(mi->mem_upper - mi->mem_lower) / 1024);
	printf("Booted from %#x\n", mi->boot_device_drive);

	/* setup the GDT */
	if(gdt_init_table((const GDTTable *)&_gdt_4gb,
				sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
	{
		puts("Could not setup the GDT");
		return 4;
	}

	/* load the kernel */
	if(!(mi->flags & BOOT_MULTIBOOT_INFO_HAS_MODS))
	{
		puts("No modules provided");
		return 6;
	}
	if(mi->mods_count == 0)
	{
		puts("No kernel provided");
		return 7;
	}
	mod = &mi->mods_addr[0];
	printf("Loading kernel: %s\n", mod->cmdline);
	if(multiboot_load_module(mod, &elfclass, &entrypoint) != 0)
	{
		puts("Could not load the kernel");
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
	printf("Jumping into the kernel at %#x (%u, %u, %#x)\n", entrypoint,
			mi->elfshdr_num, mi->elfshdr_size, mi->elfshdr_addr);
#endif
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
	return 7;
}
#endif
