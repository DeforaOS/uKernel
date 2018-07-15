/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



/* sections */
/* text */
.section .text
.global intr_disable
.type intr_disable, @function
intr_disable:
	cli
	ret


/* intr_enable */
.global intr_enable
.type intr_enable, @function
intr_enable:
	sti
	ret
