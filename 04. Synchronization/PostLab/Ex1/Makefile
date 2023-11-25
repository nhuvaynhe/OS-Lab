all: seqlock

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

seqlock: seqlock.c seqlock.h
	$(CC) -c -o seqlock.o seqlock.c
	$(CC) -o seqlock seqlock.o

clean:
	rm *.o
	rm seqlock
