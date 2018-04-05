/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



.text


/* _start */
.global _start
.type _start, @function


/* check for supported architectures */
#if defined(__amd64__)
# include "arch/amd64/start.S"
# include "arch/start.S"
#elif defined(__i386__)
# include "arch/i386/start.S"
# include "arch/start.S"
#else
# warning Unsupported architecture
#endif
