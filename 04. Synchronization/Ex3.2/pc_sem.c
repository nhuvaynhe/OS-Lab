#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_ITEMS 1
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS 100 // variable

// Init shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

// Init for semaphore
sem_t mutex, full, empty;

/* TODO: Fill in the synchronization stuff */
void put(int value); // put data into buffer 
int get(); // get data from buffer

void *producer(void *arg) {
    int i;
    int tid = (int) arg;
    for (i = 0; i < LOOPS; i++) {
        /* TODO: Fill in the synchronization stuff */
        put(i); // ine P2
        printf("Producer %d put data %d\n", tid, i);
        sleep(1);
        /* TODO: Fill in the synchronization stff */
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = (int) arg;
    while (tmp != -1) {
        sem_wait(&full);
        sem_wait(&mutex);

        /* TODO: FIll in the synchronization stuff */
        tmp = get(); // line C2
        printf("Consumer %d get data %d\n", tid, tmp);
        sleep(1);

        sem_post(&mutex);
        sem_post(&empty);
        /* TODO: Fill in the synchronization stuff */
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    if(sem_init(&mutex, 0, 1) == -1) {
        perror("sem mutex init");
        exit(1);
    }
    if(sem_init(&full, 0, 0) == -1) {
        perror("sem full init");
        exit(1);
    }
    if(sem_init(&empty, 0, MAX_ITEMS) == -1) {
        perror("sem empty init");
        exit(1);
    }
    
    /* TODO: Fill in the synchronization stuff */

    for (i = 0; i < THREADS; i++) {
        tid[i] = 1;
        // Create producer thread 
        pthread_create(&producers[i], NULL, producer, (void *) tid[i]);
        pthread_create(&consumers[i], NULL, consumer, (void *) tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    /* TODO: Fill in the synchronization stuff */

    return 0;
}

void put(int value) {
    sem_wait(&empty);
    sem_wait(&mutex);

    buffer[fill] = value; // line f1
    fill = (fill + 1) % MAX_ITEMS; // line f2

    sem_post(&mutex);
    sem_post(&full);
}

int get() {
    int tmp = buffer[use]; // line g1
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}


