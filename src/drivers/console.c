/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include "console.h"


#if defined(__amd64__) || defined(__i386__)
# include "console/vga.c"
#endif
