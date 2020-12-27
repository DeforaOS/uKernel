/* $Id$ */
/* Copyright (c) 2018-2020 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* sections */
/* text */
.section .text
.global intr_disable
#ifndef __APPLE__
.type intr_disable, @function
#endif
intr_disable:
	cli
	ret


/* intr_enable */
.global intr_enable
#ifndef __APPLE__
.type intr_enable, @function
#endif
intr_enable:
	sti
	ret
