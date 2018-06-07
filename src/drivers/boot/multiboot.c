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


/* public */
/* functions */
/* multiboot_load_module */
static int _load_module_elf(ukMultibootMod const * mod,
		unsigned char * elfclass, vaddr_t * entrypoint);
static int _load_module_elf32(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf32_Ehdr * ehdr);
static int _load_module_elf32_relocate(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr);
static int _load_module_elf32_relocate_arch(ukMultibootMod const * mod,
		Elf32_Shdr * shdr, Elf32_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf32_Sym * sym);
static int _load_module_elf32_strtab(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		char ** strtab, size_t * strtab_cnt);
static int _load_module_elf32_symtab(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		Elf32_Word type, Elf32_Sym ** symtab, size_t * symtab_cnt);
static int _load_module_elf64(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf64_Ehdr * ehdr);
static int _load_module_elf64_relocate(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr);
static int _load_module_elf64_relocate_arch(ukMultibootMod const * mod,
		Elf64_Shdr * shdr, Elf64_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf64_Sym * sym);
static int _load_module_elf64_strtab(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr, Elf64_Shdr * shdr, Elf64_Word index,
		char ** strtab, size_t * strtab_cnt);
static int _load_module_elf64_symtab(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr, Elf64_Shdr * shdr, Elf64_Word index,
		Elf64_Word type, Elf64_Sym ** symtab, size_t * symtab_cnt);

int multiboot_load_module(ukMultibootMod const * mod, unsigned char * elfclass,
		vaddr_t * entrypoint)
{
	size_t len;
	struct
	{
		int (*loader)(ukMultibootMod const * mod,
				unsigned char * elfclass, vaddr_t * entrypoint);
		char * signature;
		size_t signature_len;
	} loaders[] =
	{
		{ _load_module_elf, ELFMAG, SELFMAG }
	};
	size_t i;

	if(mod->end < mod->start)
	{
		puts("Could not load module: Invalid address");
		return -1;
	}
	if((len = mod->end - mod->start) == 0)
	{
		puts("Could not load module: Empty size");
		return -1;
	}
	for(i = 0; i < sizeof(loaders) / sizeof(*loaders); i++)
	{
		if(len < loaders[i].signature_len)
			continue;
		if(memcmp((void *)mod->start, loaders[i].signature,
					loaders[i].signature_len) == 0)
			return loaders[i].loader(mod, elfclass, entrypoint);
	}
	puts("Could not load module: Unsupported type");
	return -1;
}

static int _load_module_elf(ukMultibootMod const * mod,
		unsigned char * elfclass, vaddr_t * entrypoint)
{
	Elf32_Ehdr * ehdr;

	ehdr = (Elf32_Ehdr *)mod->start;
	if(elfclass != NULL)
		*elfclass = ehdr->e_ident[EI_CLASS];
	switch(ehdr->e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			return _load_module_elf32(mod, entrypoint, ehdr);
		case ELFCLASS64:
			return _load_module_elf64(mod, entrypoint,
					(Elf64_Ehdr *)ehdr);
	}
	puts("Could not load module: Unsupported ELF class");
	return -1;
}

static int _load_module_elf32(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf32_Ehdr * ehdr)
{
	Elf32_Phdr * phdr;
	Elf32_Half i;

	if(mod->start + ehdr->e_phoff + (ehdr->e_phnum * sizeof(*phdr))
			> mod->end)
	{
		puts("Could not load 32-bit module: Invalid format");
		return -1;
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
		if(_load_module_elf32_relocate(mod, ehdr) != 0)
		{
			puts("Could not load 32-bit module:"
					" Could not relocate");
			return -1;
		}
		return 0;
	}
	puts("Could not load 32-bit module: Invalid entrypoint");
	return -1;
}

static int _load_module_elf32_relocate(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr)
{
	Elf32_Half i;
	Elf32_Shdr * shdr;
	Elf32_Word link;
	Elf32_Sym * symtab;
	size_t symtab_cnt;
	char * strtab;
	size_t strtab_cnt;
	Elf32_Rel * rel;
	Elf32_Word j;
	Elf32_Rela rela;
	Elf32_Sym * sym;

	shdr = (Elf32_Shdr *)(mod->start + ehdr->e_shoff);
	for(i = SHN_UNDEF + 1; i < ehdr->e_shnum; i++)
	{
		if((shdr[i].sh_type != SHT_REL
					|| shdr[i].sh_entsize != sizeof(*rel))
				&& (shdr[i].sh_type != SHT_RELA
					|| shdr[i].sh_entsize != sizeof(rela)))
			continue;
		if((link = shdr[i].sh_link) > ehdr->e_shnum)
			return -1;
		if(_load_module_elf32_symtab(mod, ehdr, shdr, link, SHT_DYNSYM,
					&symtab, &symtab_cnt) != 0)
			return -1;
		if(_load_module_elf32_strtab(mod, ehdr, shdr,
					shdr[link].sh_link,
					&strtab, &strtab_cnt) != 0)
			break;
		rel = (Elf32_Rel *)(mod->start + shdr[i].sh_offset);
		for(j = 0; j < shdr[i].sh_size; j += shdr[i].sh_entsize)
		{
			rela.r_addend = 0;
			memcpy(&rela, (char *)rel + j, shdr[i].sh_entsize);
			sym = (ELF32_R_SYM(rela.r_info) < symtab_cnt)
				? &symtab[ELF32_R_SYM(rela.r_info)] : NULL;
			if(_load_module_elf32_relocate_arch(mod,
						shdr[i].sh_info < ehdr->e_shnum
						? &shdr[shdr[i].sh_info] : NULL,
						&rela, strtab, strtab_cnt,
						sym) != 0)
				return -1;
		}
	}
	return 0;
}

static int _load_module_elf32_relocate_arch(ukMultibootMod const * mod,
		Elf32_Shdr * shdr, Elf32_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf32_Sym * sym)
{
#if defined(__i386__)
	Elf32_Addr * addr;

	addr = (Elf32_Addr *)(mod->start + (shdr != NULL ? shdr->sh_offset : 0)
			+ rela->r_offset);
	switch(ELF32_R_TYPE(rela->r_info))
	{
		case R_386_32:		/* S + A */
			*addr += sym->st_value + rela->r_addend;
			break;
		case R_386_GLOB_DAT:	/* S */
		case R_386_JMP_SLOT:	/* S */
			*addr += sym->st_value + rela->r_addend;
			break;
		case R_386_NONE:
			break;
		case R_386_PC32:	/* S + A - P */
			*addr += sym->st_value + rela->r_addend
				- (shdr != NULL ? shdr->sh_offset : 0);
			break;
		case R_386_RELATIVE:	/* B + A */
			*addr += mod->start + rela->r_addend;
			break;
		default:
			printf("%u: Relocation not supported\n",
					ELF32_R_TYPE(rela->r_info));
			return -1;
	}
	return 0;
#else
# warning Unsupported platform: 32-bit relocations not implemented
	return -1;
#endif
}

static int _load_module_elf32_strtab(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		char ** strtab, size_t * strtab_cnt)
{
	if(index >= ehdr->e_shnum || shdr[index].sh_type != SHT_STRTAB)
		return -1;
	shdr = &shdr[index];
	*strtab = (char *)(mod->start + shdr->sh_offset);
	if((*strtab)[shdr->sh_size - 1] != '\0')
		return -1;
	*strtab_cnt = shdr->sh_size;
	return 0;
}

static int _load_module_elf32_symtab(ukMultibootMod const * mod,
		Elf32_Ehdr * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		Elf32_Word type, Elf32_Sym ** symtab, size_t * symtab_cnt)
{
	if(index >= ehdr->e_shnum)
		return -1;
	shdr = &shdr[index];
	if(shdr->sh_type != type || shdr->sh_entsize != sizeof(**symtab))
		return -1;
	*symtab = (Elf32_Sym *)(mod->start + shdr->sh_offset);
	*symtab_cnt = shdr->sh_size / shdr->sh_entsize;
	return 0;
}

static int _load_module_elf64(ukMultibootMod const * mod, vaddr_t * entrypoint,
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
		if(_load_module_elf64_relocate(mod, ehdr) != 0)
		{
			puts("Could not load 64-bit module:"
					" Could not relocate");
			return -1;
		}
		return 0;
	}
	puts("Could not load 64-bit module: Invalid entrypoint");
	return -1;
}

static int _load_module_elf64_relocate(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr)
{
	Elf64_Quarter i;
	Elf64_Shdr * shdr;
	Elf64_Half link;
	Elf64_Sym * symtab;
	size_t symtab_cnt;
	char * strtab;
	size_t strtab_cnt;
	Elf64_Rel * rel;
	Elf32_Word j;
	Elf64_Rela rela;
	Elf64_Sym * sym;

	shdr = (Elf64_Shdr *)(mod->start + ehdr->e_shoff);
	for(i = SHN_UNDEF + 1; i < ehdr->e_shnum; i++)
	{
		if((shdr[i].sh_type != SHT_REL
					|| shdr[i].sh_entsize != sizeof(*rel))
				&& (shdr[i].sh_type != SHT_RELA
					|| shdr[i].sh_entsize != sizeof(rela)))
			continue;
		if((link = shdr[i].sh_link) > ehdr->e_shnum)
			return -1;
		if(_load_module_elf64_symtab(mod, ehdr, shdr, link, SHT_DYNSYM,
					&symtab, &symtab_cnt) != 0)
			return -1;
		if(_load_module_elf64_strtab(mod, ehdr, shdr,
					shdr[link].sh_link,
					&strtab, &strtab_cnt) != 0)
			break;
		rel = (Elf64_Rel *)(mod->start + shdr[i].sh_offset);
		for(j = 0; j < shdr[i].sh_size; j += shdr[i].sh_entsize)
		{
			rela.r_addend = 0;
			memcpy(&rela, (char *)rel + j, shdr[i].sh_entsize);
			sym = (ELF64_R_SYM(rela.r_info) < symtab_cnt)
				? &symtab[ELF64_R_SYM(rela.r_info)] : NULL;
			if(_load_module_elf64_relocate_arch(mod,
						shdr[i].sh_info < ehdr->e_shnum
						? &shdr[shdr[i].sh_info] : NULL,
						&rela, strtab, strtab_cnt,
						sym) != 0)
				return -1;
		}
	}
	return 0;
}

static int _load_module_elf64_relocate_arch(ukMultibootMod const * mod,
		Elf64_Shdr * shdr, Elf64_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf64_Sym * sym)
{
#if defined(__i386__)
	Elf64_Addr * addr;

	addr = (Elf64_Addr *)(mod->start + (shdr != NULL ? shdr->sh_offset : 0)
			+ rela->r_offset);
	switch(ELF64_R_TYPE(rela->r_info))
	{
		case R_X86_64_GLOB_DAT:	/* S */
		case R_X86_64_JUMP_SLOT:/* S */
			*addr = sym->st_value + rela->r_addend;
			break;
		case R_X86_64_NONE:
			break;
		case R_X86_64_RELATIVE:	/* B + A */
			*addr = mod->start + rela->r_addend;
			break;
		default:
			printf("%u: Relocation not supported\n",
					ELF64_R_TYPE(rela->r_info));
			return -1;
	}
	return 0;
#else
# warning Unsupported platform: 64-bit relocations not implemented

	return -1;
#endif
}

static int _load_module_elf64_strtab(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr, Elf64_Shdr * shdr, Elf64_Half index,
		char ** strtab, size_t * strtab_cnt)
{
	if(index >= ehdr->e_shnum || shdr[index].sh_type != SHT_STRTAB)
		return -1;
	shdr = &shdr[index];
	*strtab = (char *)(mod->start + shdr->sh_offset);
	if((*strtab)[shdr->sh_size - 1] != '\0')
		return -1;
	*strtab_cnt = shdr->sh_size;
	return 0;
}

static int _load_module_elf64_symtab(ukMultibootMod const * mod,
		Elf64_Ehdr * ehdr, Elf64_Shdr * shdr, Elf64_Half index,
		Elf64_Half type, Elf64_Sym ** symtab, size_t * symtab_cnt)
{
	if(index >= ehdr->e_shnum)
		return -1;
	shdr = &shdr[index];
	if(shdr->sh_type != type || shdr->sh_entsize != sizeof(**symtab))
		return -1;
	*symtab = (Elf64_Sym *)(mod->start + shdr->sh_offset);
	*symtab_cnt = shdr->sh_size / shdr->sh_entsize;
	return 0;
}
#endif
