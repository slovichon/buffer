/* $Id$ */

#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdio.h>

struct _Buffer;
typedef struct _Buffer Buffer;

struct _Buffer
{
	char *data;
	int len;
	int mem;
};
/*
#ifdef TRUE
# if !TRUE
#  undef TRUE
#  define TRUE 1
# endif
#else
# define TRUE 1
#endif

#ifdef FALSE
# if FALSE
#  undef FALSE
#  define FALSE 0
# endif
#else
# define FALSE 0
#endif
*/

#if !defined(HAS_BOOL)
#define HAS_BOOL
typedef enum {FALSE=0, TRUE=1} bool;
#endif

Buffer *Buffer_init(size_t len);
void Buffer_long_free(Buffer **, bool);
#define Buffer_free(p) Buffer_long_free(p, FALSE);
void Buffer_addch(Buffer *, char);
void Buffer_cat(Buffer *, char *);
void Buffer_set(Buffer *, char *);
char *Buffer_get(Buffer *);
void Buffer_append(Buffer *, Buffer *);
void Buffer_ensure(Buffer *, size_t);
int Buffer_length(Buffer *);
void Buffer_replace(Buffer *, int, int, Buffer *);
void Buffer_copy(Buffer *, Buffer *);
bool Buffer_is_set(Buffer *);
void Buffer_cat_range(Buffer *, char *, char *);

void bark(char *msg, ...);

#endif /* _BUFFER_H */
