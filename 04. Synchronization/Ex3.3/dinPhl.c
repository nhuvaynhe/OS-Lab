#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define N 5

pthread_mutex_t mtx;
pthread_cond_t chopstick[N];
pthread_cond_t enter;
bool EATING = 0;
int philosopher_count = 0;


void *philosopher(void*);
void eat(int);
void think(int);

int main()
{
    int i, a[N];
    pthread_t tid[N];

    /* BEGIN PROTECTION MECHANISM */
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&enter, NULL);     
    for (i = 0; i < N; i++)
        pthread_cond_init(&chopstick[i], NULL);     // attr NULL: cond only accessible 
                                                    // by threads on this process
    /* END PROTECTION MECHANISM */

    for (i = 0; i < 5; i++)
    {
        a[i] = i;
        pthread_create(&tid[i], NULL, philosopher, (void*) &a[i]);
    }
    for (i = 0; i < 5; i++)
        pthread_join(tid[i], NULL);
}

void *philosopher(void *num)
{
    int phil = *(int*) num;

    pthread_mutex_lock(&mtx);
    philosopher_count++;
    printf("philosopher %d has entered room\n", phil);

    // Ensure that all 5 philosophers have entered the room
    if (philosopher_count == N) {
        EATING = 1;
        pthread_cond_broadcast(&enter);
    } else {
        while(!EATING)
            pthread_cond_wait(&enter, &mtx);
    }

    pthread_mutex_unlock(&mtx);

    while (1)
    {
        /* PROTECTION MECHANISM */
        pthread_mutex_lock(&mtx);

        if (!EATING) {
            pthread_cond_wait(&chopstick[phil], &mtx);
            pthread_cond_wait(&chopstick[(phil +1) % N], &mtx);
        }

        EATING = 0;
        printf("Philosopher %d takes fork %d and %d\n", 
                phil, phil, (phil +1) % N);
        eat(phil);
        sleep(2);

        printf("Philosopher %d puts fork %d and %d down\n", 
                phil, (phil +1) % N, phil);

        EATING = 1;
        /* PROTECTION MECHANISM */
        pthread_cond_signal(&chopstick[phil]);
        pthread_cond_signal(&chopstick[(phil +1) % N]);

        pthread_mutex_unlock(&mtx);

        think(phil);
        sleep(1);
    }
}

void eat(int phil) 
{
    printf("Philosopher %d is eating\n", phil);
}

void think(int phil) 
{
    printf("Philosopher %d is thinking\n", phil);
}


