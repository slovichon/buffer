/* $Id$ */

#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>

struct _Buffer;
typedef struct _Buffer Buffer;

struct _VBuffer;
typedef struct _VBuffer VBuffer;

struct _Buffer {
	char *data;
	int len;
	int mem;
};

struct _VBuffer {
	Buffer *buf;
	VBuffer *next;
};

#ifndef HAS_BOOL_T
#define HAS_BOOL_T
typedef enum {
	FALSE = 0,
	TRUE  = 1,
} bool_t;
#endif

Buffer *Buffer_init(size_t len);
void Buffer_long_free(Buffer **, bool_t);
#define Buffer_free(p) Buffer_long_free(p, FALSE);
void Buffer_addch(Buffer *, char);
void Buffer_cat(Buffer *, char *);
void Buffer_set(Buffer *, char *);
void Buffer_set_range(Buffer *, char *, char*);
char *Buffer_get(Buffer *);
void Buffer_append(Buffer *, Buffer *);
/* void Buffer_ensure(Buffer *, size_t); */
int Buffer_length(Buffer *);
void Buffer_replace(Buffer *, int, int, Buffer *);
void Buffer_copy(Buffer *, Buffer *);
bool_t Buffer_is_set(Buffer *);
void Buffer_cat_range(Buffer *, char *, char *);
void Buffer_chomp(Buffer *, int);

VBuffer *VBuffer_init(void);
#define VBuffer_free(v) VBuffer_long_free(v, FALSE)
void VBuffer_long_free(VBuffer **, bool_t);
void VBuffer_add(VBuffer *, Buffer *);
Buffer *VBuffer_remove(VBuffer **);
size_t VBuffer_length(VBuffer *);

#ifndef __BARK
void bark(char *, ...);
#endif

void *xrealloc(void *, size_t);
char *xstrdup(const char *);
void *xmalloc(size_t);

#endif /* _BUFFER_H */
