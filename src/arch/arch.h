/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_ARCH_H
# define UKERNEL_ARCH_H

# if defined(__amd64__)
#  include "arch/amd64/arch.h"
# elif defined(__i386__)
#  include "arch/i386/arch.h"
# endif

#endif /* !UKERNEL_ARCH_H */
