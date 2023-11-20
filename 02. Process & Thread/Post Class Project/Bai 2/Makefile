sum_multi_threads: sum_multi_threads.o
	gcc -o sum_multi_threads sum_multi_threads.o
sum_multi_threads.o: sum_multi_threads.c
	gcc -c sum_multi_threads.c

sum_serial: sum_serial.o
	gcc -o sum_serial sum_serial.o
sum_serial.o: sum_serial.c
	gcc -c sum_serial.c

clean:
	rm -rf *.o sum_multi_threads sum_serial