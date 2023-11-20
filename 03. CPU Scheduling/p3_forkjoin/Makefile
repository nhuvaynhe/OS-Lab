all: main

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

main: fj_worker fj_task fj_framework
	$(CC) -c -o main.o main.c
	$(CC) -o mypool main.o fj_worker.o fj_task.o fj_framework.o 

fj_worker: fj_worker.c forkjoin.h
	$(CC) -c -o fj_worker.o fj_worker.c

fj_task: fj_task.c forkjoin.h
	$(CC) -c -o fj_task.o fj_task.c 

fj_framework: fj_framework.c forkjoin.h
	$(CC) -c -o fj_framework.o fj_framework.c

clean:
	rm *.o
	rm mypool
	rm sum_serial