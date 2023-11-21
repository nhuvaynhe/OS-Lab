all: main

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

main: seqlock.h main.c 
	$(CC) -c -o main.o main.c  
	$(CC) -o seqlock main.o

clean:
	rm *.o
	rm seqlock
