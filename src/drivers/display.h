/* $Id$ */
/* Copyright (c) 2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#ifndef UKERNEL_DRIVERS_DISPLAY_H
# define UKERNEL_DRIVERS_DISPLAY_H

# include <stdint.h>
# include "bus.h"


/* public */
/* types */
typedef struct _ukDisplay ukDisplay;

typedef enum _ukDisplayMode
{
	DISPLAY_MODE_TEXT = 0,
	DISPLAY_MODE_GRAPHICS
} ukDisplayMode;

typedef struct _ukDisplayData ukDisplayData;

struct _ukDisplay
{
	char name[16];

	ukDisplay * (*init)(ukBus * bus);
	void (*destroy)(ukDisplay * display);

	int (*set_mode)(ukDisplay * display, ukDisplayMode mode,
			unsigned int width, unsigned int height,
			unsigned int depth);

	void (*clear)(ukDisplay * display);

	void (*print)(ukDisplay * display, char const * str, size_t len);

	ukDisplayData * data;
};


/* prototypes */
ukDisplay * display_init(ukBus * bus, char const * name);
void display_destroy(ukDisplay * display);

/* accessors */
ukDisplay * display_get_default(void);

int display_set_mode(ukDisplay * display, ukDisplayMode mode,
		unsigned int width, unsigned int height, unsigned int depth);

/* helpers */
void display_clear(ukDisplay * display);

void display_print(ukDisplay * display, char const * str, size_t len);

#endif /* !UKERNEL_DRIVERS_DISPLAY_H */
