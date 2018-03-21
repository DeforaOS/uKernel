/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_STDINT_H
# define UKERNEL_STDINT_H


/* types */
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

typedef unsigned long size_t;

#endif /* !UKERNEL_STDINT_H */
