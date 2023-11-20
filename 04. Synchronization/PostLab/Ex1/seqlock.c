#include "seqlock.h"

#undef READCOUNT

/* seqlock init*/
int pthread_seqlock_init(pthread_seqlock_t *seqlock) 
{
    int ret; 
    seqlock->seq_counter = 0; 
    ret = pthread_spin_init(&seqlock->spin_lock, PTHREAD_PROCESS_PRIVATE);
    /* the spin lock is to be operated on only by threads 
    * in the same process that calls pthread_spin_init */
    if (ret != 0) {
       printf("pthread_spin_init");
       exit(1);
    }

    return 0;
}

/* seqlock destroy*/
int pthread_seqlock_destroy(pthread_seqlock_t *seqlock)
{
    int ret; 
    seqlock->seq_counter = 0; 
    ret = pthread_spin_destroy(&seqlock->spin_lock);
    if (ret != 0) {
        printf("pthread_spin_destroy");
        exit(1);
    }

    printf("INFO: seqlock destroyed\n");

    return 0;
}

/* check whether writer active or not */
bool isWriting(pthread_seqlock_t *seqlock) 
{
    return seqlock->seq_counter % 2;
}

/* writer lock */
int pthread_seqlock_wrlock(pthread_seqlock_t *seqlock) 
{
    if(isWriting(seqlock)){
        printf("WARNING: Waiting for another writer\n");
        while(isWriting(seqlock));
    }

#ifdef READCOUNT
    if (seqlock->read_count > 1) {
        printf("WARNING: Waiting for reader = 1\n");
        while (seqlock->read_count > 1);
    } else if (seqlock->read_count == 1) {
        // Allow writer to proceed without waiting
        printf("INFO: Writer acquired lock with one reader\n");
        seqlock->seq_counter++;
        return 0;
    }
#endif

    pthread_spin_lock(&seqlock->spin_lock);
    seqlock->seq_counter++;

    return 0;
}

/* writer unlock */
int pthread_seqlock_wrunlock(pthread_seqlock_t *seqlock) 
{
    seqlock->seq_counter--;
    pthread_spin_unlock(&seqlock->spin_lock);

    return 0;
}

/* reader lock */
int pthread_seqlock_rdlock(pthread_seqlock_t *seqlock)
{
    if(isWriting(seqlock)){
        printf("WARNING: Waiting for writing \n");
        while(isWriting(seqlock));
    }

    seqlock->read_count++;

    return 0;
}

/* reader unlock */
int pthread_seqlock_rdunlock(pthread_seqlock_t *seqlock)
{
    if(isWriting(seqlock)) {
        printf("WARNING: Data is changing, read again \n");
        pthread_seqlock_rdlock(seqlock);
    } else {
        seqlock->read_count--;
    }

    return 0;
}


