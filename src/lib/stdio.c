/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>

#define min(a, b) ((a) > (b) ? (b) : (a))


/* private */
/* types */
typedef enum _FILEDirection { FD_READ = 0, FD_WRITE = 1 } FILEDirection;

struct _FILE
{
	int fd;
	int flags;
	unsigned char _buf[BUFSIZ];
	unsigned char * buf;
	unsigned char * mbuf;
	size_t bufsiz;
	size_t len;
	size_t pos;
	char eof;
	char error;
	int fbuf;
	FILEDirection dir;
};


/* public */
/* variables */
static FILE _stdin =
{
	STDIN_FILENO, O_RDONLY, { 0 }, _stdin._buf, NULL, sizeof(_stdin._buf),
	0, 0, 0, 0, _IOFBF, FD_READ
};
FILE * stdin = &_stdin;

static FILE _stdout =
{
	STDOUT_FILENO, O_WRONLY, { 0 }, _stdout._buf, NULL,
	sizeof(_stdout._buf), 0, 0, 0, 0, _IOLBF, FD_WRITE
};
FILE * stdout = &_stdout;

static FILE _stderr =
{
	STDERR_FILENO, O_WRONLY, { 0 }, _stderr._buf, NULL,
	sizeof(_stderr._buf), 0, 0, 0, 0, _IONBF, FD_WRITE
};
FILE * stderr = &_stderr;


/* functions */
/* fflush */
int fflush(FILE * file)
{
	ssize_t w;

	if(file == NULL)
	{
		errno = ENOSYS; /* FIXME implement */
		return EOF;
	}
	if(file->dir == FD_WRITE)
	{
		for(; file->pos < file->len; file->pos += w)
			if((w = write(file->fd, &file->buf[file->pos], file->len
							- file->pos)) < 0)
			{
				file->error = 1;
				return EOF;
			}
	}
	else if(file->dir != FD_READ)
		return EOF;
	file->pos = 0;
	file->len = 0;
	return 0;
}


/* fputc */
int fputc(int c, FILE * file)
{
	unsigned char p = c;

	if(fwrite(&p, sizeof(char), 1, file) != 1)
		return EOF;
	return p;
}


/* fwrite */
size_t fwrite(void const * ptr, size_t size, size_t nb, FILE * file)
{
	char const * p = (char const *)ptr;
	size_t i;
	size_t j;
	size_t len;
	ssize_t w;
	size_t x;

	if(file->dir != FD_WRITE)
	{
		if(fflush(file) != 0)
			return 0;
		file->dir = FD_WRITE;
	}
	for(i = 0; i < nb; i++)
		for(j = 0; j < size; j+=len)
		{
			len = min(file->bufsiz - file->len, size - j);
			memcpy(&file->buf[file->len], p, len);
			p += len;
			file->len += len;
			if(file->len != file->bufsiz) /* buffer is not full */
				continue;
			x = file->bufsiz - file->pos;
			if((w = write(file->fd, &file->buf[file->pos], x)) < 0)
			{
				file->error = 1;
				return i;
			}
			if((size_t)w != x) /* XXX cast */
				file->pos = w; /* buffer is not empty */
			else /* buffer is empty */
			{
				file->pos = 0;
				file->len = 0;
			}
		}
	if(file->fbuf == _IOFBF) /* fully buffered */
		return nb;
	if(file->fbuf == _IOLBF) /* line buffered */
		for(j = file->len; j > file->pos; j--)
		{
			if(file->buf[j - 1] == '\n')
				break;
		}
	else /* unbuffered */
		j = file->len;
	for(; file->pos < j; file->pos += w) /* empty buffer if necessary */
		if((w = write(file->fd, &file->buf[file->pos], j - file->pos))
				< 0)
		{
			file->error = 1;
			break; /* XXX should we otherwise report the error? */
		}
	return nb;
}


/* printf */
int printf(char const * format, ...)
{
	int ret;
	va_list arg;

	va_start(arg, format);
	ret = vfprintf(stdout, format, arg);
	va_end(arg);
	return ret;
}


/* putc */
int putc(int c, FILE * file)
{
	return fputc(c, file);
}


/* putchar */
int putchar(int c)
{
	return fputc(c, stdout);
}


/* puts */
int puts(char const * string)
{
	size_t i;
	
	i = strlen(string);
	if(fwrite(string, sizeof(char), i, stdout) != i)
		return EOF;
	fputc('\n', stdout);
	return i <= INT_MAX ? i : INT_MAX;
}


/* vfprintf */
typedef int (*print_func)(void * dest, size_t size, const char * buf);
static int _fprint(void * dest, size_t size, const char * buf);
static int _vprintf(print_func func, void * dest, size_t size,
		char const * format, va_list arg);

int vfprintf(FILE * file, char const * format, va_list arg)
{
	return _vprintf(_fprint, file, ~0, format, arg);
}

static int _fprint(void * dest, size_t size, char const buf[])
{
	FILE * file = dest;

	return (fwrite(buf, sizeof(char), size, file) == size) ? 0 : -1;
}

/* _vprintf */
typedef struct _print_args
{
	int ret;
	int flags;
	size_t width;
	size_t precision;
	int shrt;
	print_func func;
	void * dest;
	size_t * size;
} print_args;
static int _vfprintf_do(print_args * args, char const * buf, size_t len);
static int _vfprintf_do_do(print_args * args, char const * buf, size_t len);
#define FLAGS_HASH	0x01
#define FLAGS_MINUS	0x02
#define FLAGS_PLUS	0x04
#define FLAGS_SPACE	0x08
#define FLAGS_ZERO	0x10
static int _vprintf_flags(char const * p, size_t * i);
static size_t _vprintf_width(char const * p, size_t * i);
static size_t _vprintf_precision(char const * p, size_t * i);
static void _format_lutoa(char * dest, unsigned long n, size_t base, int upper);
static int _format_c(print_args * args, char const * chrp);
static int _format_d(print_args * args, long long * ptr);
static int _format_f(print_args * args, double * ptr);
static int _format_lf(print_args * args, long double * ptr);
static int _format_o(print_args * args, unsigned long long * ptr);
static int _format_s(print_args * args, char const * str);
static int _format_p(print_args * args, void * ptr);
static int _format_u(print_args * args, unsigned long long * ptr);
static int _format_x(print_args * args, unsigned long long * ptr, int upper);

static int _vprintf(print_func func, void * dest, size_t size,
		char const * format, va_list arg)
{
	print_args args;
	char const * p;		/* pointer to current format character */
	size_t i;
	int lng;
	int f = 0;
	char c;
	char const * str;
	double e;
	long double le;
	long long int d;
	unsigned long long int u;
	void * ptr;

	args.ret = 0;
	args.func = func;
	args.dest = dest;
	args.size = &size;
	for(p = format; *p != '\0'; p += i)
	{
		args.flags = 0;
		args.width = 0;
		args.precision = 0;
		args.shrt = 0;
		for(i = 0; p[i] != '\0' && p[i] != '%'; i++);
		if(i > 0 && _vfprintf_do(&args, p, i) != 0)
			return -1;
		if(p[i++] != '%')
			break;
		if(p[i] == '%')
		{
			if(_vfprintf_do(&args, p++, 1) != 0)
				return -1;
			continue;
		}
		args.flags = _vprintf_flags(p, &i);
		args.width = _vprintf_width(p, &i);
		args.precision = _vprintf_precision(p, &i);
		for(lng = 0; p[i] != '\0'; i++)
		{
			if(p[i] == 'L')
			{
				if(lng > 0)
				{
					errno = EINVAL;
					return -1;
				}
				lng = -1;
			}
			else if(p[i] == 'l')
			{
				if(lng < 0 || ++lng > 2)
				{
					errno = EINVAL;
					return -1;
				}
			}
			else if(p[i] == 'c')
			{
				c = va_arg(arg, int);
				f = _format_c(&args, &c);
				break;
			}
			else if(p[i] == 'd')
			{
				if(lng > 1)
					d = va_arg(arg, long long int);
				else if(lng == 1)
					d = va_arg(arg, long int);
				else
					d = va_arg(arg, int);
				f = _format_d(&args, &d);
				break;
			}
			else if(p[i] == 'f')
			{
				if(lng < 0)
				{
					le = va_arg(arg, long double);
					f = _format_lf(&args, &le);
				}
				else
				{
					e = va_arg(arg, double);
					f = _format_f(&args, &e);
				}
				break;
			}
			else if(p[i] == 'h')
			{
				if(++args.shrt > 2)
				{
					errno = EINVAL;
					return -1;
				}
			}
			else if(p[i] == 'o')
			{
				if(lng > 1)
					u = va_arg(arg, unsigned long long int);
				else if(lng == 1)
					u = va_arg(arg, unsigned long int);
				else
					u = va_arg(arg, unsigned int);
				f = _format_o(&args, &u);
				break;
			}
			else if(p[i] == 'p')
			{
				ptr = va_arg(arg, void *);
				f = _format_p(&args, ptr);
				break;
			}
			else if(p[i] == 's')
			{
				str = va_arg(arg, char const *);
				f = _format_s(&args, str);
				break;
			}
			else if(p[i] == 'u')
			{
				if(lng > 1)
					u = va_arg(arg, unsigned long long int);
				else if(lng == 1)
					u = va_arg(arg, unsigned long int);
				else
					u = va_arg(arg, unsigned int);
				f = _format_u(&args, &u);
				break;
			}
			else if(p[i] == 'x' || p[i] == 'X')
			{
				if(lng > 1)
					u = va_arg(arg, unsigned long long int);
				else if(lng == 1)
					u = va_arg(arg, unsigned long int);
				else
					u = va_arg(arg,	unsigned int);
				f = _format_x(&args, &u, (p[i] == 'X'));
				break;
			}
			else if(p[i] == 'z')
			{
				if(lng != 0)
				{
					errno = EINVAL;
					return -1;
				}
				lng = 1;
			}
			else
			{
				errno = EINVAL;
				return -1;
			}
		}
		if(f != 0)
			return -1;
		i++;
	}
	return args.ret;
}

static int _vfprintf_do(print_args * args, char const * buf, size_t len)
{
	size_t i;
	char padding = ' ';

	if(args->flags & FLAGS_MINUS)
	{
		_vfprintf_do_do(args, buf, len);
		for(i = len; i < args->width; i++)
			if(_vfprintf_do_do(args, &padding, 1) != 0)
				return -1;
		return 0;
	}
	if(args->flags & FLAGS_ZERO)
		padding = '0';
	for(i = len; i < args->width; i++)
		if(_vfprintf_do_do(args, &padding, 1) != 0)
			return -1;
	return _vfprintf_do_do(args, buf, len);
}

static int _vfprintf_do_do(print_args * args, char const * buf, size_t len)
{
	size_t s;

	s = min(*(args->size), len);
	if(args->func(args->dest, s, buf) != 0)
		return -1;
	if((size_t)INT_MAX - len < (size_t)(args->ret))
	{
		errno = ERANGE;
		return -1;
	}
	*(args->size) -= s;
	args->ret += len;
	return 0;
}

static int _vprintf_flags(char const * p, size_t * i)
{
	int flags;

	for(flags = 0; p[*i] != '\0'; (*i)++)
	{
		if(p[*i] == '#')
			flags |= FLAGS_HASH;
		else if(p[*i] == '-')
			flags |= FLAGS_MINUS;
		else if(p[*i] == '+')
			flags |= FLAGS_PLUS;
		else if(p[*i] == ' ')
			flags |= FLAGS_SPACE;
		else if(p[*i] == '0')
			flags |= FLAGS_ZERO;
		else
			break;
	}
	return flags;
}

static size_t _vprintf_width(char const * p, size_t * i)
{
	size_t width;

	for(width = 0; p[*i] != '\0'; (*i)++)
	{
		if(p[*i] < '0' || p[*i] > '9')
			break;
		width *= 10;
		width += p[*i] - '0';
	}
	return width;
}

static size_t _vprintf_precision(char const * p, size_t * i)
{
	if(p[*i] != '.')
		return 0;
	(*i)++;
	return _vprintf_width(p, i);
}

static int _format_c(print_args * args, char const * chrp)
{
	return _vfprintf_do(args, chrp, sizeof(*chrp));
}

static int _format_d(print_args * args, long long * ptr)
{
	unsigned long long uval;
	char tmp[20] = "-";

	if(*ptr >= 0)
		return _format_u(args, (unsigned long long *)ptr);
	uval = llabs(*ptr);
	/* XXX assumes tmp is large enough */
	_format_lutoa(&tmp[1], uval, 10, 0);
	return _vfprintf_do(args, tmp, strlen(tmp));
}

static int _format_f(print_args * args, double * ptr)
{
	/* FIXME really implement */
	unsigned long long uval;

	if(*ptr >= 0.0)
	{
		uval = *ptr;
		return _format_u(args, &uval);
	}
	uval = -(*ptr);
	if(_vfprintf_do(args, "-", 1) != 0 || _format_u(args, &uval) != 0)
		return -1;
	return 0;
}

static int _format_lf(print_args * args, long double * ptr)
{
	/* FIXME really implement */
	unsigned long long uval;

	if(*ptr >= 0.0)
	{
		uval = *ptr;
		return _format_u(args, &uval);
	}
	uval = -(*ptr);
	if(_vfprintf_do(args, "-", 1) != 0 || _format_u(args, &uval) != 0)
		return -1;
	return 0;
}

static int _format_o(print_args * args, unsigned long long * ptr)
{
	char tmp[22] = "";

	_format_lutoa(tmp, *ptr, 8, 0); /* XXX assumes tmp is large enough */
	return _vfprintf_do(args, tmp, strlen(tmp));
}

static int _format_s(print_args * args, char const * str)
{
	if(str == NULL) /* XXX be nice and do not crash */
		str = "(null)";
	return _vfprintf_do(args, str, strlen(str));
}

static int _format_p(print_args * args, void * ptr)
{
	char tmp[3 + sizeof(void*) + sizeof(void*)] = "0x";

	_format_lutoa(&tmp[2], (long)ptr, 16, 0); /* XXX cast */
	return _vfprintf_do(args, tmp, strlen(tmp));
}

static int _format_u(print_args * args, unsigned long long * ptr)
{
	char tmp[19] = "";

	_format_lutoa(tmp, *ptr, 10, 0); /* XXX assumes tmp is large enough */
	return _vfprintf_do(args, tmp, strlen(tmp));
}

static int _format_x(print_args * args, unsigned long long * ptr, int upper)
{
	unsigned long u = *ptr;
	char tmp[sizeof(long) + sizeof(long) + 1] = "";

	if(args->shrt == 1)
		u = u & 0xffff;
	else if(args->shrt == 2)
		u = u & 0xff;
	_format_lutoa(tmp, u, 16, upper); /* XXX assumes tmp large enough */
	if((args->flags & FLAGS_HASH) == FLAGS_HASH)
		if(_vfprintf_do_do(args, "0x", 2) != 0)
			return -1;
	return _vfprintf_do(args, tmp, strlen(tmp));
}

/* PRE	dest is long enough
 * POST	2 <= base <= 36		dest is the ascii representation of n
 *	else			dest is an empty string */
static void _format_lutoa(char * dest, unsigned long n, size_t base, int upper)
{
	static char const convl[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static char const convu[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char const * conv = (upper != 0) ? convu : convl;
	size_t len = 0;
	unsigned long p;
	size_t i;

	if(base < 2 || base >= sizeof(convl))
	{
		dest[0] = '\0';
		return;
	}
	if(n == 0)
	{
		strcpy(dest, "0");
		return;
	}
	/* XXX performing twice the divisions is not optimal */
	for(p = n; p > 0; p /= base)
		len++;
	for(i = len; n > 0; n /= base)
	{
		p = n % base;
		dest[--i] = conv[p];
		n -= p;
	}
	dest[len] = '\0';
}
