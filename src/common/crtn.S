/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* check for supported architectures */
#if defined(__amd64__)
# include "arch/amd64/crtn.S"
#elif defined(__i386__)
# include "arch/i386/crtn.S"
#else
# warning Unsupported architecture
#endif
