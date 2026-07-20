CC=gcc
CFLAGS=-Wall

all: memfd_exec dummy_payload

memfd_exec: memfd_exec.c
	$(CC) $(CFLAGS) -o memfd_exec memfd_exec.c

dummy_payload: dummy_payload.c
	$(CC) $(CFLAGS) -o dummy_payload dummy_payload.c

clean:
	rm -f memfd_exec dummy_payload
