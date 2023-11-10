#include "two_way_communication.h"

void send_message(BufferQueue* buf, int msqid)
{
    int len;

    while(fgets(buf->mtext, sizeof buf->mtext, stdin) != NULL) {
        len = strlen(buf->mtext);
        /* remove newline at end, if it exists */
        if (buf->mtext[len -1] == '\n') buf->mtext[len -1] = '\0';

        /* type "quit" to stop sending */
        if (strcmp(buf->mtext, "quit") == 0) {
            /* send end to end other thread*/
            strcpy(buf->mtext, "end");
            msgsnd(msqid, buf, len +1, 0);

            printf("--> done sending message!\n");
            break;
        }

        if (msgsnd(msqid, buf, len +1, 0) == -1) /* +1 for '\0' */
            perror("msgsnd");
    }
}

void receive_message(BufferQueue* buf, int msqid)
{
    int toend;

    for(;;) { 
        if (msgrcv(msqid, buf, sizeof(buf->mtext), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("message received: \"%s\"\n", buf->mtext);
        
        /* type "end" to stop received */
        toend = strcmp(buf->mtext, "end");
        if (toend == 0) {
            printf("--> message queue: done receiving messages.\n");
            break;
        }
    }
}

void *thread_send(void* thread_args)
{
    ThreadArgs *args = (ThreadArgs *) thread_args;

    BufferQueue buf = args->msq_buf;
    BufferQueue* buf_ptr = &buf;
    buf.mtype = 1;

    int msqid = args->msqID;
    int threadID = args->threadID;
    printf("Thread %d with queue ID \"%d\": SEND\n", threadID, msqid);
    send_message(buf_ptr, msqid);

    pthread_exit(NULL);
}

void *thread_receive(void* thread_args)
{
    ThreadArgs *args = (ThreadArgs *) thread_args;

    BufferQueue buf = args->msq_buf;
    BufferQueue* buf_ptr = &buf;

    int msqid = args->msqID;
    int threadID = args->threadID;
    printf("Thread %d with queue ID \"%d\": RECEIVE\n", threadID, msqid);

    receive_message(buf_ptr, msqid);
    
    pthread_exit(NULL);
}
