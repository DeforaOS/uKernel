/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_KERNEL_PLATFORM_H
# define UKERNEL_KERNEL_PLATFORM_H

# include <kernel/boot.h>
# include <kernel/driver.h>


/* types */
typedef struct _ukPlatform ukPlatform;

typedef struct _ukPlatformDevice
{
	ukDriverType type;
	char const * name;
	char const * bus;
} ukPlatformDevice;


/* variables */
extern ukPlatform * platform;


/* functions */
/* essential */
void platform_init(ukPlatform * platform, ukBoot * boot);

/* accessors */
char const * platform_get_arch(ukPlatform * platform);

void platform_get_physmem(ukPlatform * platform, paddr_t * low, paddr_t * high);

/* useful */
int platform_start(ukPlatform * platform);

/* devices */
void * platform_device_lookup(ukPlatform * platform, ukDriverType type,
		char const * name);
int platform_device_register(ukPlatform * platform,
		const ukPlatformDevice * device);

#endif /* !UKERNEL_KERNEL_PLATFORM_H */
