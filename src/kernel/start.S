/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* check for supported architectures */
#if defined(__amd64__)
# include "arch/amd64/gdt.S"
# include "arch/amd64/idt.S"
# include "arch/i386/intr.S"
# include "arch/amd64/kernel.S"
#elif defined(__i386__)
# include "arch/i386/gdt.S"
# include "arch/i386/idt.S"
# include "arch/i386/intr.S"
# include "arch/i386/kernel.S"
#else
# warning Unsupported architecture
#endif
