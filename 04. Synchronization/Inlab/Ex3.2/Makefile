all: pc_sem

pc_sem: pc_sem.c
	gcc -c -o pc_sem.o pc_sem.c
	gcc -o pc_sem pc_sem.o

clean:
	rm *.o
	rm pc_sem
