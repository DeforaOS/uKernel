/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_LIMITS_H
# define UKERNEL_LIMITS_H


/* constants */
/* numerical */
# ifndef INT_MAX
#  define INT_MAX 0x7fffffff
# endif
# ifndef INT_MIN
#  define INT_MIN -0x7fffffff
# endif
# ifndef LONG_MAX
#  ifdef _LP64 /* FIXME probably sometimes wrong */
#   define LONG_MAX 0x7fffffffffffffff
#  else
#   define LONG_MAX 0x7fffffff
#  endif
# endif
# ifndef LONG_MIN
#  ifdef _LP64 /* FIXME probably sometimes wrong */
#   define LONG_MIN 0x8000000000000000
#  else
#   define LONG_MIN -0x80000000
#  endif
# endif

#endif /* !UKERNEL_LIMITS_H */
