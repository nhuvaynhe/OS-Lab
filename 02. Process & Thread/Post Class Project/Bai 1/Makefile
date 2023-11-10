all: main

main: main.o average_ratings.o
	gcc -o main average_ratings.o main.o

average_ratings.o: average_ratings.h average_ratings.c
	gcc -c average_ratings.c

main.o: main.c average_ratings.h
	gcc -c main.c

clean:
	rm -f *.o main

