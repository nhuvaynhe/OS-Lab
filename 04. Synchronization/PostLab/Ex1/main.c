#include "seqlock.h"

#define NUM_OF_READERS 5

pthread_t writer_t, reader_t[5];
pthread_seqlock_t lock;
long sum = 0;
bool inProcess = true;

void* writer(void *arg)
{
    int i;
    int max = *(int*) arg;

    for(i = 0; i < max; i++)
    {
        pthread_seqlock_wrlock(&lock);
        printf("\n-------> IN WRITE <-------\n");
        usleep(5000); // 1 millisecond delay

        sum += i;
        printf("INFO: New sum %ld\n\n", sum);
                      
        pthread_seqlock_wrunlock(&lock);
        usleep(5000); // 1 millisecond delay
    }

    inProcess = false;
    return NULL;
}

void* reader(void *arg)
{
    int tid = *(int *)arg;

    while(inProcess) {
        pthread_seqlock_rdlock(&lock);

        int value = sum;
        printf("Thread ID %d read sum equal %ld\n", tid, sum);

        pthread_seqlock_rdunlock(&lock);
        usleep(5000 + tid * 1000); // 1 millisecond delay
    }

    return NULL;
}

int main()
{
    pthread_seqlock_init(&lock);

    int MAX = 100;

    for(int i = 0; i < 5; i++)
        pthread_create(&reader_t[i], NULL, &reader, &i);
    
    pthread_create(&writer_t, NULL, &writer, &MAX);
    /* Wait for the writer finish */
    pthread_join(writer_t, NULL);

    /* Wait for all reader finish */
    for(int i = 0; i < 5; i++)
        pthread_join(reader_t[i], NULL);

    pthread_seqlock_destroy(&lock);

    return 0;
}

