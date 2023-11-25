#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct seqlock {
    unsigned int seq_counter;
    unsigned int read_count;
} pthread_seqlock_t;

/* seqlock init*/
static inline void pthread_seqlock_init(pthread_seqlock_t *seqlock)
{
    seqlock->seq_counter = 0; 
}

/* seqlock destroy*/
static inline void pthread_seqlock_destroy(pthread_seqlock_t *seqlock)
{
    seqlock->seq_counter = -1; 
    printf("INFO: seqlock is destroyed\n");
}

/* check whether writer active or not */
bool isWriting(pthread_seqlock_t *seqlock) 
{
    return seqlock->seq_counter % 2;
}

/* writer lock */
static inline void pthread_seqlock_wrlock(pthread_seqlock_t *seqlock)
{
    if(isWriting(seqlock)){
        printf("WRITER: Waiting for another writer\n");
        while(isWriting(seqlock));
    }

    /* Only access when there are one or zero reader */
    if(seqlock->read_count > 1){
        printf("WRITER: Waiting for reader\n");
        while(seqlock->read_count > 1);
    }

    seqlock->seq_counter++;
}

/* writer unlock */
static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock)
{
    seqlock->seq_counter--;
}

/* reader lock */
static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *seqlock)
{
    if(isWriting(seqlock)){
        printf("READER: Waiting for writing \n");
        while(isWriting(seqlock));
    }

    printf("READER: Acquire the lock\n");
    seqlock->read_count++;

    /* Successfully acquire, return 1 */
    return 1;
}

/* reader unlock */
static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock)
{
    if(isWriting(seqlock)) {
        printf("WARNING: Data is changing, read again \n");
        pthread_seqlock_rdlock(seqlock);
    }
    
    seqlock->read_count--;

    /* Successfully unlock, return 1 */
    return 1;
}

