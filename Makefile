# $Id$

SOURCES = buffer.c buftest.c
OBJECTS = buffer.o buftest.o

LIBS = -lm -lc

TARGET = buftest

#CFLAGS = -Wall -pedantic -O2 -ffast-math -pipe -march=i686 -fexpensive-optimizations
CFLAGS = -Wall -pedantic -ansi

.c.o:
	$(CC) $(CFLAGS) -c $<

all: buftest

buftest: $(OBJECTS)
	$(CC) $(LIBS) -o $@ $(OBJECTS)

clean:
	rm -f $(TARGET) *.o
