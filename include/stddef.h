/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDDEF_H
# define UKERNEL_STDDEF_H


/* types */
# ifndef size_t
#  define size_t size_t
typedef unsigned long size_t;
# endif

/* constants */
# ifndef NULL
#  define NULL ((void *)0)
# endif

#endif /* !UKERNEL_STDDEF_H */
