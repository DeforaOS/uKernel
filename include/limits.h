/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_LIMITS_H
# define UKERNEL_LIMITS_H


/* constants */
/* numerical */
# ifndef CHAR_BIT
#  define CHAR_BIT 8
# endif
# ifndef CHAR_MAX
#  define CHAR_MAX SCHAR_MAX
# endif
# ifndef CHAR_MIN
#  define CHAR_MIN SCHAR_MIN
# endif
# ifndef SHRT_MAX
#  define SHRT_MAX 0x7fff
# endif
# ifndef SHRT_MIN
#  define SHRT_MIN -0x8000
# endif
# ifndef INT_MAX
#  define INT_MAX 0x7fffffff
# endif
# ifndef INT_MIN
#  define INT_MIN -0x80000000
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
# ifndef SCHAR_MAX
#  define SCHAR_MAX 0x7f
# endif
# ifndef SCHAR_MIN
#  define SCHAR_MIN -0x80
# endif
# ifndef SSIZE_MAX
#  define SSIZE_MAX LONG_MAX
# endif
# ifndef SSIZE_MIN
#  define SSIZE_MIN LONG_MIN
# endif
# ifndef UCHAR_MAX
#  define UCHAR_MAX 0xff
# endif
# ifndef USHRT_MAX
#  define USHRT_MAX 0xffff
# endif
# ifndef UINT_MAX
#  define UINT_MAX 0xffffffff
# endif

#endif /* !UKERNEL_LIMITS_H */
