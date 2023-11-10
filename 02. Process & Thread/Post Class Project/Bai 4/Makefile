all: mmap_send mmap_receive

mmap_send: mmap_send.o
	gcc -o mmap_send mmap_send.o

mmap_receive: mmap_receive.o
	gcc -o mmap_receive mmap_receive.o

mmap_send.o: mmap_send.c
	gcc -c mmap_send.c

mmap_receive.o: mmap_receive.c
	gcc -c mmap_receive.c

clean: 
	rm -rf *.o mmap_send mmap_receive
