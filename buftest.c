/* $Id$ */

#include <stdio.h>
#include <stdarg.h>
#include "buffer.h"

void vbuf_test(void);

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

	vbuf_test();

	return 0;
}

void vbuf_test(void)
{
	VBuffer *v;
	Buffer *p;

	v = VBuffer_init();

	p = Buffer_init(5);
	Buffer_set(p, "test");
	VBuffer_add(v, p);

	p = Buffer_init(5);
	Buffer_set(p, "test2");
	VBuffer_add(v, p);

	p = Buffer_init(12);
	Buffer_set(p, "another test");
	VBuffer_add(v, p);

	bark("len: %d", VBuffer_length(v));

	/* remove bufs */
	while ((p = VBuffer_remove(&v)) != NULL) {
		printf("vbuf: %s\n", Buffer_get(p));
		Buffer_free(&p);
	}

	bark("done!");

	VBuffer_free(&v);
}
