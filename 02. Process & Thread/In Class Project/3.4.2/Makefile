msgsnd: msgsnd.o
	gcc msgsnd.o -o msgsnd
msgrcv: msgrcv.o
	gcc msgrcv.o -o msgrcv

msgsnd.o: msgsnd.c
	gcc -c msgsnd.c
msgrcv.o: msgrcv.c 
	gcc -c msgrcv.c

clean_msgsnd: 
	rm -r msgsnd.o msgsnd
clean_msgrcv: 
	rm -r msgrcv.o msgrcv
