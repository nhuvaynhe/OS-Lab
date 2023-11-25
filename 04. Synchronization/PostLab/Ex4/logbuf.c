#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

/* init the semaphore for synchronization */
sem_t mutex, empty, full;

/* create logbuf with slot and length defined */
char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];
int count;

/* for the condition buffer is not full but no data left */
int seed = 0;

/* flush all the log to the screen */
void semaphore_init();
void semaphore_destroy();
void flushlog();

/* timer arguments for flushlog() */
struct _args
{
    unsigned int interval;
};

/* write into log */
void *wrlog(void *data)
{
    char str[MAX_LOG_LENGTH];
    int id = *(int*) data;

    usleep(20);
    sem_wait(&empty);
    sem_wait(&mutex);

    sprintf(str, "%d", id);
    strcpy(logbuf[count], str);

    count = (count > MAX_BUFFER_SLOT)? count :(count + 1); /* Only increase count to size MAX_BUFFER_SLOT*/
    //printf("wrlog(): %d \n", id);

    sem_post(&mutex);

    return 0;
}

/* flush data from log */
void flushlog()
{
    int i;
    char nullval[MAX_LOG_LENGTH];

    sem_wait(&mutex);
    //printf("flushlog()\n");
    sprintf(nullval, "%d", -1);
    for (i = 0; i < count; i++)
    {
        printf("Slot  %i: %s\n", i, logbuf[i]);
        strcpy(logbuf[i], nullval);

        sem_post(&empty);
    }

    fflush(stdout);

    /*Reset buffer */
    count = 0;

    sem_post(&mutex);

    return;
}

/* timer for flush log */
void *timer_start(void *args)
{
    while (1)
    {
        flushlog();
        /*Waiting until the next timeout */
        usleep(((struct _args *) args)->interval);
    }
}

int main()
{
    int i;
    count = 0;

    pthread_t tid[MAX_LOOPS];
    pthread_t lgrid;
    int id[MAX_LOOPS];

    struct _args args;
    args.interval = 500e3;
    /*500 msec ~ 500 * 1000 usec */

    semaphore_init();

    /*Setup periodically invoke flushlog() */
    pthread_create(&lgrid, NULL, &timer_start, (void*) &args);

    /*Asynchronous invoke task writelog */
    for (i = 0; i < MAX_LOOPS; i++)
    {
        id[i] = i;
        pthread_create(&tid[i], NULL, wrlog, (void*) &id[i]);
    }

    for (i = 0; i < MAX_LOOPS; i++)
        pthread_join(tid[i], NULL);

    semaphore_destroy();
    sleep(5);

    return 0;
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
    if(sem_init(&empty, 0, MAX_BUFFER_SLOT) == -1) {
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
