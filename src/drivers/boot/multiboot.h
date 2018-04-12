/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_BOOT_MULTIBOOT_H
# define UKERNEL_DRIVERS_BOOT_MULTIBOOT_H

# include <sys/types.h>
# include <stdint.h>
# include <elf.h>
# include "multiboot/constants.h"


/* types */
typedef struct _ukBootMultibootHeader ukMultibootHeader;
typedef struct _ukBootMultibootInfo ukMultibootInfo;
typedef struct _ukBootMultibootMod ukMultibootMod;

struct _ukBootMultibootHeader
{
	uint32_t magic;
	uint32_t flags;
	uint32_t checksum;

	paddr_t header_addr;
	paddr_t load_addr;
	paddr_t load_end_addr;
	paddr_t bss_end_addr;
	paddr_t entry_addr;

	uint32_t mode_type;
	uint32_t width;
	uint32_t height;
	uint32_t depth;
};

struct _ukBootMultibootInfo
{
	uint32_t flags;

	uint32_t mem_lower;
	uint32_t mem_upper;

	uint8_t boot_device_part3;
	uint8_t boot_device_part2;
	uint8_t boot_device_part1;
	uint8_t boot_device_drive;

	char const * cmdline;

	uint32_t mods_count;
	ukMultibootMod * mods_addr;

	uint32_t elfshdr_num;
	uint32_t elfshdr_size;
	vaddr_t elfshdr_addr;
	uint32_t elfshdr_shndx;

	uint32_t mmap_length;
	vaddr_t mmap_addr;

	uint32_t drives_length;
	vaddr_t drives_addr;

	void * config_table;

	char const * loader_name;

	void * apm_table;

	void * vbe_control_info;
	void * vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
};

struct _ukBootMultibootMod
{
	uint32_t start;
	uint32_t end;
	char const * cmdline;
	uint32_t _padding;
};


/* prototypes */
int multiboot_boot_kernel32(ukMultibootInfo * info, vaddr_t entrypoint);
int multiboot_boot_kernel64(ukMultibootInfo * info, vaddr_t entrypoint);

int multiboot_load_module(ukMultibootMod * mod, unsigned char * elfclass,
		vaddr_t * entrypoint);

#endif /* !UKERNEL_DRIVERS_BOOT_MULTIBOOT_H */
