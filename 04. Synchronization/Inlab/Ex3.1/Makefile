all: shrdmem_mutex

shrdmem_mutex: shared_buf_mutex.c
	gcc -c -o shrdmem_mutex.o shared_buf_mutex.c
	gcc -o shrdmem_mutex shrdmem_mutex.o

clean:
	rm *.o
	rm shrdmem_mutex
