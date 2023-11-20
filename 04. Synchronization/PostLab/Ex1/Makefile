all: main

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

main: seqlock 
	$(CC) -c -o main.o main.c 
	$(CC) -o seqlock main.o seqlock.o

seqlock: seqlock.h seqlock.c
	$(CC) -c -o seqlock.o seqlock.c

clean:
	rm *.o
	rm seqlock
