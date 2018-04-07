/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */
/* Originally from https://wiki.osdev.org/Bare_Bones */



/* variables */
#define ALIGN		(1 << 0)		/* align the modules loaded on */
						/* page boundaries */
#define MEMINFO		(1 << 1)		/* provide the memory map */
#define FLAGS		ALIGN | MEMINFO		/* multiboot flags */
#define MAGIC		0x1badb002		/* magic number */
#define CHECKSUM	-(MAGIC + FLAGS)	/* checksum for the above */


/* sections */
/* multiboot */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/* bss */
.section .bss
.align 16
stack_bottom:
.skip 16384 /* 16 kB */
stack_top:


/* data */
.section .data
k_ptr:
.long 0x0


/* text */
.section .text
/* exit */
.global _exit
.type _exit, @function
_exit:
	/* infinite loop */
	cli
1:	hlt
	jmp 1b


.global _kernel32
.type _kernel32,function
_kernel32:
	/* jump into the 32-bit kernel */
	mov 0x4(%esp), %ebx
	mov 0x8(%esp), %eax
	call *%eax
	ret


.global _kernel64
.type _kernel64,function
_kernel64:
	/* check for 64-bit capability */
	pushf
	pop %eax
	mov %eax, %ecx
	xor $0x00200000, %eax
	push %eax
	popf
	pushf
	pop %eax
	cmp %ecx, %eax
	jz 1f			/* CPUID is not supported */
	push %ebx
	mov $0x80000001, %eax
	cpuid
	pop %ebx
	cmp $0x80000000, %eax
	jl 1f			/* 64-bit mode is not supported */

	/* jump into the 64-bit kernel */
	mov 0x8(%esp), %esi
	mov %esi, (k_ptr)

	lgdt (_gdt64)

	mov _gdt64_data, %ax
	mov %ax, %ss
	mov %ax, %ds
	mov %ax, %es
	jmp _gdt64_code
.jmp_k:
	mov 0x4(%esp), %edi
	mov k_ptr, %eax
.long 0x0
	jmp *%eax

1:
	mov $-1, %eax
	ret


.global _setgdt
.type _setgdt,function
_setgdt:
	lea gdt_descriptor, %ecx
	/* set the offset of the GDT */
	mov 0x4(%esp), %eax
	mov %eax, 0x2(%ecx)
	/* set the size of the GDT */
	mov 0x8(%esp), %eax
	dec %eax
	mov %ax, (%ecx)
	/* load the GDT */
	lgdt (%ecx)
	ret


/* start */
.global _start
.type _start, @function
_start:
	/* disable interrupts */
	cli

	/* initialize the stack */
	mov $stack_top, %esp

#if defined(__SSP__)
	/* initialize SSP */
	call __stack_chk_setup
#endif

	/* call the global constructors */
	call _init

	/* start the loader */
	push %ebx
	call main
	add $0x4, %esp

	/* exit the kernel */
	call _exit

	hlt

.size _start, . - _start


.section .bss
.align 16
gdt_descriptor:
.skip 2		/* size */
.skip 4		/* offset */
