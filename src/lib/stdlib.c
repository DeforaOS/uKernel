/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#ifndef min
# define min(a, b) (((a) > (b)) ? (b) : (a))
#endif


/* private */
/* types */
typedef struct _Alloc
{
	size_t size;
	struct _Alloc * prev;
	struct _Alloc * next;
} Alloc;


/* variables */
static Alloc _alloc = { 0, NULL, NULL };


/* public */
/* functions */
/* abort */
void abort(void)
{
	_exit(125);
}


/* abs */
int abs(int x)
{
	return (x >= 0) ? x : -x;
}


/* arc4random */
uint32_t arc4random(void)
{
	/* FIXME really implement */
	return 0x41414141;
}


/* calloc */
void * calloc(size_t nmemb, size_t size)
{
	void * ptr;
	size_t sz;

	if(nmemb != 0 && size != 0 && SIZE_MAX / nmemb < size)
	{
		errno = ENOMEM;
		return NULL;
	}
	sz = nmemb * size;
	if((ptr = malloc(sz)) == NULL)
		return NULL;
	memset(ptr, 0, sz);
	return ptr;
}


/* exit */
void exit(int status)
{
	_exit(status);
}


/* free */
void free(void * ptr)
{
	const char buf[] = "invalid free detected: terminated\n";
	Alloc * a = (Alloc*)((char*)ptr - sizeof(*a));
	Alloc * b;

	if(ptr == NULL)
		return;
	b = a->prev;
	if(b->next != a)
	{
		write(2, buf, sizeof(buf) - 1);
		abort();
		return;
	}
	b->next = a->next;
	if(a->next != NULL) /* return if memory is alloc'd past a */
	{
		a->next->prev = b;
		return;
	}
	sbrk(-(a->size + sizeof(*a)));
}


/* labs */
long labs(long x)
{
	return (x >= 0) ? x : -x;
}


/* llabs */
long long llabs(long long x)
{
	return (x >= 0) ? x : -x;
}


/* malloc */
void * malloc(size_t size)
{
	Alloc * a = &_alloc;
	Alloc * b = NULL;
	intptr_t inc;

	if(size >= LONG_MAX - sizeof(*b) - 0x8)
	{
		errno = ENOMEM;
		return NULL;
	}
	size = (size | 0x7) + 1; /* round up to 64 bits */
	inc = size + sizeof(*b);
	if(_alloc.next != NULL) /* look for available space */
		for(a = _alloc.next; a->next != NULL; a = a->next)
			if(inc <= (intptr_t)(a->next) - (intptr_t)a
					- (intptr_t)sizeof(*a)
					- (intptr_t)a->size)
			{
				b = (Alloc*)((char*)a + sizeof(*a) + a->size);
				b->size = size;
				a->next->prev = b;
				break;
			}
	if(b == NULL) /* increase process size to allocate memory */
	{
		if((b = sbrk(inc)) == (void *)-1)
			return NULL;
		b->size = size;
	}
	b->prev = a;
	b->next = a->next;
	a->next = b;
	return (char *)b + sizeof(*b);
}


/* realloc */
void * realloc(void * ptr, size_t size)
{
	Alloc * a = (Alloc*)((char*)ptr - sizeof(*a));
	void * p;

	if(ptr == NULL)
		return malloc(size);
	if(size == a->size)
		return ptr;
	size = (size | 0xf) + 1; /* round up to 64 bits */
	if(size < a->size || (a->next != NULL && (char*)a->next - (char*)a
				- sizeof(*a) >= size))
	{
		a->size = size;
		return ptr;
	}
	if((p = malloc(size)) == NULL)
		return NULL;
	memcpy(p, ptr, min(a->size, size));
	free(ptr);
	return p;
}
