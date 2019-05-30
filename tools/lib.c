/* $Id$ */
/* Copyright (c) 2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <ssp.h>

#include "lib/libc/src/ctype.c"
#include "lib/libc/src/errno.c"
#include "lib/libc/src/ssp.c"
#include "lib/libc/src/stdio.c"
#include "lib/libc/src/stdlib.c"
#include "lib/libc/src/string.c"
#include "lib/libc/src/sys/mman.c"
#include "lib/libc/src/time.c"
#include "lib/libc/src/unistd.c"
