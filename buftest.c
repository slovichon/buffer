/* $Id$ */

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

void bark(char *msg, ...)
{
	va_list ap;
	fprintf(stderr, "[ERR] ");
	va_start(ap, msg);
	(void)vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	(void)fflush(stderr);
}

int main(int argc, char *argv[])
{
	Buffer *p, *q;
	char *s;

	p = Buffer_init(30);
	//Buffer_set(p, "Yo what's up");

	Buffer_addch(p, ',');
	Buffer_addch(p, ' ');
	Buffer_addch(p, 'm');
	Buffer_addch(p, 'y');
	Buffer_addch(p, ' ');
	Buffer_addch(p, 'm');
	Buffer_addch(p, 'a');
	Buffer_addch(p, 'n');
	Buffer_addch(p, '?');

	Buffer_cat(p, " Not much here brotha!");

	s = Buffer_get(p);
bark("buffer contents gathered");
	printf("buf: %s\n", s);

//	Buffer_free(&p);
//bark("buffer freed");

bark("--------------------");

	q = Buffer_init(22);
	Buffer_set(q, " I asked what was up.");

	s = Buffer_get(p);
	printf("p: %s\n", s);

	s = Buffer_get(q);
	printf("q: %s\n", s);

	Buffer_append(p, q);

	s = Buffer_get(p);
bark("buffer contents gathered");
	printf("buf: %s\n", s);

	Buffer_free(&p);
	Buffer_free(&q);

	return 0;
}
