CC=gcc
CFLAGS=-Wall -fPIC -shared

all: libmalware.so

libmalware.so: injector.c
	$(CC) $(CFLAGS) -o libmalware.so injector.c

clean:
	rm -f libmalware.so /tmp/libmalware.so /tmp/liquidacion_junio_2026.txt

.PHONY: all clean
