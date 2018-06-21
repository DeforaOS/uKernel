/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_SYS_MMAN_H
# define UKERNEL_SYS_MMAN_H


/* types */
# ifndef mode_t
#  define mode_t mode_t
typedef unsigned int mode_t;
# endif
# ifndef off_t
#  define off_t off_t
typedef long long off_t;
# endif
# ifndef size_t
#  define size_t size_t
typedef unsigned long size_t;
# endif


/* constants */
# define MAP_FAILED	((void *)-1)

# define MAP_SHARED	0x0001
# define MAP_PRIVATE	0x0002
# define MAP_FIXED	0x0010
# define MAP_ANONYMOUS	0x1000

# define PROT_NONE	0x0
# define PROT_READ	0x1
# define PROT_WRITE	0x2
# define PROT_EXEC	0x4


/* functions */
int mlock(const void * addr, size_t length);
void * mmap(void * addr, size_t length, int prot, int flags, int fd,
		off_t offset);
int mprotect(void * addr, size_t length, int prot);
int munlock(const void * addr, size_t length);
int munmap(void * addr, size_t length);

#endif /* !UKERNEL_SYS_MMAN_H */
