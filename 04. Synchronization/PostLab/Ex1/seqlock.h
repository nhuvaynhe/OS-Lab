#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct seqlock {
    unsigned int seq_counter;
    pthread_spinlock_t spin_lock;
    unsigned int read_count;
} pthread_seqlock_t;

int pthread_seqlock_init(pthread_seqlock_t *seqlock);
int pthread_seqlock_destroy(pthread_seqlock_t *seqlock);

bool isWriting(pthread_seqlock_t *seqlock);
//---------------------------------------------------------
int pthread_seqlock_rdlock(pthread_seqlock_t *seqlock);
int pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock);

int pthread_seqlock_wrlock(pthread_seqlock_t *seqlock);
int pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock);

/* TODO: 
 * Implement readcount variable
 * that check if reader == 1 -> can write
 * else can not write
 */
