/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#if defined(__ELF__)
.global __start_dlfcn
# ifndef __clang__
.type __start_dlfcn, @function
# endif
__start_dlfcn:
	ret
#endif


#include "lib/libc/src/start.S"
