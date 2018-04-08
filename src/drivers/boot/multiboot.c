/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <elf.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"
# include "drivers/boot/multiboot.h"


/* private */
/* prototypes */
extern int _kernel32(ukMultibootInfo * mi, vaddr_t entrypoint);
extern int _kernel64(ukMultibootInfo * mi, vaddr_t entrypoint);


/* public */
/* functions */
/* multiboot_load_module */
static int _load_module32(ukMultibootMod * mod, vaddr_t * entrypoint,
		Elf32_Ehdr * ehdr);
static int _load_module64(ukMultibootMod * mod, vaddr_t * entrypoint,
		Elf64_Ehdr * ehdr);

int multiboot_load_module(ukMultibootMod * mod, unsigned char * elfclass,
		vaddr_t * entrypoint)
{
	Elf32_Ehdr * ehdr;

	if(mod->end < mod->start || mod->end - mod->start < sizeof(ehdr))
	{
		puts("Could not load module: Invalid format");
		return -1;
	}
	ehdr = (Elf32_Ehdr *)mod->start;
	if(elfclass != NULL)
		*elfclass = ehdr->e_ident[EI_CLASS];
	switch(ehdr->e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			return _load_module32(mod, entrypoint, ehdr);
		case ELFCLASS64:
			return _load_module64(mod, entrypoint,
					(Elf64_Ehdr *)ehdr);
	}
	return -1;
}

static int _load_module32(ukMultibootMod * mod, vaddr_t * entrypoint,
		Elf32_Ehdr * ehdr)
{
	Elf32_Phdr * phdr;
	Elf32_Half i;

	if(mod->start + ehdr->e_phoff + (ehdr->e_phnum * sizeof(*phdr))
			> mod->end)
	{
		puts("Could not load 32-bit module: Invalid format");
		return NULL;
	}
	phdr = (Elf32_Phdr *)(mod->start + ehdr->e_phoff);
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(phdr[i].p_type != PT_LOAD)
			continue;
		if(phdr[i].p_vaddr > ehdr->e_entry
				|| phdr[i].p_vaddr + phdr[i].p_filesz
				<= ehdr->e_entry)
			continue;
		if(entrypoint != NULL)
			*entrypoint = mod->start + ehdr->e_entry
				- phdr[i].p_vaddr + phdr[i].p_offset;
		/* FIXME really look for main() directly? */
		return 0;
	}
	puts("Could not load 32-bit module: Invalid entrypoint");
	return -1;
}

static int _load_module64(ukMultibootMod * mod, vaddr_t * entrypoint,
		Elf64_Ehdr * ehdr)
{
	Elf64_Phdr * phdr;
	Elf64_Quarter i;

	if(mod->start + ehdr->e_phoff + (ehdr->e_phnum * sizeof(*phdr))
			> mod->end)
	{
		puts("Could not load 64-bit module: Invalid format");
		return -1;
	}
	phdr = (Elf64_Phdr *)(mod->start + ehdr->e_phoff);
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(phdr[i].p_type != PT_LOAD)
			continue;
		if(phdr[i].p_vaddr > ehdr->e_entry
				|| phdr[i].p_vaddr + phdr[i].p_filesz
				<= ehdr->e_entry)
			continue;
		if(entrypoint != NULL)
			*entrypoint = mod->start + ehdr->e_entry
				- phdr[i].p_vaddr + phdr[i].p_offset;
		/* FIXME really look for main() directly? */
		return 0;
	}
	puts("Could not load 64-bit module: Invalid entrypoint");
	return -1;
}
#endif
