/* $Id */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void *xrealloc(void *p, size_t siz)
{
	assert((p = realloc(p, siz)) != NULL);
	return p;
}

char *xstrdup(const char *s)
{
	char *t;
	assert((t = strdup(s)) != NULL);
	return t;
}

void *xmalloc(size_t siz)
{
	void *p;
	assert((p = malloc(siz)) != NULL);
	return p;
}
