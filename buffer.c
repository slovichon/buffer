#include "buffer.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void Buffer_long_free(Buffer **p, bool saveptr)
{
	if (p && *p) {
		if ((*p)->data && !saveptr) {
			free((*p)->data);
			(*p)->data = NULL;
		}
//bark("data freed");
		(*p)->mem = 0;
		(*p)->len = -1;
//bark("freeing pointer");
		free(*p);
		*p = NULL;
//bark("buffer freed");
	}
}

int Buffer_length(Buffer *p)
{
	return p->len;
}

Buffer *Buffer_init(size_t len)
{
	Buffer *p;
	assert((p = (Buffer*)malloc(sizeof(Buffer))) != NULL);
	p->data = NULL;
	p->len = -1;
	p->mem = 0;
//bark("ensuring %d", len);
	Buffer_ensure(p, len);
	/*
	 * Buffer_ensure() sets len to 0 for future
	 * invocations, so we must set it again
	 */
//	p->len = -1;
	return p;
}

void Buffer_replace(Buffer *dst, int start, int len, Buffer *src)
{
	int oldlen, newlen;
	oldlen = Buffer_length(dst);
	newlen = oldlen - len + Buffer_length(src);
	Buffer_ensure(dst, newlen);
	/* move check after target area into safe zone */
	memmove(dst->data+start+Buffer_length(src), dst->data+start+len,
		oldlen-(start+len));
	memcpy(dst->data+start, src->data, Buffer_length(src));
}

void Buffer_addch(Buffer *p, char ch)
{
	int len = p->len;
	if (len <= 0) {
		/* buffer hasn't been initialized yet */
//bark("uninitialized!");
		p->len = len = 1; /* new '\0' */
	}
//bark("len: %d (adding %c after %c)", len, ch, p->data[len-2]);
	Buffer_ensure(p, len+1);
	p->data[len-1] = ch;
	p->data[len] = '\0';
	p->len++;
//fprintf(stderr, "%c", ch);fflush(stderr);
}

void Buffer_append(Buffer *dst, Buffer *src)
{
	if (dst->len >= 0) {
		int len = Buffer_length(dst) + Buffer_length(src) - 1;
		Buffer_ensure(dst, len);
		strncat(dst->data, src->data, Buffer_length(src)-1);
		dst->data[len-1] = '\0';
		dst->len = len;
	} else {
		/* buffer hasn't been initialized yet */
		Buffer_copy(dst, src);
	}
}

void Buffer_copy(Buffer *dst, Buffer *src)
{
	assert((dst->data = strdup(src->data)) != NULL);
	dst->mem = src->mem;
	dst->len = src->len;
}

void Buffer_set(Buffer *p, char *s)
{
	int len = strlen(s)+1;
//bark("ensuring to %d (len: %d mem: %d)", len, p->len, p->mem);
	Buffer_ensure(p, len);
	strncpy(p->data, s, len-1);
	p->data[len-1] = '\0';
bark("buffer set to %s", p->data);
	p->len = len;
}

void Buffer_cat(Buffer *p, char *s)
{
	if (p->len >= 0)
	{
		int len = p->len+strlen(s);
		Buffer_ensure(p, len);
		strncat(p->data, s, strlen(s)+1);
//bark("catting buffer with %s", s);
		p->data[len-1] = '\0';
		p->len = len;
	} else {
		/* buffer hasn't been initialized yet */
bark("uninitialized buffer catted!");
		Buffer_set(p, s);
	}
}

char *Buffer_get(Buffer *p)
{
	/* make sure the buffer has been initialized*/
	/* XXX: return "" if it hasn't? */
	assert(p->data != NULL);
	return p->data;
}

void Buffer_ensure(Buffer *p, size_t len)
{
	int newlen;
	if (p->mem < len) {
//bark("buffer (%s) not big enough, resizing", p->data);
		newlen = 8*ceil((1+(len * 3)/2)/8); /* 1/log(len) */
//bark("increasing capacity from %d <len %d> to %d (%d)", p->mem, p->len, newlen, len);
		assert((p->data = (char *)realloc(p->data, newlen)) != NULL);
		p->mem = newlen;
//bark("mem now %d", p->mem);
		if (p->len < 0) {
			p->len = 0;
			p->data[0] = '\0';
		}
//bark("writing %d zeros at %p (%c)", newlen-p->len, p->data+p->len, p->data[p->len-1]);
		bzero(p->data+p->len, newlen-p->len);
	}
}

bool Buffer_is_set(Buffer *p)
{
	return p != NULL && p->data != NULL;
}

void Buffer_cat_range(Buffer *p, char *start, char *end)
{
	int len = p->len + end-start;
bark("[len: %d] [mem: %d] [data: %s]", p->len, p->mem, p->data);
	Buffer_ensure(p, len);
	strncat(p->data, start, end-start);
	p->data[len-1] = '\0';
	p->len = len;
bark("[len: %d] [mem: %d] [data: %s]", p->len, p->mem, p->data);
}
