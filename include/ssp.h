/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */


#ifndef UKERNEL_SSP_H
# define UKERNEL_SSP_H


/* public */
/* prototypes */
# if defined(__SSP__)
void __stack_chk_fail(void);
void __stack_chk_setup(void);
# endif

#endif /* !UKERNEL_SSP_H */
