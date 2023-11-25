#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

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
void semaphore_init();
void semaphore_destroy();

void *producer(void *arg) {
    int i;
    int tid = *(int*) arg;
    for (i = 0; i < LOOPS; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);

        put(i);

        sem_post(&mutex);
        sem_post(&full); 

        printf("Producer %d put data %d\n", tid, i);
        sleep(1);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = *(int*) arg;
    while (tmp != -1) {
        sem_wait(&full);
        sem_wait(&mutex);

        tmp = get(); 
        printf("Consumer %d get data %d\n", tid, tmp);

        sem_post(&empty);
        sem_post(&mutex);

        sleep(2);
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    semaphore_init();

    for (i = 0; i < THREADS; i++) {
        tid[i] = i;
        // Create producer thread 
        pthread_create(&producers[i], NULL, producer, &tid[i]);
        pthread_create(&consumers[i], NULL, consumer, &tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    semaphore_destroy();

    return 0;
}

void put(int value) {
    buffer[fill] = value; // line f1
    fill = (fill + 1) % MAX_ITEMS; // line f2
}

int get() {
    int tmp = buffer[use]; // line g1
    buffer[use] = -1;
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}

void semaphore_init()
{
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
}

void semaphore_destroy()
{
  if (sem_destroy(&mutex) == -1) {
    perror("sem mutex destroy");
    exit(1);
  }

  if (sem_destroy(&full) == -1) {
    perror("sem full destroy");
    exit(1);
  }

  if (sem_destroy(&empty) == -1) {
    perror("sem empty destroy");
    exit(1);
  }
}
