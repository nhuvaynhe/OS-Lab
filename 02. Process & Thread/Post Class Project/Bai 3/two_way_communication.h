#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>

#define PERMS 0644
#define MSQ_ID_THREAD_1 0x1023
#define MSQ_ID_THREAD_2 0x1a23

typedef struct message {
    long mtype;
    char mtext[200];
} BufferQueue;

typedef struct thread_passing {
    BufferQueue msq_buf;
    int msqID;
    int threadID;
} ThreadArgs;

void send_message(BufferQueue* buf, int msqid);
void receive_message(BufferQueue* buf, int msqid);
void *thread_receive(void* thread_args);
void *thread_send(void* thread_args);
