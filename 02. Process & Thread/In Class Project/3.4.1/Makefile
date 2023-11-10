writer: writer.o
	gcc writer.o -o writer
reader: reader.o
	gcc reader.o -o reader

writer.o: writer.c
	gcc -c writer.c
reader.o: reader.c 
	gcc -c reader.c

clean_writer: 
	rm -r writer.o writer
clean_reader: 
	rm -r reader.o reader
