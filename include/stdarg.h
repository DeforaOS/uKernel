/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDARG_H
# define UKERNEL_STDARG_H


/* types */
# ifndef va_list
#   define va_list va_list
typedef __builtin_va_list va_list;
# endif


/* macros */
# if defined(__GNUC__) && __GNUC__ >= 3
#  define va_start(ap, arg)	__builtin_va_start((ap), (arg))
#  define va_arg		__builtin_va_arg
#  define va_copy		__builtin_va_copy
#  define va_end(ap)		__builtin_va_end((ap))
# else
/* XXX works only in particular cases */
#  define va_start(ap, arg)	((ap) = ((va_list)&(arg)) \
		+ ((sizeof(long) > sizeof(arg)) ? sizeof(long) : sizeof(arg)))
#  define va_arg(ap, type)	*(type *)(ap), \
	((ap) += ((sizeof(long) > sizeof(type)) ? sizeof(long) : sizeof(type)))
#  define va_copy(copy, arg)	((copy) = (arg))
#  define va_end(ap)
# endif


#endif /* !UKERNEL_STDARG_H */
