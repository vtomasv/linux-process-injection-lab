CC=gcc
CFLAGS=-Wall -g

all: target ptrace_inject

target: target.c
	$(CC) $(CFLAGS) -o target target.c

ptrace_inject: ptrace_inject.c
	$(CC) $(CFLAGS) -o ptrace_inject ptrace_inject.c

clean:
	rm -f target ptrace_inject
