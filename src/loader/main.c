/* $Id$ */
/* Copyright (c) 2018-2019 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS uKernel */



#include <string.h>
#include <syslog.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int i;
	char buf[1024];
	size_t size = sizeof(buf) - 1;

	syslog(LOG_KERN | LOG_EMERG, "Failed to boot DeforaOS");
	for(i = 0; i < argc; i++)
	{
		strncat(buf, " ", 1);
		size -= MIN(size, 1);
		strncat(buf, argv[i], size);
		size -= MIN(size, strlen(argv[i]));
	}
	buf[sizeof(buf) - 1] = '\0';
	syslog(LOG_KERN | LOG_INFO, "Command line: %s", buf);
#ifdef DEBUG
	if(argv[i] != NULL)
		syslog(LOG_KERN | LOG_DEBUG, "%s",
				"uLoader: argv is not terminated properly");
#endif
	return 0;
}
