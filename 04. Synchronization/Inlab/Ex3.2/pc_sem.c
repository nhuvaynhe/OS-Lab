#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_ITEMS 2
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS 3 * MAX_ITEMS // variable

// Init shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

// Init for semaphore
sem_t mutex, full, empty;

void put(int value); // put data into buffer 
int get(); // get data from buffer

void *producer(void *arg) {
    int i;
    int tid = (int) arg;
    for (i = 0; i < LOOPS; i++) {
        put(i);
        printf("Producer %d put data %d\n", tid, i);
        sleep(1);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = (int) arg;
    while (tmp != -1) {
        sem_wait(&full);
        sem_wait(&mutex);

        tmp = get(); 
        printf("Consumer %d get data %d\n", tid, tmp);

        sem_post(&mutex);
        sem_post(&empty);

        sleep(3);
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

    for (i = 0; i < THREADS; i++) {
        tid[i] = i;
        // Create producer thread 
        pthread_create(&producers[i], NULL, producer, (void *) tid[i]);
        pthread_create(&consumers[i], NULL, consumer, (void *) tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

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
    buffer[use] = -1;
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}


