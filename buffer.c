/* $Id$ */

#include <string.h>
#include <stdlib.h>
/* #include <math.h> */
#include <assert.h>
#include "buffer.h"

static void Buffer_ensure(Buffer *p, size_t len);

/*
 * Frees a buffer. Some pointers may point to
 * our data member, so there is an argument to
 * free that field or not.
 *
 * Note that Buffer_free() is a macro to NOT
 * save the data pointer.
 */
void Buffer_long_free(Buffer **p, bool_t saveptr)
{
	if (p && *p) {
		if ((*p)->data && !saveptr)
			free((*p)->data);
		/*
		 * There is no point in clearing the
		 * fields here (data, mem, len) because
		 * it won't be used anymore anyhow.
		 */
		free(*p);
		*p = NULL;
	}
}

int Buffer_length(Buffer *p)
{
	return p->len;
}

Buffer *Buffer_init(size_t len)
{
	Buffer *p;
	p = (Buffer *)xmalloc(sizeof(Buffer));
	p->data = NULL;
	p->len = -1;
	p->mem = 0;
	Buffer_ensure(p, len);
	/*
	 * Buffer_ensure() sets len to 0 for future
	 * invocations to it, so we must reset len again.
	 */
//	p->len = -1;
	return p;
}

/*
 * Remove a section of a buffer and replace with another
 * buffer.
 */
void Buffer_replace(Buffer *dst, int start, int len, Buffer *src)
{
	int oldlen, newlen;
	oldlen = Buffer_length(dst);
	newlen = oldlen - len + Buffer_length(src);
	Buffer_ensure(dst, newlen);
	/*
	 * Move remaining buffer to 'safe zone' so it won't get
	 * clobbered with memcpy() next.
	 */
	memmove(dst->data + start + Buffer_length(src),
		dst->data + start + len, oldlen - (start + len));
	memcpy(dst->data + start, src->data, Buffer_length(src));
}

/*
 * Add a single character to a buffer.
 */
void Buffer_addch(Buffer *p, char ch)
{
	int len = p->len;
	/* Our data is already zero-bounded. */
	if (ch == '\0')
		return;
	if (len <= 0) {
		/* Buffer isn't initialized. */
		p->len = len = 1; /* new '\0' */
	}
	Buffer_ensure(p, len + 1);
	p->data[len - 1] = ch;
	p->data[len] = '\0';
	p->len++;
}

void Buffer_append(Buffer *dst, Buffer *src)
{
	if (dst->len > 0) {
		int len = Buffer_length(dst) + Buffer_length(src) - 1;
		Buffer_ensure(dst, len);
		strncat(dst->data, src->data, Buffer_length(src) - 1);
		dst->data[len - 1] = '\0';
		dst->len = len;
	} else {
		/* Buffer isn't initialized, just copy. */
		Buffer_copy(dst, src);
	}
}

void Buffer_copy(Buffer *dst, Buffer *src)
{
	dst->data = xstrdup(src->data);
	dst->mem = src->mem;
	dst->len = src->len;
}

void Buffer_set(Buffer *p, char *s)
{
	int len = strlen(s) + 1; /* '\0' */
	Buffer_ensure(p, len);
	strncpy(p->data, s, len - 1);
	p->data[len - 1] = '\0';
	p->len = len;
}

void Buffer_set_range(Buffer *p, char *start, char *end)
{
	int len = end - start + 1;
	Buffer_ensure(p, len);
	strncpy(p->data, start, len);
	p->data[len - 1] = '\0';
	p->len = len;
}

/*
 * Concatenate a char array onto a buffer.
 */
void Buffer_cat(Buffer *p, char *s)
{
	if (Buffer_is_set(p))
	{
		int len = p->len + strlen(s);
		Buffer_ensure(p, len);
		strncat(p->data, s, strlen(s));
		p->data[len - 1] = '\0';
		p->len = len;
	} else {
		Buffer_set(p, s);
	}
}

char *Buffer_get(Buffer *p)
{
	/* Make sure the buffer has been initialized. */
	/* XXX: return "" if it hasn't? */
	assert(p->data != NULL);
	return p->data;
}

static void Buffer_ensure(Buffer *p, size_t len)
{
	int newlen;
	if (p->mem < len) {
		/* Round to 8-byte boundary for some poor malloc()'s. */
#if 0
		newlen = 8*ceil((double)(1+(len * 3)/2)/8); /* 1/log(len) */
#endif
		len = (len * 3) / 2;
		newlen = (len & ~0x7) + (len & 0x7 ? 8 : 0);
		p->data = (char *)xrealloc(p->data, newlen);
		p->mem = newlen;
		if (p->len < 0) {
			p->len = 0;
			p->data[0] = '\0';
		}
		bzero(p->data + p->len, newlen - p->len);
	}
}

bool_t Buffer_is_set(Buffer *p)
{
	return (p != NULL) && (p->data != NULL);
}

void Buffer_cat_range(Buffer *p, char *start, char *end)
{
	int len;

	if (Buffer_is_set(p)) {
		len = p->len + end - start;
		Buffer_ensure(p, len);
		strncat(p->data, start, end - start);
		p->data[len - 1] = '\0';
		p->len = len;
	} else {
		Buffer_set_range(p, start, end);
	}
}

void Buffer_chomp(Buffer *p, int num)
{
	if (Buffer_is_set(p) && (Buffer_length(p) > num-1)) {
		p->data[Buffer_length(p) - num] = '\0';
		p->len -= num-1;
	}
}

/*
 * Buffer vector - array of buffers.
 */
VBuffer *VBuffer_init(void)
{
	VBuffer *v;
	v = (VBuffer *)xmalloc(sizeof(VBuffer));
	v->buf = NULL;
	v->next = NULL;
	return v;
}

#if 0
void VBuffer_clear(VBuffer **v)
{
	VBuffer **i;
	if (*v != NULL) {
		for (i = v; (*i)->next != NULL; i = &(*i)->next) {
			Buffer_free((*i)->buf);
			free(*i);
			*i = NULL;
		}
	}
}
#endif

/*
 * Frees a buffer vector. The second argument specifies
 * whether or not to free the associated buffers.
 *
 * Note that VBuffer_free() is a macro for this to not
 * save the buffers.
 */
void VBuffer_long_free(VBuffer **v, bool_t save)
{
	if ((v != NULL) && (*v != NULL)) {
		if (!save)
			Buffer_free(&(*v)->buf);
		free(*v);
		*v = NULL;
	}
}

/*
 * Add a buffer to a buffer vector.
 */
void VBuffer_add(VBuffer *v, Buffer *p)
{
	VBuffer *new, *i;
	if (v->buf == NULL) {
		/* If this node is empty, use it. */
		v->buf = p;
	} else {
		new = VBuffer_init();
		new->buf = p;
		/* Find last vbuf. */
		for (i = v; i->next != NULL; i = i->next);
		i->next = new;
	}
}

/*
 * Remove a buffer from a buffer vector.
 */
Buffer *VBuffer_remove(VBuffer **v)
{
	VBuffer *newlast = NULL, *last;
	Buffer *p;
	/*
	 * We need to find and return the last buffer and
	 * make the second-last buffer the new last buffer.
	 */
	for (last = *v; last && (last->next != NULL); last = last->next)
		newlast = last;
	/* This function was called on an empty vbuf. */
	if ((newlast == NULL) && (last == NULL))
		return NULL;
	p = last->buf;
	if (newlast == NULL) {
		/* This is the last buf; clear original vbuf. */
		VBuffer_long_free(v, TRUE);
	} else {
		newlast->next = NULL;
		VBuffer_long_free(&last, TRUE);
	}
	return p;
}

/*
 * Find the number of buffers in a buffer vector.
 */
size_t VBuffer_length(VBuffer *v)
{
	VBuffer *i;
	size_t n = 0;
	for (i = v; i; n++, i = i->next)
		;
	return n;
}

#ifndef __BARK
#include <stdarg.h>
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
#endif

