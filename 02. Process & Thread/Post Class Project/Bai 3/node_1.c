#include "two_way_communication.h"

/*
 * msg_send.c 
 */

int main()
{
    BufferQueue buf;
    BufferQueue* buf_ptr = &buf;
    buf.mtype = 1;
    int msqid_1, msqid_2;

    ThreadArgs thread1_args, thread2_args;
    ThreadArgs* args1_ptr = &thread1_args;
    ThreadArgs* args2_ptr = &thread2_args;
    pthread_t thread1, thread2;

    /*
     * create two message queue
     */

    if ((msqid_1 = msgget(MSQ_ID_THREAD_1, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    if ((msqid_2 = msgget(MSQ_ID_THREAD_2, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    printf("\nmessage queue ready: %d - %d \n", msqid_1, msqid_2);
    printf("======================================================\n\n");
    printf("################# NODE 1 #################\n");
    printf("Type \" end \" to stop receiving message from other process\n");
    printf("Type \" quit \" to exit the program\n\n");
    printf("======================================================\n");
    /*
     * create independt thread each of which will execute function
     */

    thread1_args.msqID = msqid_1;
    thread1_args.threadID = 1;

    thread2_args.msqID = msqid_2;
    thread2_args.threadID = 2;

    pthread_create(&thread1, NULL, &thread_send, args1_ptr);
    pthread_create(&thread2, NULL, &thread_receive, args2_ptr);

    /* wait for the two threads to exit. */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    if (msgctl(msqid_1, IPC_RMID, NULL) == -1) { 
        perror("msgctl");
        exit(1);
    }
    
    return 0;
}
