all: main

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

main: bkwrk bktask bktpool shared_memory
	$(CC) -c -o main.o main.c
	$(CC) -o mypool main.o bkwrk.o bktask.o bktpool.o shared_memory.o

bkwrk: bkwrk.c bktpool.h
	$(CC) -c -o bkwrk.o bkwrk.c

bktask: bktask.c bktpool.h
	$(CC) -c -o bktask.o bktask.c 

bktpool: bktpool.c bktpool.h
	$(CC) -c -o bktpool.o bktpool.c

shared_memory: bktpool.h shared_memory.c 
	$(CC) -c -o shared_memory.o shared_memory.c

clean:
	rm *.o
	rm mypool
