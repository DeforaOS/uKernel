/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_FCNTL_H
# define UKERNEL_FCNTL_H


/* types */
struct flock
{
	off_t l_start;
	off_t l_len;
	pid_t l_pid;
	short l_type;
	short l_whence;
};


/* constants */
# define F_DUPFD	0
# define F_GETFD	1
# define F_SETFD	2
# define F_GETFL	3
# define F_SETFL	4
# define F_GETOWN	5
# define F_SETOWN	6
# define F_GETLK	7
# define F_SETLK	8
# define F_SETLKW	9

# define FD_CLOEXEC	1

# define F_RDLCK	1
# define F_UNLCK	2
# define F_WRLCK	3

# define O_CREAT	0x0200
# define O_TRUNC	0x0400
# define O_EXCL		0x0800
# define O_NOCTTY	0x8000

# define O_NONBLOCK	0x00004
# define O_APPEND	0x00008
# define O_SYNC		0x00080
# define O_DSYNC	0x10000
# define O_RSYNC	0x20000

# define O_RDONLY	0
# define O_WRONLY	1
# define O_RDWR		2

# define O_ACCMODE	(O_WRONLY | O_RDWR)

# define LOCK_SH	0x0
# define LOCK_EX	0x2
# define LOCK_NB	0x4
# define LOCK_UN	0x8


/* functions */
int creat(const char * filename, mode_t mode);
int faccessat(int fd, char const * path, int mode, int flags);
int fchownat(int fd, char const * path, uid_t uid, gid_t gid, int flags);
int fcntl(int fd, int cmd, ...);
int flock(int fd, int operation);
int open(char const * filename, int flags, ...);
int openat(int fd, char const * filename, int flags, ...);

#endif /* !UKERNEL_FCNTL_H */
