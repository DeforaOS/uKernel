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
static void _free_abort(void);

void free(void * ptr)
{
	Alloc * a;
	Alloc * b;

	if(ptr == NULL)
		return;
	if(ptr < (void *)sizeof(*a))
	{
		_free_abort();
		return;
	}
	a = (Alloc *)ptr - 1;
	b = a->prev;
	if(b->next != a)
	{
		_free_abort();
		return;
	}
	b->next = a->next;
	if(a->next != NULL) /* memory is allocated past a */
		a->next->prev = b;
	else if(b != &_alloc)
		/* decrease to lowest possible value */
		sbrk(-((uintptr_t)a + a->size - (uintptr_t)b - b->size));
	else
		/* remove the last object */
		sbrk(-sizeof(*a) - a->size);
}

static void _free_abort(void)
{
	const char buf[] = "invalid free detected: terminated\n";

	write(2, buf, sizeof(buf) - 1);
	abort();
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
	uintptr_t inc;

	if(size >= SSIZE_MAX - sizeof(*b) - 0x8)
	{
		errno = ENOMEM;
		return NULL;
	}
	if((size & 0x7) != 0x0)
		size = (size | 0x7) + 1; /* round up to 64 bits */
	inc = size + sizeof(*b);
	if(_alloc.next != NULL) /* look for available space */
		for(a = _alloc.next; a->next != NULL; a = a->next)
			if(inc <= (uintptr_t)a->next - (uintptr_t)a - sizeof(*a)
					- a->size)
			{
				b = (Alloc *)((uintptr_t)a + sizeof(*a)
						+ a->size);
				a->next->prev = b;
				break;
			}
	if(b == NULL) /* increase process size to allocate memory */
		if((b = sbrk(inc)) == (void *)-1)
			return NULL;
	b->size = size;
	b->prev = a;
	b->next = a->next;
	a->next = b;
	return b + 1;
}


/* realloc */
void * realloc(void * ptr, size_t size)
{
	Alloc * a = (Alloc *)ptr - 1;
	void * p;

	if(ptr == NULL)
		return malloc(size);
	if((size & 0x7) != 0x0)
		size = (size | 0x7) + 1; /* round up to 64 bits */
	if(size == a->size)
		return ptr;
	if(a->next == NULL)
	{
		/* reallocate the space */
		if(sbrk(size - a->size) == (void *)-1)
			return NULL;
		a->size = size;
		return ptr;
	}
	if(size < a->size || (uintptr_t)a->next - (uintptr_t)a - sizeof(*a)
			>= size)
	{
		/* update the size */
		a->size = size;
		return ptr;
	}
	if((p = malloc(size)) == NULL)
		return NULL;
	memcpy(p, ptr, min(a->size, size));
	free(ptr);
	return p;
}
