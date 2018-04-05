/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_SYS_TYPES_H
# define UKERNEL_SYS_TYPES_H


/* types */
# ifndef paddr_t
#  define paddr_t paddr_t
typedef unsigned long paddr_t;
# endif
# ifndef size_t
#  define size_t size_t
typedef unsigned long size_t;
# endif
# ifndef ssize_t
#  define ssize_t ssize_t
typedef long ssize_t;
# endif
# ifndef vaddr_t
#  define vaddr_t vaddr_t
typedef unsigned long vaddr_t;
# endif

#endif /* !UKERNEL_SYS_TYPES_H */
