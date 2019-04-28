/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_UNISTD_H
# define UKERNEL_UNISTD_H


/* types */
# ifndef intptr_t
#  define intptr_t intptr_t
typedef long intptr_t;
# endif
# ifndef off_t
#  define off_t off_t
typedef long long off_t;
# endif
# ifndef pid_t
#  define pid_t pid_t
typedef int pid_t;
# endif
# ifndef size_t
#  define size_t size_t
typedef unsigned long size_t;
# endif
# ifndef ssize_t
#  define ssize_t ssize_t
typedef long ssize_t;
# endif


/* constants */
/* file streams */
# define STDIN_FILENO	0
# define STDOUT_FILENO	1
# define STDERR_FILENO	2


/* prototypes */
int brk(void * addr);
void _exit(int status);
pid_t getpid(void);
pid_t getppid(void);
ssize_t read(int fildes, void * buf, size_t count);
void * sbrk(intptr_t increment);
ssize_t write(int fildes, const void * buf, size_t count);

#endif /* !UKERNEL_UNISTD_H */
