/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_KERNEL_PLATFORM_H
# define UKERNEL_KERNEL_PLATFORM_H


/* functions */
void platform_init(void);


/* accessors */
void platform_set_console(char const * bus, char const * name);
void platform_set_display(char const * bus, char const * name);

#endif /* !UKERNEL_KERNEL_PLATFORM_H */
