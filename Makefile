# $Id$

OBJECTS = buffer.o xalloc.o
LIBS = -lm -lc -L.
TARGET = libbuffer.so
CFLAGS = -Wall

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(LIBS) -o $@ $(OBJECTS)
	chmod 444 $@

test: $(TARGET) buftest.o
	$(CC) $(LIBS) -o buftest buftest.o -lbuffer

buftest: $(OBJECTS)
	$(CC) $(LIBS) -o $@ $(OBJECTS)

clean:
	rm -f $(TARGET) $(OBJECTS) buftest buftest.o
