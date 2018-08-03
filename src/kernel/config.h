/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_KERNEL_CONFIG_H
# define UKERNEL_KERNEL_CONFIG_H


/* constants */
# ifndef CONFIG_BOOT_MODULES
#  define CONFIG_BOOT_MODULES		32
# endif
# ifndef CONFIG_DEFAULT_CONSOLE
#  define CONFIG_DEFAULT_CONSOLE	"uart"
# endif
# ifndef CONFIG_DEFAULT_CONSOLE_BUS
#  define CONFIG_DEFAULT_CONSOLE_BUS	"ioport"
# endif
# ifndef CONFIG_DEFAULT_DISPLAY
#  define CONFIG_DEFAULT_DISPLAY	"vga"
# endif
# ifndef CONFIG_DEFAULT_DISPLAY_BUS
#  define CONFIG_DEFAULT_DISPLAY_BUS	"vga"
# endif

#endif /* !UKERNEL_KERNEL_CONFIG_H */
