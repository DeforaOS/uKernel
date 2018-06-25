/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_TIME_H
# define UKERNEL_TIME_H


/* types */
# ifndef time_t
#  define time_t time_t
typedef long long time_t;
# endif


/* functions */
time_t time(time_t * t);

#endif /* !UKERNEL_TIME_H */
