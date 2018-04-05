/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDINT_H
# define UKERNEL_STDINT_H


/* types */
# ifdef __INT8_TYPE__
typedef __INT8_TYPE__ int8_t;
# else
typedef char int8_t;
# endif
# ifdef __INT16_TYPE__
typedef __INT16_TYPE__ int16_t;
# else
typedef short int int16_t;
# endif
# ifdef __INT32_TYPE__
typedef __INT32_TYPE__ int32_t;
# else
typedef int int32_t;
# endif
# ifdef __INT64_TYPE__
typedef __INT64_TYPE__ int64_t;
# else
typedef long long int64_t;
# endif
# ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__ uint8_t;
# else
typedef unsigned char uint8_t;
# endif
# ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__ uint16_t;
# else
typedef unsigned short uint16_t;
# endif
# ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__ uint32_t;
# else
typedef unsigned int uint32_t;
# endif
# ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__ uint64_t;
# else
typedef unsigned long long uint64_t;
# endif

typedef unsigned long size_t;

# ifndef SIZE_MAX
#  define SIZE_MAX	ULONG_MAX
# endif

#endif /* !UKERNEL_STDINT_H */
