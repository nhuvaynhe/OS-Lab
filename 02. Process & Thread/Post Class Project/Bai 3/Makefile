all: node_1 node_2

node_1: node_1.o two_way_communication.o
	gcc -o node_1 two_way_communication.o node_1.o 

node_2: node_2.o two_way_communication.o
	gcc -o node_2 two_way_communication.o node_2.o 

node_1.o: node_1.c two_way_communication.h
	gcc -c node_1.c

node_2.o: node_2.c two_way_communication.h
	gcc -c node_2.c

two_way_communication.o: two_way_communication.h two_way_communication.c
	gcc -c two_way_communication.c

clean:
	rm -f *.o node_1 node_2

