/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_KERNEL_BOOT_H
# define UKERNEL_KERNEL_BOOT_H

# include <sys/types.h>


/* types */
typedef struct _ukBoot ukBoot;

typedef struct _ukBootModule
{
	char const * name;
	char const * args;
	paddr_t start;
	paddr_t end;
} ukBootModule;


/* variables */
extern ukBoot * boot;


/* functions */
/* essential */
void boot_init(ukBoot * boot, char const * name, char const * args,
		paddr_t physmem_low, paddr_t physmem_high);

/* accessors */
char const * boot_get_arch(ukBoot * boot);
char const * boot_get_name(ukBoot * boot);

char const * boot_get_console(ukBoot * boot);
char const * boot_get_console_bus(ukBoot * boot);
void boot_set_console(ukBoot * boot, char const * driver, char const * bus);

char const * boot_get_display(ukBoot * boot);
char const * boot_get_display_bus(ukBoot * boot);
void boot_set_display(ukBoot * boot, char const * driver, char const * bus);

void boot_get_physmem(ukBoot * boot, paddr_t * low, paddr_t * high);
void boot_set_physmem(ukBoot * boot, paddr_t low, paddr_t high);

/* modules */
void boot_module_foreach(ukBoot * boot,
		void (*callback)(void *, const ukBootModule *), void * data);
int boot_module_register(ukBoot * boot, char const * name, char const * args,
		paddr_t start, paddr_t end);

#endif /* !UKERNEL_KERNEL_BOOT_H */
