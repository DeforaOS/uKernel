/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <kernel/platform.h>
#include "arch/amd64/gdt.h"
#include "arch/i386/gdt.h"
#include "arch/i386/idt.h"
#include "drivers/boot/multiboot.h"
#include "drivers/bus.h"
#include "drivers/clock.h"
#include "drivers/console.h"
#include "drivers/display.h"
#include "drivers/pic.h"

#define TO_STRING(s) "" # s


/* ukPlatform */
/* private */
/* types */
typedef struct _ukPlatformDriver
{
	ukDriverType type;
	void * driver;
} ukPlatformDriver;

struct _ukPlatform
{
	char const * arch;

	/* boot */
	ukBoot * boot;

	/* drivers */
	ukPlatformDriver * drivers;
	size_t drivers_cnt;
};


/* constants */
static const ukPlatformDevice _devices[] =
{
	{ UKDT_BUS,	"ioport",	NULL	},
	{ UKDT_BUS,	"vga",		"ioport"},
	{ UKDT_BUS,	"cmos",		"ioport"},
	{ UKDT_CLOCK,	"cmos",		"cmos"	},
	{ UKDT_PIC,	"i8259a",	"ioport"}
};

static char const * _driver_types[UKDT_COUNT] =
{
	"unknown", "bus", "clock", "console", "display", "pic"
};

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


/* variables */
static ukPlatform _platform =
{
	TO_STRING(ARCH),
	NULL,
	NULL, 0
};


/* public */
/* variables */
ukPlatform * platform = &_platform;


/* functions */
/* essential */
void platform_init(ukPlatform * platform, ukBoot * boot)
{
	platform->boot = boot;
}


/* accessors */
/* platform_get_arch */
char const * platform_get_arch(ukPlatform * platform)
{
	return platform->arch;
}


/* platform_get_physmem */
void platform_get_physmem(ukPlatform * platform, paddr_t * low, paddr_t * high)
{
	boot_get_physmem(platform->boot, low, high);
}


/* useful */
/* platform_device_lookup */
void * platform_device_lookup(ukPlatform * platform, ukDriverType type,
		char const * name)
{
	size_t i;
	ukPlatformDriver * driver;
	char const * p;

	for(i = 0; i < platform->drivers_cnt; i++)
	{
		driver = &platform->drivers[i];
		if(driver->type != type)
			continue;
		switch(type)
		{
			case UKDT_BUS:
				p = bus_get_name(driver->driver);
				if(p != NULL && strcmp(p, name) == 0)
					return driver->driver;
				break;
			case UKDT_CLOCK:
				p = clock_get_name(driver->driver);
				if(p != NULL && strcmp(p, name) == 0)
					return driver->driver;
				break;
			case UKDT_CONSOLE:
				p = console_get_name(driver->driver);
				if(p != NULL && strcmp(p, name) == 0)
					return driver->driver;
				break;
			case UKDT_DISPLAY:
				p = display_get_name(driver->driver);
				if(p != NULL && strcmp(p, name) == 0)
					return driver->driver;
				break;
			case UKDT_PIC:
				p = pic_get_name(driver->driver);
				if(p != NULL && strcmp(p, name) == 0)
					return driver->driver;
				break;
			case UKDT_UNKNOWN:
				break;
		}
	}
	return NULL;
}


/* platform_device_register */
static ukBus * _device_register_get_bus(ukPlatform * platform,
		char const * name);

int platform_device_register(ukPlatform * platform,
		const ukPlatformDevice * device)
{
	int ret = 0;
	ukBus * bus;
	ukPlatformDriver * p;
	ukPlatformDriver * driver;

	if(device->bus != NULL)
		bus = _device_register_get_bus(platform, device->bus);
	else
		bus = NULL;
	if((p = realloc(platform->drivers, sizeof(*p)
					* (platform->drivers_cnt + 1))) == NULL)
		return -1;
	platform->drivers = p;
	driver = &platform->drivers[platform->drivers_cnt];
	driver->type = device->type;
	driver->driver = NULL;
	switch(device->type)
	{
		case UKDT_BUS:
			driver->driver = bus_init(bus, device->name);
			break;
		case UKDT_CLOCK:
			driver->driver = clock_init(bus, device->name);
			break;
		case UKDT_CONSOLE:
			driver->driver = console_init(bus, device->name);
			break;
		case UKDT_DISPLAY:
			driver->driver = display_init(bus, device->name);
			break;
		case UKDT_PIC:
			driver->driver = pic_init(bus, device->name);
			break;
		case UKDT_UNKNOWN:
			errno = ENOSYS;
			break;
	}
	if(driver->driver == NULL)
		return -1;
	platform->drivers++;
	fprintf(stderr, "%s %s%s%s%s\n", device->name,
			_driver_types[device->type],
			(device->bus != NULL) ? " at " : "",
			(device->bus != NULL) ? device->bus : "",
			(device->bus != NULL) ? " bus" : "");
	return ret;
}

static ukBus * _device_register_get_bus(ukPlatform * platform,
		char const * name)
{
	size_t i;
	ukPlatformDevice const * device;
	ukBus * bus;

	if((bus = platform_device_lookup(platform, UKDT_BUS, name)) != NULL)
		return bus;
	for(i = 0; i < sizeof(_devices) / sizeof(*_devices); i++)
	{
		device = &_devices[i];
		if(device->type != UKDT_BUS)
			continue;
		if(strcmp(device->name, name) != 0)
			continue;
		if(platform_device_register(platform, device) != 0)
			continue;
		return platform_device_lookup(platform, UKDT_BUS, name);
	}
	return NULL;
}


/* platform_start */
static int _start_console(ukPlatform * platform);
static int _start_devices(ukPlatform * platform,
		ukPlatformDevice const * devices, size_t devices_cnt);
static int _start_display(ukPlatform * platform);
static void _start_module_load(void * data, const ukBootModule * module);

int platform_start(ukPlatform * platform)
{
	_start_console(platform);
	_start_display(platform);
	_start_devices(platform, _devices,
			sizeof(_devices) / sizeof(*_devices));

	/* setup the GDT */
#if defined(__amd64__)
	if(_arch_setgdt64(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#else
	if(_arch_setgdt(_gdt_4gb, sizeof(_gdt_4gb) / sizeof(*_gdt_4gb)) != 0)
#endif
	{
		puts("Could not setup the GDT");
		return 2;
	}

	/* load the modules */
	boot_module_foreach(platform->boot, _start_module_load, platform);

	/* setup the IDT */
	if(_arch_setidt(_idt, sizeof(_idt) / sizeof(*_idt)) != 0)
	{
		puts("Could not setup the IDT");
		return 2;
	}

	return 0;
}

static int _start_console(ukPlatform * platform)
{
	ukPlatformDevice device;

	if((device.name = boot_get_console(platform->boot)) == NULL)
	{
		errno = ENODEV;
		return -1;
	}
	device.type = UKDT_CONSOLE;
	device.bus = boot_get_console_bus(platform->boot);
	return platform_device_register(platform, &device);
}

static int _start_devices(ukPlatform * platform,
		ukPlatformDevice const * devices, size_t devices_cnt)
{
	int ret = 0;
	size_t i;

	for(i = 0; i < devices_cnt; i++)
		if(devices[i].type == UKDT_BUS)
			continue;
		else if(platform_device_register(platform, &devices[i]) != 0)
			ret = -1;
	return ret;
}

static int _start_display(ukPlatform * platform)
{
	ukPlatformDevice device;

	if((device.name = boot_get_display(platform->boot)) == NULL)
	{
		errno = ENODEV;
		return -1;
	}
	device.type = UKDT_DISPLAY;
	device.bus = boot_get_display_bus(platform->boot);
	return platform_device_register(platform, &device);
}


static void _start_module_load(void * data, const ukBootModule * module)
{
	ukMultibootMod mbm;
	(void) data;

	/* FIXME make it generic */
	memset(&mbm, 0, sizeof(mbm));
	mbm.start = module->start;
	mbm.end = module->end;
	mbm.cmdline = module->args;
	multiboot_load_module(&mbm, NULL, NULL);
}
