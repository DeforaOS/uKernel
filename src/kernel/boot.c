/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <errno.h>
#include <kernel/boot.h>
#include "config.h"


/* ukBoot */
/* private */
/* types */
struct _ukBoot
{
	char const * name;
	char const * args;

	/* memory */
	uintptr_t physmem_low;
	uintptr_t physmem_high;

	/* drivers */
	char const * console;
	char const * console_bus;

	char const * display;
	char const * display_bus;

	/* modules */
	ukBootModule modules[CONFIG_BOOT_MODULES];
	size_t modules_cnt;
};


/* variables */
static ukBoot _boot;


/* public */
/* variables */
ukBoot * boot = &_boot;


/* functions */
/* essential */
void boot_init(ukBoot * boot, char const * name, char const * args,
		uintptr_t physmem_low, uintptr_t physmem_high)
{
	boot->name = name;
	boot->args = args;
	boot_set_physmem(boot, physmem_low, physmem_high);
}


/* accessors */
/* boot_get_console */
char const * boot_get_console(ukBoot * boot)
{
	return boot->console;
}


/* boot_get_console_bus */
char const * boot_get_console_bus(ukBoot * boot)
{
	return boot->console_bus;
}


/* boot_get_display */
char const * boot_get_display(ukBoot * boot)
{
	return boot->display;
}


/* boot_get_display_bus */
char const * boot_get_display_bus(ukBoot * boot)
{
	return boot->display_bus;
}


/* boot_set_console */
void boot_set_console(ukBoot * boot, char const * console, char const * bus)
{
	boot->console = console;
	boot->console_bus = bus;
}


/* boot_set_display */
void boot_set_display(ukBoot * boot, char const * display, char const * bus)
{
	boot->display = display;
	boot->display_bus = bus;
}


/* boot_get_physmem */
void boot_get_physmem(ukBoot * boot, uintptr_t * low, uintptr_t * high)
{
	if(low != NULL)
		*low = boot->physmem_low;
	if(high != NULL)
		*high = boot->physmem_high;
}


/* boot_set_physmem */
void boot_set_physmem(ukBoot * boot, uintptr_t low, uintptr_t high)
{
	boot->physmem_low = low;
	boot->physmem_high = high;
}


/* useful */
/* boot_module_foreach */
void boot_module_foreach(ukBoot * boot,
		void (*callback)(void *, const ukBootModule *), void * data)
{
	size_t i;

	for(i = 0; i < boot->modules_cnt; i++)
		callback(data, &boot->modules[i]);
}


/* boot_module_register */
int boot_module_register(ukBoot * boot, char const * name, char const * args,
		uintptr_t start, uintptr_t end)
{
	ukBootModule * module;

	if(boot->modules_cnt >= sizeof(boot->modules) / sizeof(*boot->modules))
	{
		errno = ENOMEM;
		return -1;
	}
	if(start < boot->physmem_low || end > boot->physmem_high)
	{
		errno = ERANGE;
		return -1;
	}
	module = &boot->modules[boot->modules_cnt];
	module->name = name;
	module->args = args;
	module->start = start;
	module->end = end;
	return 0;
}
