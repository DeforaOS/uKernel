/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__amd64__) || defined(__i386__)
# include <unistd.h>
# include <sys/mman.h>
# include <stdint.h>
# include <stdio.h>
# include <string.h>
# include <elf.h>
# include "arch/amd64/gdt.h"
# include "arch/i386/gdt.h"
# include "drivers/boot/multiboot.h"

# ifndef MAX
#  define MAX(a, b) (a) > (b) ? (a) : (b)
# endif


/* public */
/* functions */
/* multiboot_load_module */
static int _load_module_elf(ukMultibootMod const * mod,
		unsigned char * elfclass, vaddr_t * entrypoint);
static int _load_module_elf32(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf32_Ehdr const * ehdr);
static int _load_module_elf32_allocate(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr);
static int _load_module_elf32_allocate_nobits(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr,
		Elf32_Shdr * shdr, Elf32_Half i);
static int _load_module_elf32_relocate(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr);
static int _load_module_elf32_relocate_rela(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr,
		Elf32_Shdr * shdr, Elf32_Half i);
static int _load_module_elf32_relocate_arch(ukMultibootMod const * mod,
		Elf32_Phdr const * phdr, Elf32_Shdr * shdr, Elf32_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf32_Sym * sym);
static int _load_module_elf32_strtab(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		char ** strtab, size_t * strtab_cnt);
static int _load_module_elf32_symtab(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		Elf32_Word type, Elf32_Sym ** symtab, size_t * symtab_cnt);
static int _load_module_elf64(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf64_Ehdr const * ehdr);
static int _load_module_elf64_relocate(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr);
static int _load_module_elf64_relocate_arch(ukMultibootMod const * mod,
		Elf64_Shdr * shdr, Elf64_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf64_Sym * sym);
static int _load_module_elf64_strtab(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr, Elf64_Shdr * shdr, Elf64_Word index,
		char ** strtab, size_t * strtab_cnt);
static int _load_module_elf64_symtab(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr, Elf64_Shdr * shdr, Elf64_Word index,
		Elf64_Word type, Elf64_Sym ** symtab, size_t * symtab_cnt);

int multiboot_load_module(ukMultibootMod const * mod, unsigned char * elfclass,
		vaddr_t * entrypoint)
{
	size_t len;
	struct
	{
		int (*loader)(ukMultibootMod const * mod,
				unsigned char * elfclass, vaddr_t * entrypoint);
		off_t offset;
		char * signature;
		size_t signature_len;
	} loaders[] =
	{
		{ _load_module_elf, 0x0, ELFMAG, SELFMAG }
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
		if(memcmp((void *)(mod->start + loaders[i].offset),
					loaders[i].signature,
					loaders[i].signature_len) == 0)
			return loaders[i].loader(mod, elfclass, entrypoint);
	}
	puts("Could not load module: Unsupported type");
	return -1;
}

static int _load_module_elf(ukMultibootMod const * mod,
		unsigned char * elfclass, vaddr_t * entrypoint)
{
	Elf32_Ehdr const * ehdr;

	ehdr = (Elf32_Ehdr const *)mod->start;
	if(ehdr->e_ident[EI_VERSION] != EV_CURRENT)
	{
		puts("Could not load module: Unsupported ELF version");
		return -1;
	}
	if(elfclass != NULL)
		*elfclass = ehdr->e_ident[EI_CLASS];
	switch(ehdr->e_ident[EI_CLASS])
	{
		case ELFCLASS32:
			return _load_module_elf32(mod, entrypoint, ehdr);
		case ELFCLASS64:
			return _load_module_elf64(mod, entrypoint,
					(Elf64_Ehdr const *)ehdr);
	}
	puts("Could not load module: Unsupported ELF class");
	return -1;
}

static int _load_module_elf32(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf32_Ehdr const * ehdr)
{
	Elf32_Phdr const * phdr;
	Elf32_Half i;

	if(mod->start + ehdr->e_phoff + (ehdr->e_phnum * sizeof(*phdr))
			> mod->end)
	{
		puts("Could not load ELF 32-bit module: Invalid format");
		return -1;
	}
	phdr = (Elf32_Phdr const *)(mod->start + ehdr->e_phoff);
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(phdr[i].p_type != PT_LOAD)
			continue;
		if(phdr[i].p_filesz > phdr[i].p_memsz)
		{
			puts("Could not load ELF 32-bit module:"
					" Invalid program header");
			return -1;
		}
		if(phdr[i].p_vaddr > ehdr->e_entry
				|| phdr[i].p_vaddr + phdr[i].p_filesz
				<= ehdr->e_entry)
			continue;
		if(entrypoint != NULL)
			*entrypoint = mod->start + ehdr->e_entry
				- phdr[i].p_vaddr + phdr[i].p_offset;
		if(_load_module_elf32_allocate(mod, ehdr, &phdr[i]) != 0)
		{
			puts("Could not load ELF 32-bit module:"
					" Could not allocate memory");
			return -1;
		}
		if(_load_module_elf32_relocate(mod, ehdr, &phdr[i]) != 0)
		{
			puts("Could not load ELF 32-bit module:"
					" Could not relocate");
			return -1;
		}
		return 0;
	}
	puts("Could not load ELF 32-bit module: Invalid entrypoint");
	return -1;
}

static int _load_module_elf32_allocate(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr)
{
	Elf32_Half i;
	Elf32_Shdr * shdr;

	shdr = (Elf32_Shdr *)(mod->start + ehdr->e_shoff);
	for(i = SHN_UNDEF + 1; i < ehdr->e_shnum; i++)
	{
		if(shdr[i].sh_type == SHT_NOBITS
				&& shdr[i].sh_size != 0
				&& shdr[i].sh_flags & SHF_ALLOC)
			if(_load_module_elf32_allocate_nobits(mod, ehdr, phdr,
						shdr, i) != 0)
				return -1;
	}
	return 0;
}

static int _load_module_elf32_allocate_nobits(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr,
		Elf32_Shdr * shdr, Elf32_Half i)
{
	void * addr;
	(void) mod;
	(void) ehdr;

	if((addr = mmap(0x0, shdr[i].sh_size, PROT_READ | PROT_WRITE,
					MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return -1;
	memset(addr, 0, shdr[i].sh_size);
	if((shdr[i].sh_flags & SHF_WRITE) != SHF_WRITE
			&& mprotect(addr, shdr[i].sh_size, PROT_READ) != 0)
		return -1;
	shdr[i].sh_offset = (Elf32_Off)(addr - phdr[i].p_offset);
	return 0;
}

static int _load_module_elf32_relocate(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr)
{
	Elf32_Half i;
	Elf32_Shdr * shdr;

	shdr = (Elf32_Shdr *)(mod->start + ehdr->e_shoff);
	for(i = SHN_UNDEF + 1; i < ehdr->e_shnum; i++)
	{
		if((shdr[i].sh_type == SHT_REL
					&& shdr[i].sh_entsize
					== sizeof(Elf32_Rel))
				|| (shdr[i].sh_type == SHT_RELA
					&& shdr[i].sh_entsize
					== sizeof(Elf32_Rela)))
			if(_load_module_elf32_relocate_rela(mod, ehdr, phdr,
						shdr, i) != 0)
				return -1;
	}
	return 0;
}

static int _load_module_elf32_relocate_rela(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Phdr const * phdr,
		Elf32_Shdr * shdr, Elf32_Half i)
{
	Elf32_Word link;
	Elf32_Sym * symtab;
	size_t symtab_cnt;
	char * strtab;
	size_t strtab_cnt;
	Elf32_Rel * rel;
	Elf32_Word j;
	Elf32_Rela rela;
	Elf32_Sym * sym;

	if((link = shdr[i].sh_link) > ehdr->e_shnum)
		return -1;
	if(_load_module_elf32_symtab(mod, ehdr, shdr, link, SHT_DYNSYM, &symtab,
				&symtab_cnt) != 0)
		return -1;
	if(_load_module_elf32_strtab(mod, ehdr, shdr, shdr[link].sh_link,
				&strtab, &strtab_cnt) != 0)
		return 0;
	rel = (Elf32_Rel *)(mod->start + shdr[i].sh_offset);
	for(j = 0; j < shdr[i].sh_size; j += shdr[i].sh_entsize)
	{
		rela.r_addend = 0;
		memcpy(&rela, (char *)rel + j, shdr[i].sh_entsize);
		sym = (ELF32_R_SYM(rela.r_info) < symtab_cnt)
			? &symtab[ELF32_R_SYM(rela.r_info)] : NULL;
		if(_load_module_elf32_relocate_arch(mod, phdr,
					shdr[i].sh_info < ehdr->e_shnum
					? &shdr[shdr[i].sh_info] : NULL,
					&rela, strtab, strtab_cnt, sym) != 0)
			return -1;
	}
	return 0;
}

static int _load_module_elf32_relocate_arch(ukMultibootMod const * mod,
		Elf32_Phdr const * phdr, Elf32_Shdr * shdr, Elf32_Rela * rela,
		char const * strtab, size_t strtab_cnt, Elf32_Sym * sym)
{
#if defined(__i386__)
	Elf32_Addr * addr;

	addr = (Elf32_Addr *)(mod->start + (shdr != NULL ? shdr->sh_offset : 0)
			+ rela->r_offset - phdr->p_vaddr);
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
# warning Unsupported platform: ELF 32-bit relocations not implemented
	return -1;
#endif
}

static int _load_module_elf32_strtab(ukMultibootMod const * mod,
		Elf32_Ehdr const * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
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
		Elf32_Ehdr const * ehdr, Elf32_Shdr * shdr, Elf32_Word index,
		Elf32_Word type, Elf32_Sym ** symtab, size_t * symtab_cnt)
{
	if(index >= ehdr->e_shnum)
		return -1;
	shdr = &shdr[index];
	if(shdr->sh_type != type || shdr->sh_entsize != sizeof(**symtab))
		return -1;
	*symtab = (Elf32_Sym *)(mod->start + shdr->sh_offset);
#if 0 /* XXX avoid a division */
	*symtab_cnt = shdr->sh_size / sizeof(**symtab);
#else
	*symtab_cnt = shdr->sh_size >> 4;
#endif
	return 0;
}

static int _load_module_elf64(ukMultibootMod const * mod, vaddr_t * entrypoint,
		Elf64_Ehdr const * ehdr)
{
	Elf64_Phdr const * phdr;
	Elf64_Quarter i;

	if(mod->start + ehdr->e_phoff + (ehdr->e_phnum * sizeof(*phdr))
			> mod->end)
	{
		puts("Could not load ELF 64-bit module: Invalid format");
		return -1;
	}
	phdr = (Elf64_Phdr const *)(uintptr_t)(mod->start + ehdr->e_phoff);
	for(i = 0; i < ehdr->e_phnum; i++)
	{
		if(phdr[i].p_type != PT_LOAD)
			continue;
		if(phdr[i].p_filesz > phdr[i].p_memsz)
		{
			puts("Could not load ELF 64-bit module:"
					" Invalid program header");
			return -1;
		}
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
			puts("Could not load ELF 64-bit module:"
					" Could not relocate");
			return -1;
		}
		return 0;
	}
	puts("Could not load ELF 64-bit module: Invalid entrypoint");
	return -1;
}

static int _load_module_elf64_relocate(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr)
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

	shdr = (Elf64_Shdr *)(uintptr_t)(mod->start + ehdr->e_shoff);
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
		rel = (Elf64_Rel *)(uintptr_t)(mod->start + shdr[i].sh_offset);
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

	addr = (Elf64_Addr *)(uintptr_t)(mod->start
			+ (shdr != NULL ? shdr->sh_offset : 0)
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
# warning Unsupported platform: ELF 64-bit relocations not implemented

	return -1;
#endif
}

static int _load_module_elf64_strtab(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr, Elf64_Shdr * shdr, Elf64_Half index,
		char ** strtab, size_t * strtab_cnt)
{
	if(index >= ehdr->e_shnum || shdr[index].sh_type != SHT_STRTAB)
		return -1;
	shdr = &shdr[index];
	*strtab = (char *)(uintptr_t)(mod->start + shdr->sh_offset);
	if((*strtab)[shdr->sh_size - 1] != '\0')
		return -1;
	*strtab_cnt = shdr->sh_size;
	return 0;
}

static int _load_module_elf64_symtab(ukMultibootMod const * mod,
		Elf64_Ehdr const * ehdr, Elf64_Shdr * shdr, Elf64_Half index,
		Elf64_Half type, Elf64_Sym ** symtab, size_t * symtab_cnt)
{
	if(index >= ehdr->e_shnum)
		return -1;
	shdr = &shdr[index];
	if(shdr->sh_type != type || shdr->sh_entsize != sizeof(**symtab))
		return -1;
	*symtab = (Elf64_Sym *)(uintptr_t)(mod->start + shdr->sh_offset);
#if 0 /* XXX avoid a division */
	*symtab_cnt = shdr->sh_size / sizeof(**symtab);
#else
	*symtab_cnt = shdr->sh_size >> 5;
#endif
	return 0;
}
#endif


/* multiboot_heap_reset */
void multiboot_heap_reset(ukMultibootInfo const * info)
{
	void * heap;
	uint32_t i;

	if(info->flags & BOOT_MULTIBOOT_INFO_HAS_MODS)
	{
		heap = sbrk(0);
		for(i = 0; i < info->mods_count; i++)
			heap = MAX(heap, (void *)info->mods_addr[i].end);
		sbrk(heap - sbrk(0));
	}
}
