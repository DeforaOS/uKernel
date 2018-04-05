/* $Id$ */
/* Copyright (c) 2008 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_ERRNO_H
# define UKERNEL_ERRNO_H


/* constants */
# define EBADF		9
# define ENODEV		19
# define EINVAL		22
# define ERANGE		34
# define ENOSYS		78
# define ENOTSUP	86


/* variables */
extern int errno;

#endif /* !UKERNEL_ERRNO_H */
