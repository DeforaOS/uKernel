/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_UNISTD_H
# define UKERNEL_UNISTD_H

# include <stddef.h>
# include <stdint.h>


/* constants */
/* file streams */
# define STDIN_FILENO	0
# define STDOUT_FILENO	1
# define STDERR_FILENO	2


/* prototypes */
void _exit(int status);

#endif /* !UKERNEL_UNISTD_H */
