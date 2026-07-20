CC=gcc
CFLAGS=-Wall -fPIC -shared

all: malicious.so

malicious.so: malicious.c
	$(CC) $(CFLAGS) -o malicious.so malicious.c

clean:
	rm -f malicious.so
